#include <iostream>
#include "Polinomio.h"
#include <vector>

using namespace std;

int main() {
    Polinomio p;

    vector<short> poly1 = {3, 0, 2}; // Representa 3 + 2x^2
    vector<short> poly2 = {1, 1};    // Representa 1 + x

    // Probar la multiplicación de polinomios
    vector<short> resultadoMultiplicacion = p.Multiplicar_Polinomios(poly1, poly2);
    cout << "Multiplicación de polinomios: ";
    for (short coef : resultadoMultiplicacion) {
        cout << coef << " ";
    }
    cout << endl;

    // Probar la resta de polinomios
    vector<short> resultadoResta = p.Restar_Polinomios(poly1, poly2);
    cout << "Resta de polinomios: ";
    for (short coef : resultadoResta) {
        cout << coef << " ";
    }
    cout << endl;

    // Probar la derivada de un polinomio
    vector<short> resultadoDerivada = p.Derivar_Polinomio(poly1);
    cout << "Derivada de polinomio: ";
    for (short coef : resultadoDerivada) {
        cout << coef << " ";
    }
    cout << endl;

    // Probar la evaluación de un polinomio en un punto
    short resultadoEvaluacion = p.Evaluar_Polinomio(poly1, 2); // Evaluar en x = 2
    cout << "Evaluación de polinomio en x=2: " << resultadoEvaluacion << endl;

    // Probar el cálculo de cociente y residuo de la división de polinomios
    vector<short> cociente = p.CalcularCociente(poly1, poly2);
    vector<short> residuo = p.CalcularResiduo(poly1, poly2);
    cout << "Cociente de la división de polinomios: ";
    for (short coef : cociente) {
        cout << coef << " ";
    }
    cout << endl;
    cout << "Residuo de la división de polinomios: ";
    for (short coef : residuo) {
        cout << coef << " ";
    }
    cout << endl;

    return 0;
}
