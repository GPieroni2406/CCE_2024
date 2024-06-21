#include <algorithm>
#include "Decodificador.h"


vector<short> Decodificador::LeerBloque(ifstream &archivo, const int &n) {
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

    // No es necesario intercambiar los bytes porque cada símbolo representa 1 byte

    // Incrementar el contador de bloques leídos
    IncrementoBloque();

    return datos;
}



vector<short> Decodificador::EncontrarBorraduras(ifstream &archivo, const int &n) {
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

void Decodificador::IncrementoBloque() {
    // Incrementar el contador de bloques leídos
    ++this->cantBloquesLeidos;
}

vector<short> Decodificador::ObtenerPolinomioXR() {
    // Devolver una copia del vector xr
    vector<short> copiaXR(this->xr.begin(), this->xr.end());
    return copiaXR;
}


vector<short> Decodificador::LeerBloqueSimbolos(ifstream &symbolfile) {
    // Llamar a la función LeerBloque para leer el bloque de símbolos
    return LeerBloque(symbolfile, this->n);
}



vector<short> Decodificador::LeerIndiceSimbolos(ifstream &erasfile) {
    vector<short> borraduras = EncontrarBorraduras(erasfile, this->n);
    return borraduras;
}


vector<vector<short>> Decodificador::ObtenerMatrizChequeo() {
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




vector<short> Decodificador::CalcSindromePolinomial(const vector<short> &symbols, const vector<vector<short>> &H) {
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




vector<short> Decodificador::CalcularPolBorraduras(const vector<short> &indicesBorrados) {
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



bool Decodificador::BloqIncorregible(vector<short> indicesBorrados) {
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



std::pair<std::vector<short>, std::vector<short>> Decodificador::Euclides(const std::vector<short> &primer_polinomio, const std::vector<short> &segundo_polinomio) {
    // Calcular el límite según la fórmula dada
    double limite = (this->r + this->rho) / 2.0;

    // Inicializar variables
    std::vector<short> s_anterior = {0};   // s_anterior = 0
    std::vector<short> r_anterior = segundo_polinomio;  // r_anterior = segundo_polinomio
    std::vector<short> s_actual = {1};    // s_actual = 1
    std::vector<short> r_actual = primer_polinomio;    // r_actual = primer_polinomio

    // Algoritmo extendido de Euclides
    while (!(r_actual.size() - 1 < limite && limite <= r_anterior.size() - 1)) {
        // Calcular cociente y residuo de r_anterior entre r_actual
        std::vector<short> cociente = pol.CalcularCociente(r_anterior, r_actual);
        std::vector<short> nuevo_r_anterior = r_actual;
        r_actual = pol.CalcularResiduo(r_anterior, r_actual);
        r_anterior = nuevo_r_anterior;

        // Actualizar s_anterior y s_actual
        std::vector<short> temp_s = s_anterior;
        s_anterior = s_actual;
        s_actual = pol.Restar_Polinomios(temp_s, pol.Multiplicar_Polinomios(cociente, s_actual));
    }

    // Retornar el resultado
    return std::make_pair(s_actual, r_actual);
}




pair<vector<short>, vector<short>> Decodificador::Forneys(const vector<short> &raices_polinomio_localizador_errores, const vector<short> &polinomio_localizador_errores, const vector<short> &polinomio_evaluador_errores) {
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



vector<short> Decodificador::Decodificar(const vector<short> &palabraRecibida, const vector<short> &ubicacionesErrores, const vector<short> &valoresErrores) {
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

vector<short> Decodificador::ObtenerPolinomioLocalizador(const vector<short> &polinomio_borrados, const vector<short> &polinomio_errores) {
    // Multiplicar los polinomios localizadores de borrados y errores
    return pol.Multiplicar_Polinomios(polinomio_borrados, polinomio_errores);
}



vector<short> Decodificador::RaicesNoNulasChien(const vector<short> &polinomio) {
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




std::vector<short> Decodificador::CalcularSindromeModificado(const std::vector<short> &polinomioSindrome, const std::vector<short> &localizadorBorrado) {
    // Realizar la multiplicación de polinomios entre síndrome y localizador de borrado
    std::vector<short> producto = pol.Multiplicar_Polinomios(polinomioSindrome, localizadorBorrado);

    // Obtener el residuo de la división del polinomio resultante por el polinomio generador xr
    std::vector<short> residuoDivision = pol.CalcularResiduo(producto, this->xr);

    // Devolver el residuo calculado
    return residuoDivision;
}