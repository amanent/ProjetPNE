/*
 * LinearProblem.h
 *
 *  Created on: 3 avr. 2013
 *      Author: Arnaud
 */

#ifndef LINEARPROBLEM_H_
#define LINEARPROBLEM_H_

#include "Eigen/Core"

class LinearProblem {
public:

	enum Type{MIN, MAX}

	LinearProblem();
	virtual ~LinearProblem();

	Eigen::MatrixXf constraints;
	Eigen::VectorXf objective;
	Type type;

	LinearProblem* createDual();
	LinearProblem* dual;

};

#endif /* LINEARPROBLEM_H_ */
