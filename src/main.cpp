#include "../lib/Eigen/Core"
#include "LinearProblem.h"
#include "Simplex.h"

#include <iostream>

int main(int argc, char **argv) {

	std::cout << "Beginning" << std::endl;
	LinearProblem lp;
//	Eigen::MatrixXf constraints(3,5);
//	constraints << 40,1,1,1,1,10,-2,-1,1,1,10,0,1,0,-1;

	Eigen::MatrixXf constraints(3,3);
	constraints << 18,2,1,42,2,3,24,3,1;
	lp.constraints = constraints;

//	Eigen::VectorXf objective(4);
//	objective << 0.5,3,1,4;
	Eigen::VectorXf objective(2);
	objective << 3, 2;

	lp.objective = objective;
	lp.type = LinearProblem::MAX;


	std::cout << constraints << std::endl;
	std::cout << objective << std::endl;

	lp.updateSize();

	std::cout << lp.nbConstraints << " contraintes  - " << lp.nbVars << " vars" << std::endl;

	Simplex ss(&lp);

	std::cout << std::endl << ss.tab << std::endl;
	ss.run();
	std::cout << std::endl << ss.tab << std::endl;

	std::cout << std::endl << ss.best << std::endl << std::endl;

	std::cout << std::endl << ss.side << std::endl;


	return 0;

}

