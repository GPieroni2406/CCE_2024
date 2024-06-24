#include <iostream> 
#include <string>
#include <fstream>
#include <vector>
#include "Calculos.h"
#include "Polinomio.h"
using namespace std; 

class Decodificador {
private: 
    int n, r;
    int rho;
    int alpha = 3;
    int cantBloquesLeidos;
    vector<short> xr;
    Calculos calc = Calculos();
    Polinomio pol = Polinomio();

public:
    // Constructor
    Decodificador(const int &n, const int &r);

    // Métodos para lectura de datos
    vector<short> leerBloque(ifstream &symbolfile, const int &n);
    vector<short> encontrarBorraduras(ifstream &erasfile, const int &n);
    vector<short> leerBloqueSimbolos(ifstream &symbolfile);
    vector<short> leerIndiceSimbolos(ifstream &erasfile);

    // Métodos de inicialización y gestión de estado
    void incrementoBloque();
    vector<short> obtenerPolinomioXR();

    // Métodos de cálculo
    vector<vector<short>> obtenerMatrizChequeo();
    vector<short> calcSindromePolinomial(const vector<short> &symbols, const vector<vector<short>> &H);
    vector<short> calcularPolBorraduras(const vector<short> &indexErasures);
    bool bloqIncorregible(vector<short> indexErasures);
    vector<short> calcularSindromeModificado(const vector<short> &syndromePolynomial, const vector<short> &erasureLocatorPolynomial);
    pair<vector<short>, vector<short>> euclides(const vector<short> &poly1, const vector<short> &poly2);
    vector<short> obtenerPolinomioLocalizador(const vector<short> &erasureLocatorPoly, const vector<short> &calculateErrorLocatorPoly);
    vector<short> raicesNoNulas(const vector<short> &errorLocatorPoly);
    pair<vector<short>, vector<short>> forneys(const vector<short> &errorLocatorPolyRootsIndexes, const vector<short> &errorLocatorPoly, const vector<short> &errorEvaluatorPolynomial);

    // Método de decodificación
    vector<short> decodificar(const vector<short> &receivedWord, const vector<short> &errorLocations, const vector<short> &errorValues);
};