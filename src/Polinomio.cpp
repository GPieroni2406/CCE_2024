#include "../include/Polinomio.h"

void imprimirVectorPol(const vector<short> &vec) {
    for (short val : vec) {
        cout << val << ' ';
    }
    cout << endl;
}

std::pair<std::vector<short>, std::vector<short>> Polinomio::dividirPolinomio(std::vector<short> numerador, std::vector<short> denominador) {
    std::vector<short> cociente, residuo;
    for (; !denominador.empty() && denominador.back() == 0; denominador.pop_back()) {
    }

    if (denominador.empty()) {
        std::cerr << "Problemas con la división entre cero!" << std::endl;

    }
    else{
        int gradoNum = numerador.size() - 1;
        int gradoDen = denominador.size() - 1;
        residuo = numerador;
        if (gradoNum < gradoDen) {
            cociente.push_back(0);
            residuo = numerador;
        }
        else{
            cociente.resize(gradoNum - gradoDen + 1, 0);
            int indiceNum = gradoNum;
            while (indiceNum >= gradoDen) {
                int termino = calc.division(residuo[indiceNum], denominador[gradoDen]);
                cociente[indiceNum - gradoDen] = termino;

                int indiceDen = 0;
                while (indiceDen <= gradoDen) {
                    residuo[indiceNum - indiceDen] = calc.resta(residuo[indiceNum - indiceDen], calc.mult(termino, denominador[gradoDen - indiceDen]));
                    indiceDen++;
                }
                indiceNum--;
            }

            // Eliminar ceros adicionales
            while (residuo.size() != 1 && residuo.back() == 0) {
                residuo.pop_back();
            }


        }
    }
    return std::make_pair(cociente, residuo);
}


std::vector<short> Polinomio::derivarPolinomio(std::vector<short> coeficientes) {
    if (coeficientes.size() <= 1) {
        return std::vector<short>(); // Un polinomio constante tiene derivada 0.
    }

    std::vector<short> derivada(coeficientes.size() - 1);
    long unsigned int indice = 1; // Comenzamos en 1 porque la derivada de un término constante es 0.
    while (indice < coeficientes.size()) {
        derivada[indice - 1] = calc.mult(indice, coeficientes[indice]);
        indice++;
    }
    return derivada;
}

short Polinomio::evaluarPolinomio(std::vector<short> coeficientes, short x) {
    short polEvaluado = 0;
    long unsigned int indice = 0;
    while (indice < coeficientes.size()) {
        polEvaluado = calc.suma(polEvaluado, calc.mult(coeficientes[indice], _gfalog[x * indice % 255]));
        indice++;
    }
    return polEvaluado;
}

std::vector<short> Polinomio::multiplicarPolinomios(std::vector<short> polinomio1, std::vector<short> polinomio2) {
    int nuevoTamanio = polinomio1.size() + polinomio2.size() - 1;
    std::vector<short> polMultiplicado(nuevoTamanio, 0);
    long unsigned int i = 0;
    while (i < polinomio1.size()) {
        long unsigned int j = 0;
        while (j < polinomio2.size()) {
            polMultiplicado[i + j] = calc.suma(polMultiplicado[i + j], calc.mult(polinomio1[i], polinomio2[j]));
            j++;
        }
        i++;
    }
    return polMultiplicado;
}


std::vector<short> Polinomio::restarPolinomios(std::vector<short> polinomio1, std::vector<short> polinomio2) {
    int grado;
    if (polinomio1.size() > polinomio2.size()) {
        grado = polinomio1.size();
    } else {
        grado = polinomio2.size();
    }

    // Hacer que los polinomios tengan el mismo tamaño
    std::vector<short> paddedpolinomio1(grado, 0);
    std::vector<short> paddedpolinomio2(grado, 0);

    int indice = 0;
    while (indice < polinomio1.size()) {
        paddedpolinomio1[indice] = polinomio1[indice];
        indice++;
    }

    indice = 0; // Reiniciar el índice para el segundo polinomio
    while (indice < polinomio2.size()) {
        paddedpolinomio2[indice] = polinomio2[indice];
        indice++;
    }

    std::vector<short> polRestado(grado);
    indice = 0; // Reiniciar el índice para la resta
    while (indice < grado) {
        polRestado[indice] = calc.resta(paddedpolinomio1[indice], paddedpolinomio2[indice]);
        indice++;
    }

    // Eliminar ceros al principio del polRestado
    while (!polRestado.empty() && polRestado.back() == 0) {
        polRestado.pop_back();
    }

    return polRestado;
}