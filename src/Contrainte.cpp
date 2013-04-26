#include "Contrainte.h"
#include "Variable.h"
#include <iostream>
#include <fstream>
using namespace std;

Contrainte::Contrainte()
{
    //ctor
}

Contrainte::~Contrainte()
{
    //dtor
}

void Contrainte::setValeurBorne(int n){
    this->valeur_borne = n;
}

void Contrainte::setType(std::string t){
    this->type = t;
}

void Contrainte::setNom(std::string n){
    this->nom = n;
}

std::string Contrainte::getType() const{
    return this->type;
}

std::string Contrainte::getNom() const{
    return this->nom;
}

int Contrainte::getValeurBorne() const{
    return this->valeur_borne;
}

void Contrainte::ajouter_variable(std::string nom, int coefficient){
    this->variables[nom] = coefficient;
}
