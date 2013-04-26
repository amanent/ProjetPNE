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
	std::cout << "creation pl" << std::endl;
	type = MIN;
	dual = NULL;
	nbConstraints = p->getNbConstraints();
	nbVars = p->getNbVars();
	int cpt = 0;
	constraints.resize(nbConstraints + nbVars, nbVars + 1);
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
		switch (it.getType().c_str()[0]) {
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


	for(int i = 0; i < nbVars; i++){
		Eigen::VectorXf tmp1(nbVars+1);
		int j(0);
		tmp1(j++) = 0;
		for(int k = 0; k < nbVars; k++){
			if(k == i)
				tmp1(j++) = -1;
			else
				tmp1(j++) = 0;
		}
		constraints.row(cpt++) = tmp1;
		nbConstraints++;
	}



	objective.resize(nbVars);
	Eigen::VectorXf tmp(nbVars);
	int i(0);
	for(auto & varit : p->getVariables()){
		auto elem = p->getFctObjective().variables.find(varit.getNom());
		if(elem != p->getFctObjective().variables.end()){
			tmp(i++) = elem->second;
		}
		else
			tmp(i++) = 0;
	}
	objective = tmp;

}

LinearProblem::~LinearProblem() {
	// TODO Auto-generated destructor stub
}

void LinearProblem::switchMode() {
	type = (MAX ? MIN : MAX);
	objective *= -1;
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
