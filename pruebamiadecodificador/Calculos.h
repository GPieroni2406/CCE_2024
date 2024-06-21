#include <iostream>
#include "gf256.h" 
#include <fstream>
#include <vector>
#include <string>

using namespace std; 
#ifndef Calc
#define Calc

class Calculos{
    private:
        int q = 256;
    public:
        Calculos(){};

        short Suma(int x, int y);

        short Resta(int x, int y);     

        short Mult(int x, int y);

        short Inv(int num);

        short Division(int x, int y);

        int Obtener_q();
};

#endif