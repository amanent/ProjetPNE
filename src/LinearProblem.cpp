/*
 * LinearProblem.cpp
 *
 *  Created on: 3 avr. 2013
 *      Author: Arnaud
 */

#include "LinearProblem.h"

LinearProblem::LinearProblem() {
	dual = NULL;
	type = MAX;
	nbConstraints = 0;
	nbVars = 0;

}
LinearProblem::LinearProblem(Probleme * p) {
	type = MIN;
	dual = NULL;
	nbConstraints = p->getNbConstraints();
	nbVars = p->getNbVars();

	constraints.resize(nbConstraints, nbVars);


	for(auto& it : p->getContraintes()){
		Eigen::VectorXf v;
		v.resize(nbVars + 1);
		int i(0);
		v.row(i++) = it.getValeurBorne();
		for(auto& itvar : it.variables){
//			v.row(i++) = itvar.
		}


		switch (it.getType()) {
		case "G":

			break;
		case "L":

			break;
		case "E":

			break;
		default:
			break;
		}
	}


}

LinearProblem::~LinearProblem() {
	// TODO Auto-generated destructor stub
}

void LinearProblem::switchMode() {
	type = (MAX ? MIN : MAX);
	for(int i(0); i < this->objective.rows(); ++i)
		objective(i) = - objective(i);
}

void LinearProblem::minimize() {
	if(type==MAX) switchMode();
}

void LinearProblem::maximize() {
	if(type==MIN) switchMode();
}

void LinearProblem::updateSize() {
	nbConstraints = constraints.rows();
	nbVars = objective.rows();
}
