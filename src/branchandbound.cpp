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
		this->globalBound = FLT_MAX;
	else
		this->globalBound = FLT_MIN;
	debug = true;
}

int BranchAndBound::getFirstNonIntegerVar(Eigen::VectorXf best){
	for(int i =0; i < best.rows(); i++){
		int tmp = (int)best[i];
		if((float)tmp != best[i]){
			return i;
		}
	}
	return -1;
}

int BranchAndBound::getFirstNonIntegerVar(Eigen::VectorXf best, std::vector<int> & vars_set){
	for(int i =0; i < best.rows(); i++){
		int tmp = (int)best[i];
		if((float)tmp != best[i] && std::find(vars_set.begin(), vars_set.end(), i) == vars_set.end()){
			return i;
		}
	}
	return -1;
}

void BranchAndBound::run(){
	/* Pour chaque noeud, faire une eval via le simplex,
	 * si les valeurs ne sont pas entieres, il faut fixer celles la
	 * si (dans le cas du min) local bound > lower bound => cut
	 */

	/* On crée un vector dans le lequel on met toutes les variables ayant
	 * déjà été fixé
	 */


	std::cout << "b&b... " <<std::endl;
	std::vector<int> vars_set;
	LinearProblem lp = *(this->lp);

	/**
	 * On iniatilise le nombre de step maximal a faire
	 */
	int step_max = lp.nbVars;

	// On créer le simplex et on le fait tourner
	Simplex ss(&lp,!lp.isDual);
	if(debug)
		std::cout << ss.tab << std::endl;
	ss.run();

	if(debug)
		std::cout << ss.best << std::endl;
	if(debug)
		std::cout << "ok"<< std::endl;

	// on récupère la première variable non entiere renvoyée par le simplex
	int res = this->getFirstNonIntegerVar(ss.best);

	if(debug)
		std::cout << " indice premiere val non entiere :" << res <<std::endl;
	// on la met dans le vecteur vars_set car on va maintenant la fixer
	vars_set.push_back(res);
	//std::cout << "simplex best " << std::endl << ss.best << std::endl;

	// si res renvoi -1 ce la signifie qu'on a pas de var non entiere dans le vecteur donc on s'arrete
	if(res == -1){
		best.resize(ss.best.rows());
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
				else if (i == res+1)
					vect[i] = coeff;
				else
					vect[i] = 0;
			}

			if(debug)
				std::cout << "vecteur contrainte : " <<std::endl;
			if(debug)
				std::cout << vect <<std::endl;
			// on effectue une étape du branch and bound
			if(this->step(lp,vect, step_max - 1, vars_set)){
				// on calcule la valeur de la fonction objective avec le meilleur vecteur
				float localBound;
				Eigen::VectorXf tmp(this->best.rows());
				for(int i = 0; i < this->best.rows(); i++){
					tmp.row(i) = this->best.row(i) * lp.objective.row(i);
				}
				localBound = tmp.sum();
				if(debug)
					std::cout << "lb " << localBound << " gb " << globalBound << std::endl;
				// on coupe si besoin, sinon on met à jour le vecteur best
				if(lp.type == LinearProblem::MIN){
					if(localBound >= globalBound){
						return;
					}else{
						this->best = ss.best;
						globalBound = localBound;
						//break;
					}
				}else{
					if(localBound < globalBound)
						return;
					else{
						this->best = ss.best;
						globalBound = localBound;
						//break;
					}
				}
			}
			if(debug)
				std::cout << "********** FIN STEP 1 ************" << std::endl;
		}
	}

	//std::cout << "Best : " << this->best << std::endl;*/
}

