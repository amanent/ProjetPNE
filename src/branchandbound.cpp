/*
 * BranchAndBound.cpp
 *
 *  Created on: Apr 21, 2013
 *      Author: Matthieu
 */

#include "branchandbound.hpp"
BranchAndBound::BranchAndBound(LinearProblem * lp){
	this->lp = lp;
	if(lp->type == LinearProblem::MIN)
		this->globalBound = INT_MAX;
	else
		this->globalBound = INT_MIN;
}

int BranchAndBound::getFirstNonIntegerVar(Eigen::VectorXf best){
	for(int i =1; i < best.rows(); i++){
		int tmp = (int)best[i];
		if((float)tmp != best[i]){
			return i;
		}
	}
	return -2;
}

int BranchAndBound::getFirstNonIntegerVar(Eigen::VectorXf best, std::vector<int> & vars_set){
	for(int i = 1; i < best.rows(); i++){
		int tmp = (int)best[i];
		if((float)tmp != best[i] && std::find(vars_set.begin(), vars_set.end(), i + 1) == vars_set.end()){
			return i;
		}
	}
	return -2;
}

void BranchAndBound::run(){
	/* Pour chaque noeud, faire une eval via le simplex,
	 * si les valeurs ne sont pas entieres, il faut fixer celles la
	 * si (dans le cas du min) local bound > lower bound => cut
	 */

	/* On crée un vector dans le lequel on met toutes les variables ayant
	 * déjà été fixé
	 */
	std::vector<int> vars_set;
	LinearProblem lp = *(this->lp);

	/**
	 * On iniatilise le nombre de step maximal a faire
	 */
	int step_max = lp.nbVars;

	// On créer le simplex et on le fait tourner
	Simplex ss(&lp);
	ss.run();

	// on récupère la première variable non entier renvoyer par le simplex
	int res = this->getFirstNonIntegerVar(ss.best) + 1;
	std::cout << res << std::endl;
	// on la met dans le vecteur vars_set car on va maintenant la fixer
	vars_set.push_back(res);
	//std::cout << "simplex best " << std::endl << ss.best << std::endl;

	std::cout << "b&b... ";

	// si res renvoi -1 ce la signifie qu'on a pas de var non entiere dans le vecteur donc on s'arrete
	if(res == -1){
		this->best = ss.best;
		return;
	}
	else{
		// Pour mettre une valeur à 0 ou 1, il faut rajouter une contrainte ayant pour objectif obj et pour coefficient de cette var coeff
		int obj;
		int coeff;
		// on effectue deux tours de boucle, une pour mettre a 0 (k=0) et une pour mettre a 1 (k=1)
		for(int k = 0; k <= 1; k++){
			// on créer le vecteur qui va représenter cette contrainte
			Eigen::VectorXf vect = Eigen::VectorXf(lp.constraints.cols());
			// POur mettre a 0 une var, il faut mettre l'objectif a 0 et le coefficient de la variable a 1 (x <= 0)
			if(k == 0){
				obj = 0;
				coeff = 1;
			}
			// Pour mettre a 1 une var, il faut mettre  l'objectif a -1 et le coefficient de la variable a -1 (-x <= -1)
			else{
				obj = -1;
				coeff = -1;
			}
			//std::cout << "res " << res << std::endl;

			// On remplie ensuite le vecteur contrainte avec les bonnes valeurs (obj,0,0,...,coeff,0,...)
			for(int i = 0 ; i < lp.constraints.cols(); i++){
				if(i == 0)
					vect[i] = obj;
				else if (i == res)
					vect[i] = coeff;
				else
					vect[i] = 0;
			}
			// on effectue une étape du branch and bound
			if(this->step(lp,vect, step_max - 1, vars_set)){
				// on calcule la valeur de la fonction objective avec le meilleur vecteur
				int localBound;
				Eigen::VectorXf tmp(this->best.rows());
				for(int i = 0; i < this->best.rows(); i++){
					tmp.row(i) = this->best.row(i) * lp.objective.row(i);
				}
				localBound = tmp.sum();

				// on coupe si besoin, sinon on met à jour le vecteur best
				if(lp.type == LinearProblem::MIN){
					if(localBound >= globalBound)
						return;
					else{
						this->best = ss.best;
						globalBound = localBound;
					}
				}else{
					if(localBound < globalBound)
						return;
					else{
						this->best = ss.best;
						globalBound = localBound;
					}
				}
			}
		}
	}

	//std::cout << "Best : " << this->best << std::endl;
}

