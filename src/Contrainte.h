#ifndef CONTRAINTE_H
#define CONTRAINTE_H
#include "Variable.h"
#include <vector>
#include <map>

class Contrainte
{
    public:
        Contrainte();
        virtual ~Contrainte();
        void setType(std::string t);
        void setNom(std::string n);
        void setValeurBorne(int n);
        std::string getType();
        std::string getNom();
        int getValeurBorne();
        void ajouter_variable(std::string nom, int coefficient);
        std::map<std::string,int> variables;
    protected:
    private:
        std::string nom;
        std::string type; /*E, N, G, L*/
        int valeur_borne;

};

#endif // CONTRAINTE_H
