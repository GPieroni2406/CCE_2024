#include "../include/Decoder.h"
#include <algorithm>

vector<short> Decoder::LeerBloque(ifstream &archivo, const int &n) {
    // Calcular la cantidad de bytes a leer
    int bytesALeer = n; // Cada símbolo representa 1 byte

    // Calcular el desplazamiento
    int desplazamiento = this->cantBloquesLeidos * bytesALeer;

    // Mover el puntero de lectura al inicio del bloque deseado
    archivo.seekg(desplazamiento, ios::beg);

    // Leer el bloque de datos
    vector<short> buffer(n);
    archivo.read(reinterpret_cast<char*>(buffer.data()), bytesALeer);

    // Verificar si se leyeron todos los bytes esperados
    if (archivo.gcount() < static_cast<streamsize>(bytesALeer)) {
        return {}; // Devolver vector vacío si no se leyeron todos los datos
    }

    // No es necesario intercambiar los bytes porque cada símbolo representa 1 byte

    // Incrementar el contador de bloques leídos
    IncrementoBloque();

    return buffer;
}


vector<short> Decoder::EncontrarBorraduras(ifstream &archivo, const int &n) {
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


Decoder::Decoder(const int &n, const int &r) {
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

void Decoder::IncrementoBloque() {
    // Incrementar el contador de bloques leídos
    ++this->cantBloquesLeidos;
}

vector<short> Decoder::ObtenerPolinomioXR() {
    // Devolver una copia del vector xr
    vector<short> copiaXR(this->xr.begin(), this->xr.end());
    return copiaXR;
}


vector<short> Decoder::LeerBloqueSimbolos(ifstream &symbolfile) {
    // Llamar a la función LeerBloque para leer el bloque de símbolos
    return LeerBloque(symbolfile, this->n);
}



vector<short> Decoder::LeerIndiceSimbolos(ifstream &erasfile) {
    // Llamar a la función EncontrarBorraduras para obtener los índices de símbolos borrados
    return EncontrarBorraduras(erasfile, this->n);
}



vector<vector<short>> Decoder::ObtenerMatrizChequeo() {
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




vector<short> Decoder::CalcSindromePolinomial(const vector<short> &symbols, const vector<vector<short>> &H) {
    vector<short> sindrome(this->r, 0);

    // Calcular el polinomio de síndrome
    for (int i = 0; i < this->r; ++i) {
        for (int j = 0; j < this->n; ++j) {
            sindrome[i] = calc.Suma(sindrome[i], calc.Mult(symbols[j], H[i][j]));
        }
    }

    // Eliminar los coeficientes cero no significativos al final del polinomio
    while (!sindrome.empty() && sindrome.back() == 0) {
        sindrome.pop_back();
    }

    // Devolver el polinomio de síndrome calculado
    return sindrome;
}




vector<short> Decoder::CalcularPolBorraduras(const vector<short> &indicesBorrados) {
    // Polinomio inicializado como 1
    vector<short> polinomioBorraduras = {1};

    // Multiplicar por cada índice de borrado
    for (short indice : indicesBorrados) {
        // Coeficiente α^indice en el campo galoisiano
        short coeficiente = _gfalog[indice];

        // Polinomio (1 - α^indice)
        vector<short> factor = {calc.Resta(0, coeficiente), 1};

        // Multiplicación de polinomios
        polinomioBorraduras = pol.Multiplicar_Polinomios(polinomioBorraduras, factor);
    }

    return polinomioBorraduras;
}



bool Decoder::BloqIncorregible(vector<short> indicesBorrados) {
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



pair<vector<short>, vector<short>> Decoder::Euclides(const vector<short> &primer_polinomio, const vector<short> &segundo_polinomio) {
    // Calcular el límite según la fórmula dada
    double limite = (this->r + this->rho) / 2.0;

    // Inicializar variables
    vector<short> s_previous = {0};   // s_previous = 0
    vector<short> r_previous = segundo_polinomio;  // r_previous = segundo_polinomio
    vector<short> s_current = {1};    // s_current = 1
    vector<short> r_current = primer_polinomio;    // r_current = primer_polinomio

    // Algoritmo extendido de Euclides
    while (!(r_current.size() - 1 < limite && limite <= r_previous.size() - 1)) {
        // Dividir r_previous entre r_current
        auto division_result = pol.Dividir_Polinomio(r_previous, r_current);
        vector<short> quotient = division_result.first;
        r_previous = r_current;
        r_current = division_result.second;

        // Actualizar s_previous y s_current
        vector<short> temp_s = s_previous;
        s_previous = s_current;
        s_current = pol.Restar_Polinomios(temp_s, pol.Multiplicar_Polinomios(quotient, s_current));
    }

    // Retornar el resultado
    return make_pair(s_current, r_current);
}





pair<vector<short>, vector<short>> Decoder::Forneys(const vector<short> &raices_polinomio_localizador_errores, const vector<short> &polinomio_localizador_errores, const vector<short> &polinomio_evaluador_errores) {
    vector<short> valores_errores;
    vector<short> ubicaciones_errores;
    vector<short> derivada = pol.Derivar_Polinomio(polinomio_localizador_errores);

    // Si no hay raíces en el polinomio localizador de errores, devolver listas vacías
    if (raices_polinomio_localizador_errores.empty()) {
        return make_pair(valores_errores, ubicaciones_errores);
    }
    
    // Calcular valores y ubicaciones de los errores
    for (short raiz : raices_polinomio_localizador_errores) {
        // Calcular valor del error
        short valor_error = calc.Mult(calc.Obtener_q(), calc.Division(pol.Evaluar_Polinomio(polinomio_evaluador_errores, raiz), pol.Evaluar_Polinomio(derivada, raiz)));
        valores_errores.push_back(valor_error);

        // Calcular ubicación del error
        short ubicacion_error = (calc.Obtener_q() - raiz) % calc.Obtener_q();
        ubicaciones_errores.push_back(ubicacion_error);
    }
    
    // Devolver valores y ubicaciones de los errores encontrados
    return make_pair(valores_errores, ubicaciones_errores);
}



vector<short> Decoder::Decodificar(const vector<short> &palabraRecibida, const vector<short> &ubicacionesErrores, const vector<short> &valoresErrores) {
    vector<short> palabraDecodificada = palabraRecibida;

    // Corregir errores en las ubicaciones especificadas
    for (size_t i = 0; i < ubicacionesErrores.size(); ++i) {
        size_t indice = ubicacionesErrores[i];
        if (indice < palabraRecibida.size()) {
            size_t posicion = this->n - indice - 1;
            palabraDecodificada[posicion] = calc.Resta(palabraRecibida[posicion], valoresErrores[i]);
        }
    }

    return palabraDecodificada;
}

vector<short> Decoder::ObtenerPolinomioLocalizador(const vector<short> &polinomio_borrados, const vector<short> &polinomio_errores) {
    // Multiplicar los polinomios localizadores de borrados y errores
    return pol.Multiplicar_Polinomios(polinomio_borrados, polinomio_errores);
}



vector<short> Decoder::RaicesNoNulasChien(const vector<short> &polinomio) {
    vector<short> indicesRaices;   // Índices de las raíces no nulas
    int contadorRaices = 0; // Contador de raíces encontradas
    int gradoPolinomio = (int)polinomio.size() - 1; // Grado del polinomio

    // Buscar raíces no nulas del polinomio en el campo GF(256)
    if (!polinomio.empty()) {
        for (int i = 1; i < 256; i++) {
            if (contadorRaices < gradoPolinomio) {
                if (pol.Evaluar_Polinomio(polinomio, i) == 0) {
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




vector<short> Decoder::CalcularSindromeModificado(const vector<short> &polinomioSindrome, const vector<short> &localizadorBorrado) {
    // Realizar la multiplicación de polinomios entre síndrome y localizador de borrado
    vector<short> producto = pol.Multiplicar_Polinomios(polinomioSindrome, localizadorBorrado);

    // Dividir el polinomio resultante por el polinomio generador xr y obtener el residuo
    vector<short> residuoDivision = pol.Dividir_Polinomio(producto, this->xr).second;

    // Devolver el residuo calculado
    return residuoDivision;
}
