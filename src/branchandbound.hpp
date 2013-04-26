/*
 * BranchAndBound.h
 *
 *  Created on: Apr 21, 2013
 *      Author: Matthieu
 */

#ifndef BRANCHANDBOUND_H_
#define BRANCHANDBOUND_H_

#include "LinearProblem.h"
#include "Simplex.h"
#include <vector>
#include <climits>
#include <iostream>

class BranchAndBound{
private :
	LinearProblem * lp;
	Eigen::VectorXf best;
	int globalBound;

public :
	BranchAndBound(LinearProblem * lp);
	void run();
	int getFirstNonIntegerVar(Eigen::VectorXf best);
	bool step(LinearProblem lp, Eigen::VectorXf vect, int step);
	Eigen::VectorXf getBest(){return best;}
};



#endif /* BRANCHANDBOUND_H_ */
