#include <algorithm>
#include "../include/Decodificador.h"


Decodificador::Decodificador(const int &n, const int &r) {
    // Inicializar variables miembro
    this->n = n;
    this->r = r;
    this->cantBloquesLeidos = 0;
    for (int i = 0; i < this->r; i++){
        this->xr.push_back(0);
    }
    this->xr.push_back(1);
}


vector<short> Decodificador::leerBloque(ifstream &archivo, const int &n) {
    // Calcular la cantidad de bytes a leer
    int simbolos = n; //Cada simbolo es de 1 byte -> n bytes para leer.

    // Calcular el desplazamiento
    int desplazamiento = this->cantBloquesLeidos * n;

    // Mover el puntero de lectura al inicio del bloque deseado
    archivo.seekg(desplazamiento, ios::beg);

    // Leer el bloque de datos
    vector<short> datos(n);
    char symbol;
    for (int i = 0; i < simbolos; ++i) {
        if (archivo.read(&symbol, 1)) {
            std::cout << "Carácter leído: " << symbol << std::endl;
            // Convertir el byte leído a short y almacenarlo
            datos[i] = static_cast<short>(static_cast<unsigned char>(symbol));
        } else {
            // Si no se pudo leer, devolver un vector vacío
            return {};
        }
    }
    //transform(datos.begin(), datos.end(), datos.begin(), [](short value) {
    //    return (value << 8) | ((value >> 8) & 0xFF);
    //});

    return datos;
}

vector<short> Decodificador::encontrarBorraduras(ifstream &archivo, const int &n) {
    vector<short> indices;

    // Calcular la cantidad de bytes a leer
    int bytesALeer = n;

    // Calcular el desplazamiento
    int desplazamiento = this->cantBloquesLeidos * bytesALeer;

    // Mover el puntero de lectura al inicio del bloque deseado
    archivo.seekg(desplazamiento, ios::beg);

    char borradura;

    // Leer los bytes y buscar los índices de símbolos borrados
    for (int i = 0; i < n; ++i) {
        if (archivo.read(&borradura, 1) && borradura == 1) {
            // Si encontramos una borradura, guardamos su posición
            indices.push_back(n-i-1);
        }
    }

    return indices;
}

void Decodificador::incrementoBloque() {
    this->cantBloquesLeidos++;
    printf("Se leyeron %d bloques\n",cantBloquesLeidos);
}

vector<short> Decodificador::obtenerPolinomioXR() {
    return this->xr;
}

vector<short> Decodificador::leerBloqueSimbolos(ifstream &symbolfile) {
    // Llamar a la función leerBloque para leer el bloque de símbolos
    return leerBloque(symbolfile, this->n);
}

vector<short> Decodificador::leerIndiceBorraduras(ifstream &erasfile) {
    // Llamar a la función encontrarBorraduras para obtener los índices de símbolos borrados
    return encontrarBorraduras(erasfile, this->n);
}

vector<vector<short>> Decodificador::obtenerMatrizChequeo() {
    vector<vector<short>> res;
    for (int i = 0; i < this->r; i++){
        vector<short> row;
        for (int j = 0; j < this->n; j++){
            row.push_back(_gfalog[((i+1)*j)%(255)]);
        }
        res.push_back(row);
    }
    return res;
}

vector<short> Decodificador::calcSindromePolinomial(const vector<short> &symbols, const vector<vector<short>> &H) {
    vector<short> sindrome;

    // Calcular el polinomio de síndrome
    for (int i = 0; i < this->r; ++i) {
        short sum = 0;
        for (int j = 0; j < this->n; ++j) {
            sum = calc.suma(sum, calc.mult(symbols[this->n-j-1], H[i][j]));
        }
        sindrome.push_back(sum);
    }

    // Eliminar los coeficientes cero no significativos al final del polinomio
    while (!sindrome.empty() && sindrome.back() == 0) {
        sindrome.pop_back();
    }

    // Devolver el polinomio de síndrome calculado
    return sindrome;
}

vector<short> Decodificador::calcularPolBorraduras(const vector<short> &indicesBorrados) {
    // Polinomio inicializado como 1
    vector<short> polinomioBorraduras(1,1);

    // Multiplicar por cada índice de borrado
    for (short indice : indicesBorrados) {
        short coeficiente = _gfalog[indice];
        vector<short> factor = {1,calc.resta(0, coeficiente)};
        polinomioBorraduras = pol.multiplicarPolinomios(polinomioBorraduras, factor);
    }

    return polinomioBorraduras;
}

