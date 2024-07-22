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
    int borrados;
    int cantBloquesLeidos;
    vector<short> polinomio_xr;
    Calculos calc = Calculos();
    Polinomio pol = Polinomio();

public:
    // Constructor
    Decodificador(const int &n,const int &q,const int &r);

    // Métodos para lectura de datos
    vector<short> leerBloque(ifstream &archivo);
    vector<short> encontrarBorraduras(ifstream &archivo);
    vector<short> leerBloqueSimbolos(ifstream &archivo);
    vector<short> leerIndiceBorraduras(ifstream &archivo);

    // Métodos de inicialización y gestión de estado
    void incrementoBloque();
    vector<short> obtenerPolinomioXR();
    int obtenerRedundancia() const {
        return this->r;
    }


    int obtenerBorrados() const {
        return this->borrados;
    }

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