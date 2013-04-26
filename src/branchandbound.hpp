/*
 * BranchAndBound.h
 *
 *  Created on: Apr 21, 2013
 *      Author: Matthieu
 */

#ifndef BRANCHANDBOUND_H_
#define BRANCHANDBOUND_H_

#include "LinearProblem.h"

class BranchAndBound{
private :
	LinearProblem * lp;

public :
	BranchAndBound(LinearProblem * lp);
	void run();
	void reduce_matrix();
};


#endif /* BRANCHANDBOUND_H_ */
