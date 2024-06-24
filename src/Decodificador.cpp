#include <algorithm>
#include "../include/Decodificador.h"


Decodificador::Decodificador(const int &n, const int &r) {
    // Inicializar variables miembro
    this->n = n;
    this->r = r;
    this->cantBloquesLeidos = 0;

    // Inicializar vector xr
    this->xr.resize(r + 1); // El tamaño del vector xr es r + 1
    for (int i = 0; i <= r; ++i) {
        this->xr[i] = (i == r) ? 1 : 0;
    }
}

vector<short> Decodificador::leerBloque(ifstream &archivo, const int &n) {
    // Calcular la cantidad de bytes a leer
    int bytesALeer = n; // Cada símbolo representa 1 byte

    // Calcular el desplazamiento
    int desplazamiento = this->cantBloquesLeidos * bytesALeer;

    // Mover el puntero de lectura al inicio del bloque deseado
    archivo.seekg(desplazamiento, ios::beg);

    // Leer el bloque de datos
    vector<short> datos(n);
    archivo.read(reinterpret_cast<char*>(datos.data()), bytesALeer);

    // Verificar si se leyeron todos los bytes esperados
    if (archivo.gcount() < static_cast<streamsize>(bytesALeer)) {
        return {}; // Devolver vector vacío si no se leyeron todos los datos
    }

    // Incrementar el contador de bloques leídos
    incrementoBloque();

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

    // Leer los bytes y buscar los índices de símbolos borrados
    for (int i = 0; i < n; ++i) {
        char byte;
        archivo.read(&byte, 1);
        if (byte == 1) {
            indices.push_back(n - i - 1);
        }
    }

    return indices;
}

void Decodificador::incrementoBloque() {
    // Incrementar el contador de bloques leídos
    ++this->cantBloquesLeidos;
}

vector<short> Decodificador::obtenerPolinomioXR() {
    // Devolver una copia del vector xr
    vector<short> copiaXR(this->xr.begin(), this->xr.end());
    return copiaXR;
}

vector<short> Decodificador::leerBloqueSimbolos(ifstream &symbolfile) {
    // Llamar a la función leerBloque para leer el bloque de símbolos
    return leerBloque(symbolfile, this->n);
}

vector<short> Decodificador::leerIndiceSimbolos(ifstream &erasfile) {
    // Llamar a la función encontrarBorraduras para obtener los índices de símbolos borrados
    return encontrarBorraduras(erasfile, this->n);
}

vector<vector<short>> Decodificador::obtenerMatrizChequeo() {
    // Crear la matriz de control de paridad
    vector<vector<short>> matrizDeParidad(this->r, vector<short>(this->n));

    // Llenar la matriz con valores del campo galoisiano
    for (int i = 0; i < this->r; ++i) {
        for (int j = 0; j < this->n; ++j) {
            // Calcular el valor en la posición (i, j)
            matrizDeParidad[i][j] = _gfalog[((i + 1) * j) % 255];
        }
    }

    // Retornar la matriz de control de paridad calculada
    return matrizDeParidad;
}

