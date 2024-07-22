#include <iostream>
#include "gf256.h"
#include "Calculos.h"  
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std; 

    class Polinomio{
        private: 
            Calculos calc = Calculos();
        public:

    //Calcula la derivada del polinomio 
    vector<short> derivarPolinomio(vector<short> coeficientes);
    //Resta dos polinomios p1 y p2. Al igual que con la suma y la resta de elementos individuales, 
   
    std::vector<short> restarPolinomios(vector<short> polinomio1, vector<short> polinomio2); 

    //Multiplica dos polinomios representados por vectores de coeficientes. 
    
    vector<short> multiplicarPolinomios(vector<short> polinomio1, vector<short> polinomio2);

    //Evalúa un polinomio en un punto utilizando el antilogaritmo
    short evaluarPolinomio(vector<short> coeficientes, short x);

    //Divide el polinomio numerador con el denominador.
    std::pair<std::vector<short>, std::vector<short>> dividirPolinomio(vector<short> numerador, vector<short> denominador);
};
