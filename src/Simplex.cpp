/*
 * Simplex.cpp
 *
 * Created on: 3 avr. 2013
 * Author: Arnaud
 */

#include "Simplex.h"
#include <iostream>

Simplex::Simplex(LinearProblem* lp , bool t) : step(0) {
	// TODO Auto-generated constructor stub
	problem = lp;
	this->isPrimal = t;

	tab.resize(lp->constraints.rows() + 1, lp->constraints.rows()+ lp->constraints.cols());
	lp->minimize();
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
	if (this->isPrimal)
		best.resize(lp->constraints.cols() - 1);
	else
		best.resize(lp->nbConstraints);

	top.resize(lp->nbVars + lp->nbConstraints);
	//side.resize(lp->nbConstraints);
	side.resize(lp->constraints.rows());


	for(int i(0); i < lp->nbVars + lp->nbConstraints; ++i)
		top[i] = i;
	for(int i(0); i < lp->nbConstraints; ++i)
		side[i] = i + lp->nbVars;
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
		/*std::cout << "min ratio " << min_ratio << std::endl;
		std::cout << tab(i,column) << " ";*/
		float ratio = tab(i, 0) / tab(i, column);
		if(isPrimal){
			if ( (tab(i, column) > 0  && ratio < min_ratio ) ||( min_ratio < 0 && tab(i, column) > 0) ) {
				//std::cout << "maj min ratio " << min_ratio << " par ratio " << ratio << std::endl;
				min_ratio = ratio;
				pivot_row = i;
			}
		}else{
			if ( (ratio > 0  && ratio < min_ratio ) ||( min_ratio < 0 && tab(i, column) > 0) ) {
				min_ratio = ratio;
				pivot_row = i;
			}
		}
		//std::cout << "piv row " << pivot_row << std::endl;
	}
	//std::cout << std::endl;
	if(min_ratio == -1)
		return -1; //no bound
	return pivot_row;
}

void Simplex::pivot(int row, int col) {
	float pivot = tab(row, col);
	if(pivot == 0)
		exit(-1);
	for(int i(0); i < tab.cols(); ++i)
		tab(row, i) /= pivot;
	for (int i = 0; i < tab.rows(); ++i) {
		float multip = tab(i, col);
		if(i == row) continue;
		for (int j = 0; j < tab.cols(); ++j) {
			tab(i, j) -= multip * tab(row, j);
		}
	}
	side(row-1) = col;
}

Eigen::VectorXf Simplex::run() {
	int piv_col, piv_row;
	while(1){
		piv_col = findPivotColumn();
		//std::cout << " CHOIX PIVOT " << std::endl;
		//std::cout << " simplexe run piv_col : "<< piv_col << std::endl;
		if(piv_col < 0)
		{
			//std::cout<< " STOP " << std::endl;
			getBest(); // optimal
			return best;
		}
		piv_row = findPivotRow(piv_col);
		//std::cout << " simplexe run piv_row : " << piv_row << std::endl;

		if(piv_row < 0)
		{
			//std::cout << " Pas de solution" << std::endl;
			break; //caca
		}
		//std::cout << " val pivot : " << tab(piv_row,piv_col);
		//std::cout << " " << std::endl;
		pivot(piv_row, piv_col);
		//std::cout << tab << std::endl;
	}
	return best;
}

int Simplex::find_basis_variable(int col) {
	int i, xi=-1;
	for(i=1; i < tab.rows(); i++) {
		if (tab(i, col) == 1 ) {
			if (xi == -1)
				xi=i; // found first '1', save this row number.
			else
				return -1; // found second '1', not an identity matrix.

		} else if (tab(i, col) != 0 ) {
			return -1; // not an identity matrix column.
		}
	}
	return xi;
}

void Simplex::getBest() {
	if (this->isPrimal){
		for(int i(0); i < best.rows(); ++i)
			best(i) = 0;
		for(int i(0); i < side.rows(); ++i){
			if(side(i) <= problem->nbVars){
				best(side(i) - 1) = tab(i+1, 0);
			}
		}
	}
	else
	{
		for(int i(0); i < best.rows(); ++i){
			best(i) = tab(0,this->problem->nbVars+i);
		}
	}
}
