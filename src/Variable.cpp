#include "Variable.h"
#include <iostream>
#include <fstream>
#include <climits>

Variable::Variable()
{
    this->borne = INT_MAX;
    this->bounds = ">=";
}

Variable::~Variable()
{
    //dtor
}

void Variable::setNom(std::string n){
    this->nom = n;
}

void Variable::setBounds(std::string b){
    this->bounds = b;
}

void Variable::setBorne(int b){
    this->borne = b;
}

std::string Variable::getNom() const{
    return this->nom;
}

std::string Variable::getBounds() const{
    return this->bounds;
}

int Variable::getBorne() const{
    return this->borne;
}