bool Decodificador::bloqIncorregible(vector<short> indicesBorrados) {
    // Calcular la cantidad de índices de borrado
    int cantBorrados = (int)indicesBorrados.size();
    int redundancia = this -> r;
    // Actualizar rho con la cantidad de índices de borrado encontrados
    this->rho = cantBorrados;

    // Determinar si el bloque es incorregible basándose en el límite
    bool bloqueIncorregible = (this->rho > redundancia);

    // Devolver el resultado
    return bloqueIncorregible;
}

pair<vector<short>, vector<short>> Decodificador::euclides(const vector<short> &poly1,const vector<short> &poly2) {
    double stop = (this->r + this->rho) / 2.0;
    vector<short> r0 = poly2;
    vector<short> r1 = poly1;
    vector<short> t0 = {0};
    vector<short> t1 = {1};
    while (!(r1.size() - 1 < stop && stop <= r0.size() - 1)) {
        pair <vector<short>, vector<short>> res = pol.dividirPolinomio(r0, r1);
        vector<short> quotient = res.first;
        r0 = r1;
        r1 = res.second;

        vector<short> temp = t0;
        t0 = t1;
        t1 = pol.restarPolinomios(temp, pol.multiplicarPolinomios(quotient, t1));
    }
    return make_pair(t1,r1);
}

pair<vector<short>, vector<short>> Decodificador::forneys(const vector<short> &errorLocatorPolyRootsIndexes,const  vector<short> &errorLocatorPoly,const vector<short> &errorEvaluatorPolynomial){
    vector<short> errorValues;
    vector<short> errorLocations;
    vector<short> derivative = pol.derivarPolinomio(errorLocatorPoly);

    if (errorLocatorPolyRootsIndexes.empty()){
        return make_pair(errorValues, errorLocations);
    }
    
    for (short i : errorLocatorPolyRootsIndexes){
        errorValues.push_back(calc.mult(255, calc.division(pol.evaluarPolinomio(errorEvaluatorPolynomial, i), pol.evaluarPolinomio(derivative, i))));
        errorLocations.push_back((255 - i) % 255);
    }
    
    return make_pair(errorValues, errorLocations);
}

vector<short> Decodificador::decodificar(const vector<short> &palabraRecibida, const vector<short> &ubicacionesErrores, const vector<short> &valoresErrores) {
    int j=0;
    vector<short> decodedWord = palabraRecibida;
    for (auto i: ubicacionesErrores){
        if (i < (int)palabraRecibida.size()){
            decodedWord[this->n - i - 1] = calc.resta(palabraRecibida[this->n - i - 1], valoresErrores[j]);
        }
        j++;
    }

    return decodedWord;
}

vector<short> Decodificador::obtenerPolinomioLocalizador(const vector<short> &polinomioBorrados, const vector<short> &polinomioErrores) {
    // Multiplicar los polinomios localizadores de borrados y errores
    return pol.multiplicarPolinomios(polinomioBorrados, polinomioErrores);
}

vector<short> Decodificador::raicesNoNulas(const vector<short> &polinomio) {
    vector<short> index = {};        
    int amountRoots = 0;    
    int degree = (int)polinomio.size() - 1;  

     if (!polinomio.empty()){
         for (int i=1; i < 256 && amountRoots < degree; i++){
            if (pol.evaluarPolinomio(polinomio, i) == 0){
                printf("raiz encontrada en rootIndexes %d\n",i);
                index.push_back(i);
                amountRoots++;
            }
        }
        }
    if (amountRoots == degree){
        return index;
    }
    else {
        printf("Se encontraron menos raices que el grado del polinomio.\n");
        return {};
    }    
}

vector<short> Decodificador::calcularSindromeModificado(const vector<short> &polinomioSindrome, const vector<short> &indicesBorrados) {
    // Realizar la multiplicación de polinomios entre síndrome y localizador de borrado
    vector<short> producto = pol.multiplicarPolinomios(polinomioSindrome, indicesBorrados);

    // Obtener el residuo de la división del polinomio resultante por el polinomio generador xr
    vector<short> residuodivision = pol.dividirPolinomio(producto, this->xr).second;

    // Devolver el residuo calculado
    return residuodivision;
}