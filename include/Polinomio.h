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
    //la resta de polinomios en un campo finito es equivalente a su suma (operación XOR bit a bit de los coeficientes correspondientes).
    std::vector<short> restarPolinomios(vector<short> polinomio1, vector<short> polinomio2); 

    //Multiplica dos polinomios representados por vectores de coeficientes. 
    //La lógica es similar a la multiplicación de polinomios en matemáticas: 
    //cada término de p1 se multiplica por cada término de p2 y se suman los resultados.
    vector<short> multiplicarPolinomios(vector<short> polinomio1, vector<short> polinomio2);

    //Evalúa un polinomio en un punto utilizando el antilogaritmo (brindado por los profesores) del valor de i multiplicado por su índice,
    //lo que facilita la evaluación de polinomios en campos finitos.
    short evaluarPolinomio(vector<short> coeficientes, short x);

    //Divide el polinomio numerador con el denominador.
    std::pair<std::vector<short>, std::vector<short>> dividirPolinomio(vector<short> numerador, vector<short> denominador);
};
