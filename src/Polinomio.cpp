#include "../include/Polinomio.h"

std::pair<std::vector<short>, std::vector<short>> Polinomio::dividirPolinomio(vector<short> numerator, vector<short> denominator) {
    std::vector<short> quotient, remainder;
    
    while (!denominator.empty() && denominator.back() == 0) {
        denominator.pop_back();
    }

    if (denominator.empty()) {
        std::cerr << "Error: Division by zero!" << std::endl;
        return std::make_pair(quotient, remainder);
    }

    int numDegree = numerator.size() - 1;
    int denDegree = denominator.size() - 1;
    remainder = numerator;

    if (numDegree < denDegree) {
        quotient.push_back(0);
        return std::make_pair(quotient, numerator);
    }

    quotient.resize(numDegree - denDegree + 1, 0);

    for (int i = numDegree; i >= denDegree; --i) {
        int term = calc.division(remainder[i], denominator[denDegree]);

        quotient[i - denDegree] = term;

        for (int j = 0; j <= denDegree; ++j) {
            remainder[i - j] = calc.resta(remainder[i-j],calc.mult(term,denominator[denDegree - j]));
        }
    }

    // Remove extra zeros
    while (remainder.size()!=1 && remainder.back() == 0) {
        remainder.pop_back();
    }

    return std::make_pair(quotient, remainder);
}

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
    vector<short> res(coeficientes.size()-1);
    for (long unsigned int i = 1; i < coeficientes.size(); i++){
        res[i-1] = calc.mult(i, coeficientes[i]);
    }
    return res;
}

short Polinomio::evaluarPolinomio(std::vector<short> coeficientes, short x) {
    short res = 0;
    for (long unsigned int j = 0; j < coeficientes.size(); j++){
        res = calc.suma(res, calc.mult(coeficientes[j],_gfalog[x*j % 255]));
    }
    return res;
}

std::vector<short> Polinomio::multiplicarPolinomios(std::vector<short> polinomio1, std::vector<short> polinomio2) {
    vector<short> res(polinomio1.size() + polinomio2.size() - 1, 0);  
    for (long unsigned int i = 0; i < polinomio1.size(); i++){
        for (long unsigned int j = 0; j < polinomio2.size(); j++){
            res[i+j] = calc.suma(res[i+j], calc.mult(polinomio1[i], polinomio2[j]));
        }
    }
    return res;
}


std::vector<short> Polinomio::restarPolinomios(std::vector<short> poly1, std::vector<short> poly2) {
    size_t maxDegree = std::max(poly1.size()-1, poly2.size()-1);

    // Make the polynomials have the same size
    std::vector<short> paddedPoly1(maxDegree+1, 0);
    std::vector<short> paddedPoly2(maxDegree+1, 0);

    for (size_t i = 0; i < poly1.size(); ++i) {
        paddedPoly1[i] = poly1[i];
    }

    for (size_t i = 0; i < poly2.size(); ++i) {
        paddedPoly2[i] = poly2[i];
    }

    std::vector<short> result(maxDegree+1);
    for (size_t i = 0; i < maxDegree+1; ++i) {
        result[i] = calc.resta(paddedPoly1[i], paddedPoly2[i]);
    }

    // Remove leading zeros in the result
    while (!result.empty() && result.back() == 0) {
        result.pop_back();
    }

    return result;
}