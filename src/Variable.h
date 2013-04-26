#ifndef VARIABLE_H
#define VARIABLE_H
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>


class Variable
{
    public:
        Variable();
        virtual ~Variable();
        void setNom(std::string n);
        void setBounds(std::string b);
        void setBorne(int b);
        std::string getNom() const;
        std::string getBounds();
        int getBorne();
    protected:
    private:
        std::string nom;
        int borne;
        std::string bounds;



};

#endif // VARIABLE_H
