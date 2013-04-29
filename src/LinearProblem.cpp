/*
 * LinearProblem.cpp
 *
 *  Created on: 3 avr. 2013
 *      Author: Arnaud
 */

#include "LinearProblem.h"
#include <algorithm>
#include <vector>

using namespace std;

LinearProblem::LinearProblem()
{
    dual = NULL;
    type = MAX;
    nbConstraints = 0;
    nbVars = 0;

}

LinearProblem::LinearProblem(Probleme * p, LinearProblem::Type t)
{
    std::cout << "Creation du probleme lineaire...";

    type = t;
    dual = NULL;
    nbConstraints = p->getNbConstraints();
    nbVars = p->getNbVars();
    int cpt = 0;
    constraints.resize(nbConstraints+nbVars , nbVars + 1);
for(auto& it : p->getContraintes())
    {
        Eigen::VectorXf v;
        v.resize(nbVars + 1);
        int i(0);
        v(i++) = it.getValeurBorne();
for(auto & varit : p->getVariables())
        {
            auto elem = it.variables.find(varit.getNom());
            if(elem != it.variables.end())
                v(i++) = elem->second;
            else
                v(i++) = 0;
        }
        switch (it.getType().c_str()[0])
        {
        case 'G':
            //v *= -1;
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

    /* AJOUTE DES CONTRAINTES DE TYPE x>= 0 */
    for(int i = 0; i < nbVars; i++)
    {
        Eigen::VectorXf tmp1(nbVars+1);
        int j(0);
        tmp1(j++) = 0;
        for(int k = 0; k < nbVars; k++)
        {
            if(k == i)
                tmp1(j++) = -1;
            else
                tmp1(j++) = 0;
        }
        constraints.row(cpt++) = tmp1;
    }

    objective.resize(nbVars);
    Eigen::VectorXf tmp(nbVars);
    int i(0);
for(auto & varit : p->getVariables())
    {
        auto elem = p->getFctObjective().variables.find(varit.getNom());
        if(elem != p->getFctObjective().variables.end())
        {
            tmp(i++) = elem->second;
        }
        else
            tmp(i++) = 0;
    }
    objective = tmp;
}

LinearProblem LinearProblem::dualize(LinearProblem::Type t)
{
    LinearProblem dual;
    dual.type = t;
    dual.nbConstraints = this->nbVars;
    dual.nbVars = this->nbConstraints; // on ne recupere pas les contraitnes de type x>=0 car les variables vont changer*/
    // nb ligne : nb contraintes + nb contraintes sur les var ( type x >= 0 ) | nb colonnes : nb vars + nb contraintes + 1 pour la partie droite de l'inégalité */
    dual.constraints.resize(dual.nbConstraints,dual.nbVars+1);
    dual.objective = this->constraints.col(0);
    int cpt=0;

    /* On remplit les premières lignes de la matrices avec les nouvelles contraintes*/
    for(int i=0; i< this->objective.rows() ; i++)
    {
        Eigen::VectorXf tmp(dual.nbVars+1);
        tmp(0) =  this->objective(i);
        for (int j=0; j< dual.nbVars ; j++)
        {
            tmp(j+1)=  this->constraints(j,i+1) ;
        }
        dual.constraints.row(cpt) = tmp;
        cpt++;
    }

    /* On remplit les dernière lignes avec les contraintes de type x >= 0*/

    dual.constraints.conservativeResize(dual.nbConstraints+dual.nbVars,dual.nbVars+1);
    for(int k = 0 ; k < dual.nbVars ; k++)
    {
        Eigen::VectorXf tmp1(dual.nbVars+1);
        int j(0);
        tmp1(j++) = 0;
        for(int l = 0 ; l< dual.nbVars ; l++)
        {
            if(dual.constraints(l) == dual.constraints(k))
                tmp1(j++) = 1;
            else
                tmp1(j++) = 0;
        }
        dual.constraints.row(cpt++) = tmp1;
    }
    return dual;
}

LinearProblem::~LinearProblem()
{
    // TODO Auto-generated destructor stub
}

void LinearProblem::switchMode()
{
    type = (MAX ? MIN : MAX);
    objective *= -1;
}

void LinearProblem::minimize()
{
    if(type==MAX) switchMode();
}

void LinearProblem::maximize()
{
    if(type==MIN) switchMode();
}

void LinearProblem::updateSize()
{
    nbConstraints = constraints.rows();
    nbVars = objective.rows();
}

LinearProblem::LinearProblem(string fileName)
{
    std::ifstream input = new std::ifstream(fileName.c_str(), ios_base::in);
    vector<string> rows = new vector<string>();
    vector<char> rowsType = new vector<char>();
    vector<string> cols = new vector<string>();
    std::stringstream buffer;
    std::string str;
    char state = 0; //r for rows, c for columns, h for rhs, b for bounds
    int itmp;
    float ftmp;


    if(!input.good())
    {
        std::cerr << "unable to read file" << std::endl;
        std::exit(-1);
    }

    while(std::getlint(input, buffer))
    {
        buffer >> str;
        if(str.compare("NAME"))
        {
            buffer >> str;
            std::cout << "Reading : " << str << std::endl;
        }
        else if(str.compare("ROWS"))
        {
            state = 'r';
        }
        else if(str.compare("COLUMNS"))
        {
            state = 'c';
        }
        else if(str.compare("RHS"))
        {
            state = 'h';
        }
        else if(str.compare("BOUNDS"))
        {
            state = 'b';
        }
        else if(str.compare("ENDATA"))
        {
            state = 'e';
        }
        else
        {
            switch(state)
            {
            case r:
                buffer >> str;
                rowsType.push_back(str[0]);
                buffer >> str;
                rows.push_back(str);
                // on garde en mémoire els rows et leur type
            case c:
                buffer >> str; // name of the col
                // j'en fais quoi ?
                buffer >> str; // name of the row
                // j'en fais quoi ?
                buffer >> str; // value
                if(str.find('.'))
                	ftmp = stof(str);
               	else
               		itmp = stof(str); // necessary ?

               	// traitement ?
            case h:
            	buffer >> str; // name of the what ?
                // j'en fais quoi ?
                buffer >> str; // name of the row
                // j'en fais quoi ?
                buffer >> str; // value
                if(str.find('.'))
                	ftmp = stof(str);
               	else
               		itmp = stof(str); // necessary ?

               	// traitement ?
            case b:
            	buffer >> str; // UP or DOWN
                // j'en fais quoi ?
                buffer >> str; // always bnd ?
                // j'en fais quoi ?
                buffer >> str; // name of the col
                // j'en fais quoi ?
                buffer >> str; // value
                if(str.find('.'))
                	ftmp = stof(str);
               	else
               		itmp = stof(str); // necessary ?

               	// traitement ?
            default:
                std::cerr << "error :read " << buffer << " after ENDATA" << std::endl;
                break;
            }
        }
    }
}
