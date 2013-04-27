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
	for(int i =1; i < best.rows(); i++){
		int tmp = (int)best[i];
		if((float)tmp != best[i] && std::find(vars_set.begin(), vars_set.end(), i + 1) == vars_set.end()){
			return i;
		}
	}
	return -2;
}

void BranchAndBound::run(){
	std::cout << "Demarrage du b&b" << std::endl;
	/* Pour chaque noeud, faire une eval via le simplex,
	 * si les valeurs ne sont pas entieres, il faut fixer celles la
	 * si (dans le cas du min) local bound > lower bound => cut
	 */

	std::vector<int> vars_set;
	LinearProblem lp = *(this->lp);
	int step_max = lp.nbVars;
	Simplex ss(&lp);
	ss.run();
	int res = this->getFirstNonIntegerVar(ss.best) + 1;
	vars_set.push_back(res);
	//std::cout << "simplex best " << std::endl << ss.best << std::endl;
	int localBound;
	Eigen::VectorXf tmp(ss.best.rows());
	for(int i = 0; i < ss.best.rows(); i++){
		tmp.row(i) = ss.best.row(i) * lp.objective.row(i);
	}
	localBound = tmp.sum();
	std::cout << "b&b... ";
	/*if(lp.type == LinearProblem::MIN){
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
	}*/
	if(res == -1){
		this->best = ss.best;
		return;
	}
	else{
		int obj;
		int coeff;
		for(int k = 0; k <= 1; k++){
			Eigen::VectorXf vect = Eigen::VectorXf(lp.constraints.cols());
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
			this->step(lp,vect, step_max - 1, vars_set);
		}
	}

	//std::cout << "Best : " << this->best << std::endl;
}

bool BranchAndBound::step(LinearProblem lp, Eigen::VectorXf vect, int step, std::vector<int>  vars_set){
	//std::cout << "************** STEP " << (lp.nbVars - step) << " ***************" << std::endl;
	//std::cout << "rows : " << lp.constraints.rows() << std::endl;
	lp.constraints.conservativeResize(lp.constraints.rows()+1, lp.constraints.cols());
	lp.constraints.row(lp.constraints.rows() - 1) = vect;
	//std::cout << lp.constraints << std::endl;
	lp.nbConstraints = lp.constraints.rows();
	Simplex ss(&lp);
	ss.run();
	int res = this->getFirstNonIntegerVar(ss.best, vars_set) + 1;
	int localBound;
	//std::cout << "simplex best " << std::endl << ss.best << std::endl;
	Eigen::VectorXf tmp(ss.best.rows());
	for(int i = 0; i < ss.best.rows(); i++){
		tmp.row(i) = ss.best.row(i) * lp.objective.row(i);
	}
	localBound = tmp.sum();
	//std::cout << "global bound " << globalBound << " local bound " << localBound << std::endl;

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
	if(res == -1){
		this->best = ss.best;
		return true;
	}
	else if(step == 0)
		return false;
	else{
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
			if(this->step(lp,vect,step - 1, vars_set))
				return true;
		}
	}
	/*if(lp.type == LinearProblem::MIN){
		if(localBound >= globalBound){
			return false;
		}
		else{
			this->best = ss.best;
			globalBound = localBound;
		}
	}else{
		if(localBound < globalBound){
			return false;
		}else{
			this->best = ss.best;
			globalBound = localBound;
		}
	}*/
	return false;
}

float BranchAndBound::getBestResult(){
	Eigen::VectorXf tmp(this->best.rows());
	for(int i = 0; i < this->best.rows(); i++){
		tmp.row(i) = this->best.row(i) * this->lp->objective.row(i);
	}
	return tmp.sum();
}
