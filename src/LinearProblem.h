/*
 * LinearProblem.h
 *
 *  Created on: 3 avr. 2013
 *      Author: Arnaud
 */

#ifndef LINEARPROBLEM_H_
#define LINEARPROBLEM_H_

#include "./lib/Eigen/Core"
#include "Probleme.h"
#include <iostream>

class LinearProblem {
public:

	enum Type{MIN, MAX};

	LinearProblem();
	LinearProblem(Probleme * p, LinearProblem::Type t);
	LinearProblem dualize(LinearProblem::Type t);
	virtual ~LinearProblem();

	Eigen::MatrixXf constraints;
	Eigen::VectorXf objective;
	Type type;

	int nbVars;
	int nbConstraints;

	LinearProblem* createDual();
	LinearProblem* dual;

	void switchMode();
	void minimize();
	void maximize();
	void updateSize();

private:


};

#endif /* LINEARPROBLEM_H_ */
