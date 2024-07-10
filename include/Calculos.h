#include <iostream>
#include "gf256.h" 
#include <fstream>
#include <vector>
#include <string>

using namespace std; 
#ifndef Calc
#define Calc

class Calculos{
    public:
        Calculos(){};

        short suma(int x, int y);

        short resta(int x, int y);     

        short mult(int x, int y);

        short inv(int num);

        short division(int x, int y);
};

#endif