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
	LinearProblem lp(&p,LinearProblem::MIN,true);
	std::cout << " done" << std::endl;

	Simplex ss(&lp,true);
	std::cout << "tableau de départ" << std::endl;
	std::cout<< ss.tab << std::endl;
	ss.run();
	std::cout << " vecteur resultat : " <<std::endl;
	std::cout << std::endl << ss.best << std::endl << std::endl;*/


	/**************************** PROBLEME AVEC B&B (fichier test2.mps )*************************/
	/* PROBLEM : MIN -1x -2x2
	 * 			2x + 4x2 <= 3
	 * 			x >= 0
	 * 			x2 >= 0
	 * RESULTAT ATTENDU x = 1 , x2 = 0 | MIN = -1 ( Resultat sur à 100% )
	 */
	/*std::cout << " PROBLEME MIN SANS DUALIZATION " << std::endl;
	Probleme p;
	p.parseur("test2.mps");
	std::cout << " done" << std::endl;
	LinearProblem lp(&p,LinearProblem::MIN,true);
	std::cout << " done" << std::endl;
	BranchAndBound * b = new BranchAndBound(&lp);*/

	std::cout << " PROBLEME MIN SANS DUALIZATION " << std::endl;
	Probleme p;
	p.parseur("iis-bupa-cov.mps");
	std::cout << " done" << std::endl;
	LinearProblem lp(&p,LinearProblem::MIN,false);
	std::cout << " done" << std::endl;
	LinearProblem dual = lp.dualize(LinearProblem::MAX);

	Simplex s1(&lp ,true);
		std::cout << " Tableau initial " <<std::endl;
		std::cout<< s1.tab << std::endl;
		std::cout<< " Tableau dual" <<std::endl;

	BranchAndBound * b = new BranchAndBound(&dual);

	/*Simplex ss(&lp,true);
	std::cout << "tableau de départ" << std::endl;
	std::cout<< ss.tab << std::endl;
	ss.run();
	std::cout << " vecteur resultat : " <<std::endl;
	std::cout << std::endl << ss.best << std::endl << std::endl;*/


	/****************************** DUALIZATION SUR PROBLEM TYPE min x+y ( fichier momo.mps )**********************/

	/* PROBLEM : MIN 21x + 14y
	 *    2x + 3y >=12
	 *    3x + y >= 6
	 *    x + 3y >= 9
	 *    y >= 0
	 *    x >= 0
	 * RESULTAT ATTENDU x = 6/7 , y = 24/7 | MIN = 66 ( Resultat sur à 100% )
	 */

	/*std::cout << " PROBLEME MIN AVEC DUALIZATION " << std::endl;
	Probleme p;
	p.parseur("momo.mps");
	std::cout << " done" << std::endl;
	LinearProblem lp(&p,LinearProblem::MIN,false);
	std::cout << " done" << std::endl;


	LinearProblem dualize = lp.dualize(LinearProblem::MAX);
	/*BranchAndBound * b = new BranchAndBound(&dualize);*/


	/*Simplex s1(&lp ,true);
	std::cout << " Tableau initial " <<std::endl;
	std::cout<< s1.tab << std::endl;
	Simplex ss(&dualize,false);
	std::cout<< " Tableau dual" <<std::endl;
	std::cout<< ss.tab << std::endl;
	ss.run();
	std::cout << " vecteur resultat : " <<std::endl;
	std::cout << std::endl << ss.best << std::endl << std::endl;*/


	/********************************************* BRANCH AND BOUND ***************************************************/
	//BranchAndBound * b = new BranchAndBound(&lp);

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