bool BranchAndBound::step(LinearProblem lp, Eigen::VectorXf vect, int step, std::vector<int>  vars_set){
	std::cout << "************** STEP " << (lp.nbVars - step) << " ***************" << std::endl;
	//std::cout << "rows : " << lp.constraints.rows() << std::endl;

	/*
	 * On redimensionne la matrice contrainte pour y ajouter notre nouvelle contrainte
	 */
	lp.constraints.conservativeResize(lp.constraints.rows()+1, lp.constraints.cols());
	lp.constraints.row(lp.constraints.rows() - 1) = vect;
	lp.nbConstraints = lp.constraints.rows();



	//std::cout << lp.constraints << std::endl;

	// on créer un simplex que l'on fait tourner
	Simplex ss(&lp);
	ss.run();

	// de même que pour la méthode run, on calcule la valeur de la fonction objective avec le vecteur renvoyé par le simplex
	// tout en vérifiant qu'elle n'a pas déjà été fixée
	int res = this->getFirstNonIntegerVar(ss.best, vars_set) + 1;
	int localBound;
	//std::cout << "simplex best " << std::endl << ss.best << std::endl;
	Eigen::VectorXf tmp(ss.best.rows());
	for(int i = 0; i < ss.best.rows(); i++){
		tmp.row(i) = ss.best.row(i) * lp.objective.row(i);
	}
	localBound = tmp.sum();
	//std::cout << "global bound " << globalBound << " local bound " << localBound << std::endl;

	// on coupe si nécessaire
	if(lp.type == LinearProblem::MIN){
		if(localBound >= globalBound){
			return false;
		}
		else{
			this->best = ss.best;
			globalBound = localBound;
		}
	}else{
		if(localBound <= globalBound){
			return false;
		}else{
			this->best = ss.best;
			globalBound = localBound;
		}
	}

	// si pas de valeur non entier, c'est good
	if(res == -1){
		this->best = ss.best;
		return true;
	}
	// si on est arrivé a la fin de notre nombre max d'itérations, on arrete
	else if(step == 0)
		return false;
	else{
		// sinon on fait la meme chose que pour la méthode run
		Eigen::VectorXf vect = Eigen::VectorXf(lp.constraints.cols());
		int obj;
		int coeff;
		for(int k = 0; k <= 1; k++){
			if(k == 0){
				obj = 0;
				coeff = 1;
			}else{
				obj = -1;
				coeff = -1;
			}
			//std::cout << "res " << res << std::endl;
			// set vector
			for(int i = 0 ; i < lp.constraints.cols(); i++){
				if(i == 0)
					vect[i] = obj;
				else if (i == res)
					vect[i] = coeff;
				else
					vect[i] = 0;
			}
			if(this->step(lp,vect,step - 1, vars_set)){
				//return true;
				int localBound;
				//std::cout << "simplex best " << std::endl << ss.best << std::endl;
				Eigen::VectorXf tmp(ss.best.rows());
				for(int i = 0; i < ss.best.rows(); i++){
					tmp.row(i) = ss.best.row(i) * lp.objective.row(i);
				}
				localBound = tmp.sum();
				if(lp.type == LinearProblem::MIN){
					if(localBound >= globalBound){
						return false;
					}
					else{
						this->best = ss.best;
						globalBound = localBound;
					}
				}else{
					if(localBound <= globalBound){
						return false;
					}else{
						this->best = ss.best;
						globalBound = localBound;
					}
				}

			}
		}
	}
	return false;
}

float BranchAndBound::getBestResult(){
	Eigen::VectorXf tmp(this->best.rows());
	for(int i = 0; i < this->best.rows(); i++){
		tmp.row(i) = this->best.row(i) * this->lp->objective.row(i);
		//std::cout << this->best.row(i) << "*" << this->lp->objective.row(i) << std::endl;
	}
	return (float)tmp.sum();
}
