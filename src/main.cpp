#include "./lib/Eigen/Core"
#include "LinearProblem.h"
#include "Simplex.h"
#include "branchandbound.hpp"
#include "Probleme.h"
#include "Contrainte.h"
#include "Variable.h"

#include <iostream>
#include <fstream>

int main(int argc, char **argv) {

	/************************** PROBLEME DE BRASSEUR ( mode  ARNAUD )  ************************/
	/* PROBLEM : MAX 15x + 25x2
	 * 			x1 + 3x2 <= 96
	 * 			x1 + x2 <=40
	 * 			7x1 + 4x2 <=238
	 * 			x >= 0
	 * 			y >= 0
	 * RESULTAT ATTENDU x = 12 , y = 28 | MAX = 880 ( Resultat sur à 100% )
	 */


	/*std::cout << " PROBLEME DU BRASSEUR (max) " << std::endl;
	LinearProblem lp;
	Eigen::MatrixXf constraints(3,3);
	constraints << 96,1,3, 40,1,1, 238,7,4;
	lp.constraints = constraints;
	Eigen::VectorXf objective(2);
	objective << 3,5;

	lp.constraints = constraints;
	lp.objective = objective;
	lp.nbConstraints = 3;
	lp.nbVars = 2;

	Simplex ss(&lp,true);
	std::cout << "tableau de départ" << std::endl;
	std::cout<< ss.tab << std::endl;
	ss.run();
	std::cout << " vecteur resultat : " <<std::endl;
	std::cout << ss.best << std::endl;*/

	/**************************** PROBLEME avec parseur flo (fichier caca.mps )*************************/
	/* PROBLEM : MIN -2x -3x2
	 * 			5x + 4x2 <= 32
	 * 			x + 2x2 <=10
	 * 			x >= 0
	 * 			y >= 0
	 * RESULTAT ATTENDU x = 4 , y = 3 | MIN = -17 ( Resultat sur à 100% )
	 */

	/*std::cout << " PROBLEME MIN SANS DUALIZATION " << std::endl;
	Probleme p;
	p.parseur("caca.mps");
	std::cout << " done" << std::endl;
	LinearProblem lp(&p,LinearProblem::MIN);
	std::cout << " done" << std::endl;

	Simplex ss(&lp,true);
	std::cout << "tableau de départ" << std::endl;
	std::cout<< ss.tab << std::endl;
	ss.run();
	std::cout << " vecteur resultat : " <<std::endl;
	std::cout << std::endl << ss.best << std::endl << std::endl;*/


	/****************************** DUALIZATION SUR PROBLEM TYPE min x+y ( fichier momo.mps )**********************/


	/*std::cout << " PROBLEME MIN AVEC DUALIZATION " << std::endl;
	Probleme p;
	p.parseur("momo.mps");
	std::cout << " done" << std::endl;
	LinearProblem lp(&p,LinearProblem::MIN);
	std::cout << " done" << std::endl;

	Simplex s1(&lp ,true);
	std::cout << " Tableau initial " <<std::endl;
	std::cout<< s1.tab << std::endl;
	std::cout<< " Tableau dual" <<std::endl;

	LinearProblem dualize = lp.dualize(LinearProblem::MAX);

	Simplex ss(&dualize,false);
	std::cout<< ss.tab << std::endl;
	ss.run();
	std::cout << " vecteur resultat : " <<std::endl;
	std::cout << std::endl << ss.best << std::endl << std::endl;*/


	/********************************************* BRANCH AND BOUND ***************************************************/
	/*BranchAndBound * b = new BranchAndBound(&lp);

	b->run();
	std::cout << " done" << std::endl;
	std::ofstream res("results.txt", std::ios::out);
	std::cout << "Ecriture des resultats dans le ficher results.txt...";
	res << "Best " << std::endl <<  b->getBest();
	res  << "Objective best result " << b->getBestResult() << std::endl;
	std::cout << " done" << std::endl;
	std::cout << "Meilleur resultat " << b->getBestResult() << std::endl;
	delete b;*/

	/*std::cout << constraints << std::endl;
	std::cout << objective << std::endl;
	 */
	/*
	lp.updateSize();

	std::cout << lp.nbConstraints << " contraintes  - " << lp.nbVars << " vars" << std::endl;

	 */
	//

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

