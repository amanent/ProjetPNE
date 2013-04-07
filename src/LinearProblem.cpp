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
