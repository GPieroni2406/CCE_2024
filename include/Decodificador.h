#include <iostream> 
#include <string>
#include <fstream>
#include <vector>
#include "Calculos.h"
#include "Polinomio.h"
using namespace std; 

class Decodificador {
private: 
    int q, n, r;
    int rho;
    int cantBloquesLeidos;
    vector<short> polinomio_xr;
    Calculos calc = Calculos();
    Polinomio pol = Polinomio();

public:
    // Constructor
    Decodificador(const int &n,const int &q,const int &r);

    // Métodos para lectura de datos
    vector<short> leerBloque(ifstream &symbolfile, const int &n);
    vector<short> encontrarBorraduras(ifstream &erasfile, const int &n);
    vector<short> leerBloqueSimbolos(ifstream &symbolfile);
    vector<short> leerIndiceBorraduras(ifstream &erasfile);

    // Métodos de inicialización y gestión de estado
    void incrementoBloque();
    vector<short> obtenerPolinomioXR();
        // Método para obtener el valor de 'r'
    int obtenerRedundancia() const {
        return this->r;
    }

    // Método para obtener el valor de 'rho'
    int obtenerRho() const {
        return this->rho;
    }

    // Método para obtener el valor de 'rho'
    int obtenerQ() const {
        return this->q;
    }

    // Métodos de cálculo
    vector<vector<short>> obtenerMatrizChequeo();
    vector<short> calcSindromePolinomial(const vector<short> &symbols, const vector<vector<short>> &H);
    vector<short> calcularPolBorraduras(const vector<short> &indexErasures);
    bool bloqIncorregible(vector<short> indexErasures);
    vector<short> calcularSindromeModificado(const vector<short> &syndromePolynomial, const vector<short> &erasureLocatorPolynomial);
    pair<vector<short>, vector<short>> a_e_extendido(const vector<short> &poly2,const vector<short> &poly1);
    vector<short> obtenerPolinomioLocalizador(const vector<short> &erasureLocatorPoly, const vector<short> &calculateErrorLocatorPoly);
    vector<short> raicesNoNulas(const vector<short> &errorLocatorPoly);
    pair<vector<short>, vector<short>> algoritmo_f(const vector<short> &errorEvaluatorPolynomial,const vector<short> &errorLocatorPolyRootsIndexes,const  vector<short> &errorLocatorPoly);

    // Método de decodificación
    vector<short> decodificar(const vector<short> &receivedWord,const vector<short> &errorValues,const vector<short> &errorLocations);
};