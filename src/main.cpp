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
	std::cout << " done" << std::endl;
	LinearProblem lp(&p,LinearProblem::MIN);
	std::cout << " done" << std::endl;

	/*p.parseur("caca.mps");
	std::cout << " done" << std::endl;
	LinearProblem lp(&p,LinearProblem::MIN);
	std::cout << " done" << std::endl;*/

	/*p.parseur("momo.mps");
	std::cout << " done" << std::endl;
	LinearProblem lp(&p,LinearProblem::MAX);
	std::cout << " done" << std::endl;*/

	/*p.parseur("macrophage.mps");
	std::cout << " done" << std::endl;
	LinearProblem lp(&p,LinearProblem::MIN);
	std::cout << " done" << std::endl;*/

	std::ofstream resultat("lp.txt", std::ios::out);
	std::cout << "Ecriture du probleme lineaire dans le fichier lp.txt...";
	resultat << lp.constraints << std::endl;
	resultat <<"Fonction objectif " << std::endl << lp.objective << std::endl;
	resultat.close();
	std::cout << " done" << std::endl;

	BranchAndBound * b = new BranchAndBound(&lp);

	b->run();
	std::cout << " done" << std::endl;
	std::ofstream res("results.txt", std::ios::out);
	std::cout << "Ecriture des resultats dans le ficher results.txt...";
	res << "Best " << std::endl <<  b->getBest() << std::endl;
	res  << "Objective best result " << b->getBestResult() << std::endl;
	std::cout << " done" << std::endl;
	std::cout << "Meilleur resultat " << b->getBestResult() << std::endl;
	delete b;

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



	//Eigen::VectorXf vect(4);
	//vect << 1,2,2.3,4;
	//std::cout << b->getFirstNonIntegerVar(vect) << std::endl;


	return 0;

}

