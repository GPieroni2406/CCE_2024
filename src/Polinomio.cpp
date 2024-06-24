#include "../include/Polinomio.h"


std::vector<short> Polinomio::calcularCociente(const std::vector<short> &numerador, std::vector<short> denominador) {
    std::vector<short> cociente;
    
    // Eliminar los ceros finales del denominador para evitar errores
    while (!denominador.empty() && denominador.back() == 0) {
        denominador.pop_back();
    }

    // Comprobar si el denominador se ha convertido en un polinomio nulo
    if (denominador.empty()) {
        std::cerr << "Error: división por cero" << std::endl;
        return cociente;
    }

    std::vector<short> residuo = numerador;
    int gradoNumerador = numerador.size() - 1;
    int gradoDenominador = denominador.size() - 1;

    // Si el grado del numerador es menor que el del denominador, el cociente es cero
    if (gradoNumerador < gradoDenominador) {
        cociente.push_back(0);
        return cociente;
    }

    // Inicializar el vector cociente con ceros
    cociente.resize(gradoNumerador - gradoDenominador + 1, 0);

    // Realizar la división de polinomios
    for (int i = gradoNumerador; i >= gradoDenominador; --i) {
        short coeficiente = residuo[i] / denominador[gradoDenominador];
        cociente[i - gradoDenominador] = coeficiente;

        // Restar el polinomio del divisor multiplicado por el coeficiente al residuo
        for (int j = 0; j <= gradoDenominador; ++j) {
            residuo[i - j] -= coeficiente * denominador[gradoDenominador - j];
        }
    }

    return cociente;
}

std::vector<short> Polinomio::calcularResiduo(const std::vector<short> &numerador, std::vector<short> denominador) {
    std::vector<short> residuo;

    // Eliminar los ceros finales del denominador para evitar errores
    while (!denominador.empty() && denominador.back() == 0) {
        denominador.pop_back();
    }

    // Comprobar si el denominador se ha convertido en un polinomio nulo
    if (denominador.empty()) {
        std::cerr << "Error: división por cero" << std::endl;
        return residuo;
    }

    residuo = numerador;
    int gradoNumerador = numerador.size() - 1;
    int gradoDenominador = denominador.size() - 1;

    // Si el grado del numerador es menor que el del denominador, el residuo es el numerador
    if (gradoNumerador < gradoDenominador) {
        return numerador;
    }

    // Realizar la división de polinomios
    for (int i = gradoNumerador; i >= gradoDenominador; --i) {
        short coeficiente = residuo[i] / denominador[gradoDenominador];

        // Restar el polinomio del divisor multiplicado por el coeficiente al residuo
        for (int j = 0; j <= gradoDenominador; ++j) {
            residuo[i - j] -= coeficiente * denominador[gradoDenominador - j];
        }
    }

    // Eliminar ceros finales del residuo usando un reverse iterator
    auto rit = residuo.rbegin();
    while (rit != residuo.rend() && *rit == 0) {
        ++rit;
    }
    residuo.erase(rit.base(), residuo.end());

    return residuo;
}

std::vector<short> Polinomio::derivarPolinomio(std::vector<short> coeficientes) {
    // Inicializar el vector para almacenar los coeficientes de la derivada
    std::vector<short> derivada;

    // Recorrer los coeficientes del polinomio, comenzando desde el segundo término
    for (size_t indice = 1; indice < coeficientes.size(); ++indice) {
        // Calcular el nuevo coeficiente multiplicando el coeficiente actual por su índice
        derivada.push_back(coeficientes[indice] * static_cast<short>(indice));
    }

    // Devolver el vector que contiene los coeficientes de la derivada del polinomio
    return derivada;
}

short Polinomio::evaluarPolinomio(std::vector<short> coeficientes, short x) {
    short resultado = 0;
    short potencia = 1; // x^0
    for (size_t indice = 0; indice < coeficientes.size(); ++indice) {
        resultado += coeficientes[indice] * potencia;
        potencia *= x; // siguiente potencia de x
    }
    return resultado;
}

std::vector<short> Polinomio::multiplicarPolinomios(std::vector<short> polinomio1, std::vector<short> polinomio2) {
    // Inicializar el vector resultado con ceros, su tamaño es la suma de los grados de los dos polinomios menos uno
    std::vector<short> resultado(polinomio1.size() + polinomio2.size() - 1, 0);

    // Multiplicar cada coeficiente de polinomio1 por cada coeficiente de polinomio2
    for (size_t i = 0; i < polinomio1.size(); ++i) {
        for (size_t j = 0; j < polinomio2.size(); ++j) {
            // Sumar el producto de los coeficientes correspondientes al lugar apropiado en el vector resultado
            resultado[i + j] += polinomio1[i] * polinomio2[j];
        }
    }

    // Devolver el polinomio resultante de la multiplicación
    return resultado;
}


std::vector<short> Polinomio::restarPolinomios(std::vector<short> minuendo, std::vector<short> sustraendo) {
    size_t maxGrado = std::max(minuendo.size(), sustraendo.size());

    // Redimensionar los polinomios al mismo tamaño
    minuendo.resize(maxGrado, 0);
    sustraendo.resize(maxGrado, 0);

    std::vector<short> diferencia(maxGrado);

    
    for (size_t i = 0; i < maxGrado; ++i) {
        diferencia[i] = minuendo[i] - sustraendo[i];
    }


    // Eliminar ceros al final del resultado
    while (!diferencia.empty() && diferencia.back() == 0) {
        diferencia.pop_back();
    }

    return diferencia;
}