vector<short> Decodificador::calcSindromePolinomial(const vector<short> &symbols, const vector<vector<short>> &H) {
    vector<short> sindrome(this->r, 0);

    // Calcular el polinomio de síndrome
    for (int i = 0; i < this->r; ++i) {
        for (int j = 0; j < this->n; ++j) {
            sindrome[i] = calc.suma(sindrome[i], calc.mult(symbols[j], H[i][j]));
        }
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
    vector<short> polinomioBorraduras = {1};

    // Multiplicar por cada índice de borrado
    for (short indice : indicesBorrados) {
        // Coeficiente α^indice en el campo galoisiano
        short coeficiente = _gfalog[indice];

        // Polinomio (1 - α^indice)
        vector<short> factor = {calc.resta(0, coeficiente), 1};

        // Multiplicación de polinomios
        polinomioBorraduras = pol.multiplicarPolinomios(polinomioBorraduras, factor);
    }

    return polinomioBorraduras;
}

bool Decodificador::bloqIncorregible(vector<short> indicesBorrados) {
    // Calcular la cantidad de índices de borrado
    int cantBorrados = static_cast<int>(indicesBorrados.size());
    
    // Calcular el límite para determinar si el bloque es incorregible
    int limiteIncorregible = (this->r + this->rho) / 2;

    // Actualizar rho con la cantidad de índices de borrado encontrados
    this->rho = cantBorrados;

    // Determinar si el bloque es incorregible basándose en el límite
    bool bloqueIncorregible = (this->rho > limiteIncorregible);

    // Devolver el resultado
    return bloqueIncorregible;
}

pair<vector<short>, vector<short>> Decodificador::euclides(const vector<short> &primerPolinomio, const vector<short> &segundoPolinomio) {
    // Calcular el límite según la fórmula dada
    double limite = (this->r + this->rho) / 2.0;

    // Inicializar variables
    vector<short> s_anterior = {0};   // s_anterior = 0
    vector<short> r_anterior = segundoPolinomio;  // r_anterior = segundoPolinomio
    vector<short> s_actual = {1};    // s_actual = 1
    vector<short> r_actual = primerPolinomio;    // r_actual = primerPolinomio

    // Algoritmo extendido de Euclides
    while (!(r_actual.size() - 1 < limite && limite <= r_anterior.size() - 1)) {
        // Calcular cociente y residuo de r_anterior entre r_actual
        vector<short> cociente = pol.calcularCociente(r_anterior, r_actual);
        vector<short> nuevo_r_anterior = r_actual;
        r_actual = pol.calcularResiduo(r_anterior, r_actual);
        r_anterior = nuevo_r_anterior;

        // Actualizar s_anterior y s_actual
        vector<short> temp_s = s_anterior;
        s_anterior = s_actual;
        s_actual = pol.restarPolinomios(temp_s, pol.multiplicarPolinomios(cociente, s_actual));
    }

    // Retornar el resultado
    return make_pair(s_actual, r_actual);
}

pair<vector<short>, vector<short>> Decodificador::forneys(const vector<short> &raicesPolinomioLocalizadorErrores, const vector<short> &polinomioLocalizadorErrores, const vector<short> &polinomioEvaluadorErrores) {
    vector<short> valoresErrores;
    vector<short> ubicacionesErrores;
    vector<short> derivada = pol.derivarPolinomio(polinomioLocalizadorErrores);

    // Si no hay raíces en el polinomio localizador de errores, devolver listas vacías
    if (raicesPolinomioLocalizadorErrores.empty()) {
        return make_pair(valoresErrores, ubicacionesErrores);
    }
    
    // Calcular valores y ubicaciones de los errores
    for (short raiz : raicesPolinomioLocalizadorErrores) {
        // Calcular valor del error
        short valorError = calc.mult(calc.obtener_q(), calc.division(pol.evaluarPolinomio(polinomioEvaluadorErrores, raiz), pol.evaluarPolinomio(derivada, raiz)));
        valoresErrores.push_back(valorError);

        // Calcular ubicación del error
        short ubicacionError = (calc.obtener_q() - raiz) % calc.obtener_q();
        ubicacionesErrores.push_back(ubicacionError);
    }
    
    // Devolver valores y ubicaciones de los errores encontrados
    return make_pair(valoresErrores, ubicacionesErrores);
}

vector<short> Decodificador::decodificar(const vector<short> &palabraRecibida, const vector<short> &ubicacionesErrores, const vector<short> &valoresErrores) {
    vector<short> palabraDecodificada = palabraRecibida;

    // Corregir errores en las ubicaciones especificadas
    for (size_t i = 0; i < ubicacionesErrores.size(); ++i) {
        size_t indice = ubicacionesErrores[i];
        if (indice < palabraRecibida.size()) {
            size_t posicion = this->n - indice - 1;
            palabraDecodificada[posicion] = calc.resta(palabraRecibida[posicion], valoresErrores[i]);
        }
    }

    return palabraDecodificada;
}

vector<short> Decodificador::obtenerPolinomioLocalizador(const vector<short> &polinomioBorrados, const vector<short> &polinomioErrores) {
    // Multiplicar los polinomios localizadores de borrados y errores
    return pol.multiplicarPolinomios(polinomioBorrados, polinomioErrores);
}

vector<short> Decodificador::raicesNoNulas(const vector<short> &polinomio) {
    int contadorRaices = 0; // Contador de raíces encontradas
    vector<short> indicesRaices;   // Índices de las raíces no nulas
    
    int gradoPolinomio = (int)polinomio.size() - 1; // Grado del polinomio

    // Buscar raíces no nulas del polinomio en el campo GF(256)
    if (!polinomio.empty()) {
        for (int i = 1; i < 256; i++) {
            if (contadorRaices < gradoPolinomio) {
                if (pol.evaluarPolinomio(polinomio, i) == 0) {
                    indicesRaices.push_back(i);
                    contadorRaices++;
                }
            } else {
                break;
            }
        }
    }

    // Verificar si se encontraron todas las raíces del polinomio
    if (contadorRaices == gradoPolinomio) {
        return indicesRaices;
    } else {
        vector<short> noRaices;
        return noRaices; // Polinomio no tiene suficientes raíces no nulas
    }
}

vector<short> Decodificador::calcularSindromeModificado(const vector<short> &polinomioSindrome, const vector<short> &localizadorBorrado) {
    // Realizar la multiplicación de polinomios entre síndrome y localizador de borrado
    vector<short> producto = pol.multiplicarPolinomios(polinomioSindrome, localizadorBorrado);

    // Obtener el residuo de la división del polinomio resultante por el polinomio generador xr
    vector<short> residuodivision = pol.calcularResiduo(producto, this->xr);

    // Devolver el residuo calculado
    return residuodivision;
}