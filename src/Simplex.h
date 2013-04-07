/*
 * Simplex.h
 *
 *  Created on: 3 avr. 2013
 *      Author: Arnaud
 */

#ifndef SIMPLEX_H_
#define SIMPLEX_H_

#include "../lib/Eigen/Core"
#include <utility>
#include "LinearProblem.h"

class Simplex {
public:
	Simplex(LinearProblem* lp);
	virtual ~Simplex();


	Eigen::MatrixXf tab;


	int step;

//	std::pair<int, int> pivot;

	Eigen::VectorXf run();
	Eigen::VectorXf best;

private:
	int findPivotColumn();
	int findPivotRow(int column);

	void pivot(int row, int col);
	void getBest();
	int find_basis_variable(int col);



};

#endif /* SIMPLEX_H_ */
