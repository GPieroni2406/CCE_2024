#include <iostream> 
#include <string>
#include <fstream>
#include <vector>
#include "Calculos.h"
#include "Polinomio.h"
using namespace std; 

class Decoder{
    private: 
        int n, r;
        int rho;
        int alpha = 3;
        int cantBloquesLeidos;
        vector<short> xr;
        Calculos calc = Calculos();
        Polinomio pol = Polinomio();
        //Lee un bloque de datos del archivo binario. La función busca en el archivo usando un desplazamiento basado en cuántos bloques se han leído previamente 
        //y lee 2*n bytes (cada símbolo representa 2 bytes en este contexto). 
        //Luego intercambia los bytes de cada símbolo (big-endian a little-endian o viceversa) y devuelve un vector con los símbolos leídos.
        vector<short> LeerBloque(ifstream & symbolfile, const int &n);
        //Esta función lee el archivo de indicadores de borraduras y devuelve un vector que contiene los índices de los símbolos borrados 
        //dentro de un bloque de datos.
        vector<short> EncontrarBorraduras(ifstream & erasfile, const int &n);

    public:
        //El constructor de la clase Decoder inicializa las 
        //variables para manejar un código RS con longitud n y redundancia r. También inicializa el contador de bloques leídos y 
        //el polinomio xr que se usa en cálculos posteriores.
        Decoder(const int &n, const int &r);

        //Incrementa el contador de bloques leídos.
        void IncrementoBloque();

        //Devuelve el polinomio xr.
        vector<short> ObtenerPolinomioXR();

        //Lee un bloque de datos del archivo de símbolos y devuelve los símbolos leídos.
        vector<short> LeerBloqueSimbolos(ifstream & symbolfile);

        //Lee los índices de borrado del archivo de borraduras.
        vector<short> LeerIndiceSimbolos(ifstream & erasfile);

        //Genera y devuelve la matriz de control de paridad para el código Reed-Solomon basado en las tablas de logaritmos y antilogaritmos.
        vector<vector<short>> ObtenerMatrizChequeo();

        // POLY[0] is the independent term //
        //Calcula el polinomio de síndrome a partir de los símbolos recibidos y la matriz de control de paridad.
        vector<short> CalcSindromePolinomial(const vector<short> &symbols,const vector<vector<short>> &H);

        //Calcula el polinomio localizador de borraduras utilizando los índices de borrado.
        vector<short> CalcularPolBorraduras(const vector<short> &indexErasures);

        //Determina si un bloque con un cierto número de borraduras es incorregible basándose en la redundancia r.
        bool BloqIncorregible(vector<short> indexErasures);

        // /Calcula el polinomio de síndrome modificado multiplicando el polinomio de síndrome
        // y el polinomio localizador de borrado y luego dividiéndolo por xr
        vector<short> CalcularSindromeModificado(const vector<short> &syndromePolynomial, const vector<short> &erasureLocatorPolynomial);

        // /Implementa el algoritmo euclidiano extendido para encontrar el máximo común divisor de dos polinomios y un par de polinomios (s, t) 
        //tales que s * poly1 + t * poly2 = gcd(poly1, poly2).
        pair<vector<short>, vector<short>> Euclides(const vector<short> &poly1,const vector<short> &poly2);

        //Multiplica el polinomio localizador de borraduras y 
        //el polinomio localizador de errores para obtener el polinomio localizador de errores recalcado.
        vector<short> ObtenerPolinomioLocalizador(const vector<short> &erasureLocatorPoly, const vector<short> &calculateErrorLocatorPoly);

        //Busca las raíces no cero del polinomio dado utilizando la búsqueda de Chien.
        vector<short> RaicesNoNulasChien(const vector<short> &errorLocatorPoly);
        
        //Implementa el algoritmo de Forney para calcular los valores y las ubicaciones de los errores utilizando las raíces del polinomio localizador de errores, 
        //el propio polinomio y el polinomio evaluador de errores.
        pair<vector<short>, vector<short>> Forneys(const vector<short> &errorLocatorPolyRootsIndexes,const vector<short> &errorLocatorPoly,const vector<short> &errorEvaluatorPolynomial);

        //Decodifica la palabra recibida corrigiendo los errores en las ubicaciones especificadas utilizando los valores de error dados.
        vector<short> Decodificar(const vector<short> &receivedWord, const vector<short> &errorLocations, const vector<short> &errorValues);


};