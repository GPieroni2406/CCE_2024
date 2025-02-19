#include <iostream>
#include "gf256.h"
#include "Calculos.h"  
#include <fstream>
#include <vector>
#include <string>
using namespace std; 

class Polinomio{
    private: 
        Calculos calc = Calculos();
    public:

//Calcula la derivada formal de un polinomio en un campo finito, 
//que se utiliza en algunos algoritmos de decodificación, como el Algoritmo de Forney para la corrección de errores.
vector<short> Derivar_Polinomio(vector<short> poly);


//Resta dos polinomios p1 y p2. Al igual que con la suma y la resta de elementos individuales, 
//la resta de polinomios en un campo finito es equivalente a su suma (operación XOR bit a bit de los coeficientes correspondientes).
std::vector<short> Restar_Polinomios(vector<short> p1, vector<short> p2); 

//Multiplica dos polinomios representados por vectores de coeficientes. 
//La lógica es similar a la multiplicación de polinomios en matemáticas: 
//cada término de p1 se multiplica por cada término de p2 y se suman los resultados apropiados.
vector<short> Multiplicar_Polinomios(vector<short> p1, vector<short> p2);

//Evalúa un polinomio en un punto utilizando el antilogaritmo del valor de i multiplicado por su índice,
//lo que facilita la evaluación de polinomios en campos finitos.
short Evaluar_Polinomio(vector<short> poly, short i);

//Divide un polinomio numerator por otro polinomio denominator utilizando la división larga de polinomios.
// Devuelve un par que contiene el cociente y el residuo de la división.
std::pair<std::vector<short>, std::vector<short>> Dividir_Polinomio(vector<short> numerator, vector<short> denominator);
};
