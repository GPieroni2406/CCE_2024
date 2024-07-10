#include <algorithm>
#include "../include/Decodificador.h"


Decodificador::Decodificador(const int &n,const int &q,const int &r) : n(n), q(q) ,r(r), cantBloquesLeidos(0), polinomio_xr(r + 1) {
    // Inicializar los primeros 'r' elementos con 0, el último elemento será 1 por defecto.
    // Como ya se realizó la inicialización en la lista de inicialización, no es necesario hacer push_back.
    polinomio_xr[r] = 1; // Establecer el último elemento a 1.
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
    return datos;
}

vector<short> Decodificador::encontrarBorraduras(ifstream &archivo, const int &n) {
    vector<short> indices;

    // Calcular el desplazamiento
    int desplazamiento = this->cantBloquesLeidos * n;

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
    return this->polinomio_xr;
}

vector<short> Decodificador::leerBloqueSimbolos(ifstream &symbolfile) {
    // Llamar a la función leerBloque para leer el bloque de símbolos
    return leerBloque(symbolfile, this->n);
}

vector<short> Decodificador::leerIndiceBorraduras(ifstream &erasfile) {
    // Llamar a la función encontrarBorraduras para obtener los índices de símbolos borrados
    return encontrarBorraduras(erasfile, this->n);
}

std::vector<std::vector<short>> Decodificador::obtenerMatrizChequeo() {
    std::vector<std::vector<short>> matriz;
    int i = 0;
    while (i < this->r) {
        std::vector<short> fila;
        int j = 0;
        while (j < this->n) {
            fila.push_back(_gfalog[((i + 1) * j) % (obtenerQ()-1)]);
            j++;
        }
        matriz.push_back(fila);
        i++;
    }
    return matriz;
}

std::vector<short> Decodificador::calcSindromePolinomial(const std::vector<short> &s, const std::vector<std::vector<short>> &H) {
    std::vector<short> sindrome;
    int i = 0;

    // Calcular el polinomio de síndrome
    while (i < this->r) {
        short sum = 0;
        int j = 0;
        while (j < this->n) {
            sum = calc.suma(sum, calc.mult(s[this->n - j - 1], H[i][j]));
            j++;
        }
        sindrome.push_back(sum);
        i++;
    }

    // Eliminar los coeficientes cero no significativos al final del polinomio
    for (; !sindrome.empty() && sindrome.back() == 0; sindrome.pop_back()) {
        // No se necesita cuerpo en el bucle for.
    }

    // Devolver el polinomio de síndrome calculado
    return sindrome;
}

