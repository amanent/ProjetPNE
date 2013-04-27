/*
 * Simplex.cpp
 *
 *  Created on: 3 avr. 2013
 *      Author: Arnaud
 */

#include "Simplex.h"
#include <iostream>

Simplex::Simplex(LinearProblem* lp) : step(0) {
	// TODO Auto-generated constructor stub
	problem = lp;

	tab.resize(lp->constraints.rows() + 1, lp->constraints.rows()+ lp->constraints.cols());
	/*if(lp->type == LinearProblem::MIN)
		lp->maximize();*/
	if(lp->type == LinearProblem::MAX)
		lp->minimize();
	//std::cout << "lp apres maximisation " << std::endl << std::endl << lp->objective << std::endl;
	tab(0,0) = 0;

	for(int i(1); i < lp->constraints.cols(); ++i){
		tab(0, i) = lp->objective(i - 1);
	}
	for(int i(lp->constraints.cols()); i < tab.cols(); ++i){
		tab(0, i) = 0;
	}

	for(int i(0); i < lp->constraints.rows(); ++i){
		for(int j(0); j < lp->constraints.cols(); ++j)
			tab(i+1, j) = lp->constraints(i, j);

	}

	for(int i=1; i<tab.rows(); i++) {
		for(int j=1; j<tab.rows(); j++)
			tab(i, j + lp->constraints.cols()-1) = (i==j);
	}

	best.resize(lp->constraints.cols() - 1);

	top.resize(lp->nbVars + lp->nbConstraints);
	side.resize(lp->nbConstraints);


	for(int i(0); i < lp->nbVars + lp->nbConstraints; ++i)
		top[i] = i;
	for(int i(0); i < lp->nbConstraints; ++i)
		side[i] = i + lp->nbVars;

/*
	std::cout << top << std::endl << std::endl;
	std::cout << side << std::endl;
*/
}

Simplex::~Simplex() {
	// TODO Auto-generated destructor stub
}

int Simplex::findPivotColumn() {
	int pivot_col(1);
	float lowest = tab(0, pivot_col);
	for (int j = 1; j < tab.cols(); ++j) {
		if(tab(0, j) < lowest){
			lowest = tab(0, j);
			pivot_col = j;
		}
	}
	if(lowest >= 0)
		return -1;//OPTIMAL
	return pivot_col;
}

int Simplex::findPivotRow(int column) {
	int pivot_row = 0;
	float min_ratio = -1;
	for (int i = 1; i < tab.rows(); ++i) {
		float ratio = tab(i, 0) / tab(i, column);
		if ( (ratio > 0  && ratio < min_ratio ) || min_ratio < 0 ) {
			min_ratio = ratio;
			pivot_row = i;
		}
	}
	if(min_ratio == -1)
		return -1; //no bound
	return pivot_row;
}

void Simplex::pivot(int row, int col) {
	float pivot = tab(row, col);
	for(int i(0); i < tab.cols(); ++i)
		tab(row, i) /= pivot;
	for (int i = 0; i < tab.rows(); ++i) {
		float multip = tab(i, col);
		if(i == row) continue;
		for (int j = 0; j < tab.cols(); ++j) {
			tab(i, j) -= multip * tab(row, j);
		}
	}
	//std::cout << row << " " << col << std::endl;
	side(row-1) = col;
}

Eigen::VectorXf Simplex::run() {
	int piv_col, piv_row;
	//std::cout << std::endl << tab << std::endl;
	int i(0);
	while(1){
		piv_col = findPivotColumn();
		if(piv_col < 0)
		{
			getBest(); // optimal
			return best;
		}
		piv_row = findPivotRow(piv_col);
		if(piv_row < 0)
		{
			break; //caca
		}
		pivot(piv_row, piv_col);
		i++;
	}
	return best;
}

int Simplex::find_basis_variable(int col) {
	int i, xi=-1;
	for(i=1; i < tab.rows(); i++) {
		if (tab(i, col) == 1 ) {
			if (xi == -1)
				xi=i;   // found first '1', save this row number.
			else
				return -1; // found second '1', not an identity matrix.

		} else if (tab(i, col) != 0 ) {
			return -1; // not an identity matrix column.
		}
	}
	return xi;
}

void Simplex::getBest() {
/*	int xi;
	for (int j = 0, i(0); j < tab.cols(); ++j) {
		xi = find_basis_variable(j);
		if(xi != -1)
			best(i++) = tab(xi, 0);
		//else
			//best(j) = 0;
	}
*/
	for(int i(0); i < best.rows(); ++i)
		best(i) = 0;
	for(int i(0); i < side.rows(); ++i){
		//std::cout << side(i) << "  " << tab(i+1, 0) << std::endl;
		if(side(i) <= problem->nbVars){
			best(side(i) - 1) = tab(i+1, 0);
		}
	}


}
