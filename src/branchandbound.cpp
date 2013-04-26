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
			return tmp;
		}
	}
	return -1;
}

void BranchAndBound::run(){
	/* Pour chaque noeud, faire une eval via le simplex,
	 * si les valeurs ne sont pas entieres, il faut fixer celles la
	 * si (dans le cas du min) local bound > lower bound => cut
	 */
	LinearProblem lp = *(this->lp);
	Simplex * ss = new Simplex(&lp);
	ss->run();
	int res = this->getFirstNonIntegerVar(ss->best);
	int localBound;
	Eigen::VectorXf tmp(ss->best.rows());
	for(int i = 0; i < ss->best.rows(); i++){
		tmp.row(i) = ss->best.row(i) * lp.constraints.col(0).row(i);
	}
	localBound = tmp.sum();

	if(lp.type == LinearProblem::MIN){
		if(localBound >= globalBound)
			return;
		else
			globalBound = localBound;
	}else{
		if(localBound < globalBound)
			return;
		else
			globalBound = localBound;
	}
	if(res == -1){
		this->best = ss->best;
		return;
	}
	else{
		Eigen::VectorXf vect = Eigen::VectorXf(lp.constraints.ColsAtCompileTime);
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
			// set vector
			for(int i = 0 ; i < lp.constraints.ColsAtCompileTime; i++){
				if(i == 0)
					vect[i] = obj;
				else if (i == res)
					vect[i] = coeff;
				else
					vect[i] = 0;
			}
			this->step(lp,vect);
		}
	}

	std::cout << this->best << std::endl;
}

void BranchAndBound::step(LinearProblem lp, Eigen::VectorXf vect){

	lp.constraints.conservativeResize(lp.constraints.rows()+1, lp.constraints.cols());
	lp.constraints.row(lp.constraints.rows() - 1) = vect;

	Simplex * ss = new Simplex(&lp);
	ss->run();
	int res = this->getFirstNonIntegerVar(ss->best);
	int localBound;
	Eigen::VectorXf tmp;
	for(int i = 0; i < ss->best.rows(); i++){
		tmp.row(i) = ss->best.row(i) * lp.constraints.col(0).row(i);
	}
	localBound = tmp.sum();

	if(lp.type == LinearProblem::MIN){
		if(localBound >= globalBound)
			return;
		else
			globalBound = localBound;
	}else{
		if(localBound < globalBound)
			return;
		else
			globalBound = localBound;
	}
	if(res == -1){
		this->best = ss->best;
		return;
	}
	else{
		Eigen::VectorXf vect = Eigen::VectorXf(lp.constraints.ColsAtCompileTime);
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
			// set vector
			for(int i = 0 ; i < lp.constraints.ColsAtCompileTime; i++){
				if(i == 0)
					vect[i] = obj;
				else if (i == res)
					vect[i] = coeff;
				else
					vect[i] = 0;
			}
			this->step(lp,vect);
		}
	}
}
