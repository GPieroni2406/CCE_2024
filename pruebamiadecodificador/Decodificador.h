#include <iostream> 
#include <string>
#include <fstream>
#include <vector>
#include "Calculos.h"
#include "Polinomio.h"
using namespace std; 

class Decodificador{
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
    vector<short> LeerBloque(ifstream &symbolfile, const int &n);
    vector<short> EncontrarBorraduras(ifstream &erasfile, const int &n);
    vector<short> LeerBloqueSimbolos(ifstream &symbolfile);
    vector<short> LeerIndiceSimbolos(ifstream &erasfile);

    // Métodos de inicialización y gestión de estado
    void IncrementoBloque();
    vector<short> ObtenerPolinomioXR();

    // Métodos de cálculo
    vector<vector<short>> ObtenerMatrizChequeo();
    vector<short> CalcSindromePolinomial(const vector<short> &symbols,const vector<vector<short>> &H);
    vector<short> CalcularPolBorraduras(const vector<short> &indexErasures);
    bool BloqIncorregible(vector<short> indexErasures);
    vector<short> CalcularSindromeModificado(const vector<short> &syndromePolynomial, const vector<short> &erasureLocatorPolynomial);
    pair<vector<short>, vector<short>> Euclides(const vector<short> &poly1,const vector<short> &poly2);
    vector<short> ObtenerPolinomioLocalizador(const vector<short> &erasureLocatorPoly, const vector<short> &calculateErrorLocatorPoly);
    vector<short> RaicesNoNulasChien(const vector<short> &errorLocatorPoly);
    pair<vector<short>, vector<short>> Forneys(const vector<short> &errorLocatorPolyRootsIndexes,const vector<short> &errorLocatorPoly,const vector<short> &errorEvaluatorPolynomial);

    // Método de decodificación
    vector<short> Decodificar(const vector<short> &receivedWord, const vector<short> &errorLocations, const vector<short> &errorValues);
};