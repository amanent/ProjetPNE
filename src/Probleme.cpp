#include "Probleme.h"
#include "Contrainte.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

Probleme::Probleme()
{
	contraintes.reserve(500000);
	variables.reserve(500000);
}

Probleme::~Probleme()
{
    //dtor
}


bool Probleme::parseur(string nom){
	std::cout << "Parsage du fichier "<< nom <<"...";
    string line;
    vector<string> tampon;
    //On ouvre le fichier que l'on doit lire
	ifstream fichier(nom.c_str(), ios_base::in);
	if(fichier){
		while(getline(fichier, line)){
            if(!line.find("NAME")){
                tampon = SegmentselonSymbole(line, " ");
                this->name = tampon[1];
            }
            else if(!line.find("ROWS")){
            	getline(fichier, line);
                while(line.find("COLUMNS")){
                    tampon = SegmentselonSymbole(line, " ");
                    Contrainte c;
                    c.setNom(tampon[2]);
                    if(tampon[1] == "N" || tampon[1] == "RO" || tampon[1] == "OBJ"){
                        c.setType("min");
                        c.setValeurBorne(0);
                        this->fct_objective = c;
                    }else{
                        if(tampon[1]=="E"){
                            c.setType("E");
                        }else if(tampon[1]=="G"){
                            c.setType("G");
                        }else{
                            c.setType("L");
                        }
                        this->ajouter_contrainte(c);
                    }
                    getline(fichier, line);
                }
                getline(fichier, line);
                while(line.find("RHS")){
                        tampon = SegmentselonSymbole(line, " ");
                        Variable v;
                        v.setNom(tampon[1]);
                        ajouter_variable(v);
                        if(this->fct_objective.getNom() == tampon[2]){
                            this->fct_objective.ajouter_variable(tampon[1], stof(tampon[3], NULL));
                        }else
                            this->getContrainte(tampon[2])->ajouter_variable(tampon[1], stof(tampon[3], NULL));

                    getline(fichier, line);

                }
                getline(fichier, line);
                while(line.find("BOUNDS")){
                    tampon = SegmentselonSymbole(line, " ");
                    this->getContrainte(tampon[2])->setValeurBorne(atoi(tampon[3].c_str()));
                    getline(fichier, line);
                }
                getline(fichier, line);
                while(line.find("ENDATA")){

                    tampon = SegmentselonSymbole(line, " ");
                    this->getVariable(tampon[3])->setBorne(atoi(tampon[4].c_str()));
                    if(tampon[1] == "UP"){
                        this->getVariable(tampon[3])->setBounds("<=");
                    }else if(tampon[1] == "LO"){
                        this->getVariable(tampon[3])->setBounds(">=");
                    }else{
                        this->getVariable(tampon[3])->setBounds("=");
                    }
                    getline(fichier, line);
                }
            }
		}
    }
	else{

		cerr<<"Erreur: Impossible d'ouvrir le fichier en lecture"<<endl;
		fichier.close();
		return false;
	}
	fichier.close();

	verification();

	return true;

}

/*Fonction qui découpe str selon symbol*/
vector<string> Probleme::SegmentselonSymbole(string str, string symbole){
	/*On crer un vector de string pour stocker les mots découpés*/
	vector<string> listeString;
	/*permet de trouver la position de symbole dans str*/
	size_t position = str.find(symbole);
	/*Tant que l'on trouve un symbole alors il y a encore une découpe a faire*/
	while((position = str.find(symbole)) != string::npos){
		/*On enregistre la chaine précédent symbole dans la listeString*/
        listeString.push_back(str.substr(0, position));

		/*On met à jour str en supprimant les caractères déja lu, c'est à dire le string avant le symbole et le symbole*/
		str = str.substr(position + symbole.length(), str.length());
		int i = 0;
		while(str[i]==' '){
            str = str.substr(symbole.length(), str.length());
		}

	}

	/*si str n'est pas vide ici,alors c'est qu'il n'y a plu de symbole mais seulement un string qu'il faut rajouter dans la listeString*/
	if(!str.empty())
		listeString.push_back(str);

	return listeString;
}

void Probleme::ajouter_contrainte(Contrainte c){
    this->contraintes.push_back(c);
}

void Probleme::ajouter_variable(Variable v){
    bool b = true;
    for(size_t i = 0; i < this->variables.size() ; i++){
        if(v.getNom() == this->variables[i].getNom())
            b= false;
    }
    if(b == true)
        this->variables.push_back(v);
}

Contrainte* Probleme::getContrainte(string nom){
    for(size_t i = 0; i < this->contraintes.size(); i ++){
        if(this->contraintes[i].getNom() == nom){
            return &contraintes[i];
        }
    }
    return &contraintes[0];
}

Variable* Probleme::getVariable(string nom){
    for(size_t i = 0; i < this->variables.size(); i ++){
        if(this->variables[i].getNom() == nom){
            return &variables[i];
        }
    }
    return &variables[0];
}

void Probleme::verification(){
  ofstream resultat("parseur.txt", ios::out);
  resultat<<"le nom du probleme : "<<this->name<<"\n\n";
  resultat<<"\n\n------- FONCTION OBJECTIVE ---------\n\n";

  resultat<<"nom : "<<this->fct_objective.getNom()<<"\n";
  resultat<<this->fct_objective.getType()<<"\n";
  for(map<string,int>::iterator it = this->fct_objective.variables.begin();it!=this->fct_objective.variables.end(); ++it){
        if(it->second >= 0)
            resultat<<" + ";
        resultat<<it->second<<"*"<<it->first<<" ";
  }


  resultat<<"\n\n------- LES CONTRAINTES ---------\n\n";
  for(size_t i = 0; i<this->contraintes.size();i++){
    resultat<<"nom : "<<this->contraintes[i].getNom()<<"\n";
    for(map<string,int>::iterator it = this->contraintes[i].variables.begin();it!=this->contraintes[i].variables.end(); ++it){
        if(it->second >= 0)
            resultat<<" + ";
        resultat<<it->second<<"*"<<it->first<<" ";
    }
    resultat<<this->contraintes[i].getType()<<" "<<this->contraintes[i].getValeurBorne()<<"\n";
  }

  resultat<<"\n\n------- LES VARIABLES ---------\n\n";
  for(size_t i = 0; i < this->variables.size(); i++){
    resultat<<this->variables[i].getNom()<<" "<<this->variables[i].getBounds()<<" "<<this->variables[i].getBorne()<<"\n";
  }

}
