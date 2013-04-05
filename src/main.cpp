#include "../lib/Eigen/Core"
#include "LinearProblem.h"
#include "Simplex.h"

#include <iostream>

int main(int argc, char **argv) {


	LinearProblem lp;
	Eigen::MatrixXf constraints(3,3);
	constraints << 1.,2.,3.,4.,5.,6.,7.,8.,9.;
	lp.constraints = constraints;
	Eigen::VectorXf objective(2);
	objective << 2,3;
	lp.objective = objective;
	lp.type = LinearProblem::MAX;


	std::cout << constraints << std::endl;
	std::cout << objective << std::endl;

	Simplex ss(&lp);
	std::cout << std::endl << ss.tab << std::endl;


	return 0;

}