std::vector<short> Decodificador::calcularPolBorraduras(const std::vector<short> &indicesBorrados) {
    // Polinomio inicializado como 1
    std::vector<short> polinomioBorraduras(1, 1);

    // Multiplicar por cada índice de borrado
    auto it = indicesBorrados.begin();
    while (it != indicesBorrados.end()) {
        short coeficiente = _gfalog[*it];
        std::vector<short> factor = {1, calc.resta(0, coeficiente)};
        polinomioBorraduras = pol.multiplicarPolinomios(polinomioBorraduras, factor);
        ++it;
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

std::pair<std::vector<short>, std::vector<short>> Decodificador::a_e_extendido(const std::vector<short> &polinomio2,const std::vector<short> &polinomio1) {
    //Inicializo vectores
    std::vector<short> r0 = polinomio2;
    std::vector<short> t0 = {0};
    std::vector<short> t1 = {1};
    std::vector<short> r1 = polinomio1;

    //obtengo valores
    int redundancia = this->obtenerRedundancia();
    int ro = this->obtenerRho();
    double fin = static_cast<double>(redundancia + ro) / 2.0;

    //Core del algoritmo euclides extendido
    for (; !(r1.size() - 1 < fin && fin <= r0.size() - 1); ) {
        std::pair<std::vector<short>, std::vector<short>> resultado = pol.dividirPolinomio(r0, r1);

        std::vector<short> cociente = resultado.first;
        std::vector<short> temp = t0;
        t0 = t1;
        std::vector<short> mult = pol.multiplicarPolinomios(cociente, t1);
        t1 = pol.restarPolinomios(temp, mult);

        std::vector<short> temp_1 = r1;
        r1 = resultado.second;
        r0 = temp_1;

    }

    return std::make_pair(t1, r1);
}

std::pair<std::vector<short>, std::vector<short>> Decodificador::algoritmo_f( const std::vector<short> &polinomioEvaluadorErrores,const std::vector<short> &indices, const std::vector<short> &p_localizador) {
    
    std::vector<short> ubiErrores;
    std::vector<short> derivada;
    std::vector<short> errores;
    if (indices.empty()) {
        return std::make_pair(errores, ubiErrores);
    }

    derivada = pol.derivarPolinomio(p_localizador);
    size_t indice = 0;
    while (indice < indices.size()) {
        short i = indices[indice];
        short evaluacion1 = pol.evaluarPolinomio(polinomioEvaluadorErrores, i);
        short evaluacion2 = pol.evaluarPolinomio(derivada, i);
        short division = calc.division(evaluacion1,evaluacion2);
        short mult = calc.mult((obtenerQ()-1),division);
        errores.push_back(mult);
        ubiErrores.push_back(((obtenerQ()-1) - i) % (obtenerQ()-1));
        indice++;
    }

    return std::make_pair(errores, ubiErrores);
}

std::vector<short> Decodificador::decodificar(const std::vector<short> &palabraRecibida, const std::vector<short> &valoresErrores, const std::vector<short> &ubicacionesErrores) {
    std::vector<short> palabraDecodificada = palabraRecibida;
    size_t totalErrores = ubicacionesErrores.size();
    size_t indiceError = 0;

    while (indiceError < totalErrores) {
        int ubicacionError = ubicacionesErrores[indiceError];
        if (ubicacionError < static_cast<int>(palabraRecibida.size())) {
            int indicePalabra = this->n - ubicacionError - 1;
            short valorCorreccion = valoresErrores[indiceError];
            palabraDecodificada[indicePalabra] = calc.resta(palabraDecodificada[indicePalabra], valorCorreccion);
        }
        indiceError++;
    }

    return palabraDecodificada;
}

vector<short> Decodificador::obtenerPolinomioLocalizador(const vector<short> &polinomioBorrados, const vector<short> &polinomioErrores) {
    // Multiplicar los polinomios localizadores de borrados y errores
    return pol.multiplicarPolinomios(polinomioBorrados, polinomioErrores);
}

std::vector<short> Decodificador::raicesNoNulas(const std::vector<short> &polinomio) {
    std::vector<short> indices = {};
    int cantidadRaices = 0;
    int grado = static_cast<int>(polinomio.size()) - 1;
    int contRaices = 1;

    if (!polinomio.empty()) {
        while (contRaices < obtenerQ() && cantidadRaices < grado) {
            short evaluar = pol.evaluarPolinomio(polinomio, contRaices);
            if (evaluar == 0) { //Encontre una raiz
                cantidadRaices++;
                printf("raiz encontrada en indices %d\n", contRaices);
                indices.push_back(contRaices);
            }
            contRaices++;
        }
    }

    if (cantidadRaices < grado) {
        printf("Se encontraron menos raices que el grado del polinomio.\n");
        return {};
    }
    return indices;
}

vector<short> Decodificador::calcularSindromeModificado(const vector<short> &polinomioSindrome, const vector<short> &indicesBorrados) {
    // Realizar la multiplicación de polinomios entre síndrome y localizador de borrado
    vector<short> producto = pol.multiplicarPolinomios(polinomioSindrome, indicesBorrados);

    // Obtener el residuo de la división del polinomio resultante por el polinomio generador polinomio_xr
    vector<short> residuodivision = pol.dividirPolinomio(producto, this->polinomio_xr).second;

    // Devolver el residuo calculado
    return residuodivision;
}