//* ce qu'il faut : 

		recupérer un PL, sous forme de matrice.
		(transformer le max en min)
		faire une relaxation (passer de variables discrètes a variables continues)
		fixer UB a l'infini (+ ou - je ne sais plus)
boucle:	evaluer le resultat avec la méthode du simplexe + fixer LB/UB (selon quels critères ?)
		-si les valeurs sont toutes entières : OK
		-sinon, on fixe une variable (ce qui créée deux branches) et bien sur s'il reste des variables a fixer
		retour boucle
*/