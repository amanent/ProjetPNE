#ifndef PROBLEME_H
#define PROBLEME_H
#include "Contrainte.h"

class Probleme
{
    public:
        Probleme();
        virtual ~Probleme();
        bool parseur(std::string name);
        void ajouter_contrainte(Contrainte c);
        void ajouter_variable(Variable v);
        std::vector<std::string> SegmentselonSymbole(std::string str, std::string symbole);
        void verification();
        Contrainte* getContrainte(std::string nom);
        Variable* getVariable(std::string nom);
    protected:
    private:
        std::string name;
        Contrainte fct_objective;
        std::vector<Contrainte> contraintes;
        std::vector<Variable> variables;
};

#endif // PROBLEME_H
