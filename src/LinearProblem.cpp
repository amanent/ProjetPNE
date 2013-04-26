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
	int cpt = 0;
	constraints.resize(nbConstraints, nbVars);


	for(auto& it : p->getContraintes()){
		Eigen::VectorXf v;
		v.resize(nbVars + 1);
		int i(0);
		v(i++) = it.getValeurBorne();
		for(auto & varit : p->getVariables()){
			auto elem = it.variables.find(varit.getNom());
			if(elem != it.variables.end())
				v(i++) = elem->second;

			else
				v(i++) = 0;
		}

		switch (it.getType()[0]) {
		case 'G':
			v *= -1;
			break;
		case 'L':

			break;
		case 'E':

			break;
		default:
			break;
		}
		constraints.row(cpt++) = v;
	}

	objective.resize(nbVars);
	Eigen::VectorXf tmp(nbVars);
	int i(0);
	for(auto & varit : p->getVariables()){
		auto elem = p->getFctObjective().variables.find(varit.getNom());
		if(elem != p->getFctObjective().variables.end())
			tmp(i++) = elem->second;

		else
			tmp(i++) = 0;
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
