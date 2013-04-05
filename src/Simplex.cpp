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


	tab.resize(lp->constraints.rows() + 1, lp->constraints.rows()+ lp->constraints.cols());

	tab(0,0) = 0;

	for(int i(1); i < lp->constraints.cols(); ++i){
		tab(0, i) = lp->objective(i - 1);
	}

	std::cout << "lolol" << std::endl;
	for(int i(0); i < lp->constraints.rows(); ++i){
		for(int j(0); j < lp->constraints.cols(); ++j)
			tab(i+1, j) = lp->constraints(i, j);

	}

	for(int i=1; i<tab.rows(); i++) {
	    for(int j=1; j<tab.rows(); j++)
	      tab(i, j + lp->constraints.cols()-1) = (i==j);
	  }
}

Simplex::~Simplex() {
	// TODO Auto-generated destructor stub
}

