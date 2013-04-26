#include "../lib/Eigen/Core"
#include "LinearProblem.h"
#include "Simplex.h"
#include "branchandbound.hpp"
#include "Probleme.h"
#include "Contrainte.h"
#include "Variable.h"

#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
	/*LinearProblem lp;
	Eigen::MatrixXf constraints(3,3);
	constraints << 16,3,0,27,1,1,10,0,2;
	Eigen::VectorXf objective(2);
	objective << 36,24;
	lp.constraints = constraints;
	lp.nbConstraints = 3;
	lp.nbVars = 2;
	lp.objective = objective;
	lp.type = LinearProblem::MAX;*/

	/*LinearProblem lp;
	Eigen::MatrixXf constraints(3,3);
	constraints << 3,1,2;
	Eigen::VectorXf objective(2);
	objective << 1,2;
	lp.constraints = constraints;
	lp.nbConstraints = 3;
	lp.nbVars = 2;
	lp.objective = objective;
	lp.type = LinearProblem::MAX;*/
	//std::cout << constraints << std::endl;
	/*LinearProblem lp;
	//Eigen::MatrixXf constraints(3,5);
	//constraints << 40,1,1,1,1,10,-2,-1,1,1,10,0,1,0,-1;

	Eigen::MatrixXf constraints(3,3);
	constraints << 18,2,1,42,2,3,24,3,1;
	lp.constraints = constraints;
	lp.nbConstraints = 3;
	lp.nbVars = 2;

	//Eigen::VectorXf objective(4);
	//objective << 0.5,3,1,4;
	Eigen::VectorXf objective(2);
	objective << 3, 2;

	lp.objective = objective;
	lp.type = LinearProblem::MAX;

	 */
	Probleme p;
	p.parseur("iis-bupa-cov.mps");
	//p.parseur("caca.mps");
	//p.parseur("test.mps");
	LinearProblem lp(&p);
	std::ofstream resultat("lp.txt", std::ios::out);

	resultat << lp.constraints << std::endl;
	std::cout <<"Fonction objectif " << std::endl << lp.objective << std::endl;
	resultat.close();


	/*std::cout << constraints << std::endl;
	std::cout << objective << std::endl;
	 */
	/*
	lp.updateSize();

	std::cout << lp.nbConstraints << " contraintes  - " << lp.nbVars << " vars" << std::endl;

	 */
	//Simplex ss(&lp);

	//std::cout << std::endl << ss.tab << std::endl;
	//ss.run();
	//std::cout << std::endl << ss.tab << std::endl;

	//std::cout << std::endl << ss.best << std::endl << std::endl;

	//std::cout << std::endl << ss.side << std::endl;


	BranchAndBound * b = new BranchAndBound(&lp);

	//Eigen::VectorXf vect(4);
	//vect << 1,2,2.3,4;

	b->run();

	std::cout << "Best " << std::endl <<  b->getBest() << std::endl;
	//std::cout << b->getFirstNonIntegerVar(vect) << std::endl;

	delete b;

	return 0;

}