bool BranchAndBound::step(LinearProblem lp, Eigen::VectorXf vect, int step, std::vector<int>  vars_set){
	if(debug)
		std::cout << "************** STEP " << (lp.nbVars - step) << " ***************" << std::endl;
	//std::cout << "rows : " << lp.constraints.rows() << std::endl;

	/*
	 * On redimensionne la matrice contrainte pour y ajouter notre nouvelle contrainte
	 */
	lp.constraints.conservativeResize(lp.constraints.rows() + 1, lp.constraints.cols());
	lp.constraints.row(lp.constraints.rows() - 1) = vect;
	//lp.nbConstraints = lp.constraints.rows();
	lp.nbConstraints++;

	if(debug)
		std::cout << "Matrice de contraintes : "<< std::endl <<  lp.constraints << std::endl <<std::endl;
	//std::cout << lp.constraints << std::endl;

	// on créer un simplex que l'on fait tourner
	Simplex ss(&lp,!lp.isDual);
	//if(debug)
	//std::cout << ss.tab << std::endl;
	ss.run();
	//if(debug)
	//std::cout << ss.tab << std::endl;
	if(debug)
		std::cout << "best "<< std::endl << ss.best << std::endl;

	// de même que pour la méthode run, on calcule la valeur de la fonction objective avec le vecteur renvoyé par le simplex
	int res = this->getFirstNonIntegerVar(ss.best, vars_set);
	if(debug)
		std::cout << "res " << res << std::endl;
	float localBound;
	//std::cout << "simplex best " << std::endl << ss.best << std::endl;
	Eigen::VectorXf tmp(ss.best.rows());
	for(int i = 0; i < ss.best.rows(); i++){
		tmp.row(i) = ss.best.row(i) * lp.objective.row(i);
	}
	localBound = tmp.sum();
	//std::cout << "global bound " << globalBound << " local bound " << localBound << std::endl;

	// on coupe si nécessaire


	// si pas de valeur non entier, c'est good
	if(res == -1){
		if(debug)
			std::cout << "lb " << localBound << " gb " << globalBound << std::endl;

		if(debug)
			std::cout << "vars_set : ";
		for(unsigned int  i = 0; i < vars_set.size(); ++i){
			if(debug)
				std::cout << vars_set[i] << " ";
		}
		if(debug)
			std::cout << std::endl;
		if(debug)
			std::cout << "The non integer ones "  << this->getFirstNonIntegerVar(ss.best) << std::endl;

		if(std::find(vars_set.begin(), vars_set.end(),this->getFirstNonIntegerVar(ss.best)) != vars_set.end()){
			return false;
		}
		/*this->best = ss.best;
		return true;*/
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
	// si on est arrivé a la fin de notre nombre max d'itérations, on arrete
	/*else if(step == 0)
		return false;*/
	else{
		vars_set.push_back(res);
		for(int k = 0; k <= 1; k++){
			Eigen::VectorXf vect = Eigen::VectorXf(lp.constraints.cols());
			// sinon on fait la meme chose que pour la méthode run
			int obj;
			int coeff;
			if(k == 0){
				obj = 0;
				coeff = 1;
			}else{
				obj = -1;
				coeff = -1;
			}
			//std::cout << "res " << res << std::endl;
			// set vector
			if(debug)
				std::cout << "obj " << obj << " coeff " << coeff << " res " << res <<std::endl;
			for(int i = 0 ; i < vect.rows(); i++){
				if(i == 0)
					vect[i] = obj;
				else if (i == res + 1)
					vect[i] = coeff;
				else
					vect[i] = 0;
			}
			if(debug)
				std::cout << "vecteur contrainte : " <<std::endl;
			if(debug)
				std::cout << vect <<std::endl;
			if(this->step(lp,vect,step - 1, vars_set)){
				//return true;
				/*int localBound;
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
				}*/
			}
			if(debug)
				std::cout << "********** FIN STEP "<< step << "***************" << std::endl;
		}
	}
	return false;
}

float BranchAndBound::getBestResult(){
	Eigen::VectorXf tmp(this->best.rows());
	for(int i = 0; i < this->best.rows(); i++){
		//if(i < this->lp->objective.rows())
			tmp.row(i) = this->best.row(i) * this->lp->objective.row(i);
	}
	return tmp.sum();
}
