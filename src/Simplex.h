/*
 * Simplex.h
 *
 *  Created on: 3 avr. 2013
 *      Author: Arnaud
 */

#ifndef SIMPLEX_H_
#define SIMPLEX_H_

#include "Eigen/Core"
#include <utility>


class Simplex {
public:
	Simplex();
	virtual ~Simplex();


	Eigen::MatrixXf tab;


	int step;
	int nbCol;
	int nbLines;


	void evaluateMatrixSize(Eigen::Matrix m);
	std::pair<int, int> pivot;





};

#endif /* SIMPLEX_H_ */
