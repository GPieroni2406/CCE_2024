#include "../include/Polinomio.h"

std::vector<short> Polinomio::Multiplicar_Polinomios(std::vector<short> poly1, std::vector<short> poly2) {
    std::vector<short> producto(poly1.size() + poly2.size() - 1, 0);

    for (size_t a = 0; a < poly1.size(); ++a) {
        for (size_t b = 0; b < poly2.size(); ++b) {
            producto[a + b] += poly1[a] * poly2[b];
        }
    }

    return producto;
}

std::pair<std::vector<short>, std::vector<short>> Polinomio::Dividir_Polinomio(std::vector<short> numerador, std::vector<short> denominador) {
    std::vector<short> cociente, residuo;

    // Eliminar los ceros finales en el denominador (por si acaso, no debería suceder)
    while (!denominador.empty() && denominador.back() == 0) {
        denominador.pop_back();
    }

    if (denominador.empty()) {
        std::cerr << "Error, está dividiendo entre cero!" << std::endl;
        return std::make_pair(cociente, residuo);
    }

    residuo = numerador;
    int gradoNumerador = numerador.size() - 1;
    int gradoDenominador = denominador.size() - 1;

    if (gradoNumerador < gradoDenominador) {
        cociente.push_back(0);
        return std::make_pair(cociente, numerador);
    }

    cociente.resize(gradoNumerador - gradoDenominador + 1, 0);

    for (int idx = gradoNumerador; idx >= gradoDenominador; --idx) {
        short coeficiente = residuo[idx] / denominador[gradoDenominador];
        cociente[idx - gradoDenominador] = coeficiente;

        for (int j = 0; j <= gradoDenominador; ++j) {
            residuo[idx - j] -= coeficiente * denominador[gradoDenominador - j];
        }
    }

    // Eliminar ceros adicionales
    while (!residuo.empty() && residuo.back() == 0) {
        residuo.pop_back();
    }

    return std::make_pair(cociente, residuo);
}

vector<short> Polinomio::Derivar_Polinomio(vector<short> polynomial) {
    vector<short> derivative;
    for (size_t i = 1; i < polynomial.size(); ++i) {
        derivative.push_back(polynomial[i] * i);
    }
    return derivative;
}

short Polinomio::Evaluar_Polinomio(vector<short> polynomial, short exp) {
    short result = 0;
    for (size_t idx = 0; idx < polynomial.size(); ++idx) {
        result = calc.Suma(result, calc.Mult(polynomial[idx], _gfalog[exp * idx % (calc.Obtener_q())]));
    }
    return result;
}

std::vector<short> Polinomio::Restar_Polinomios(std::vector<short> poly1, std::vector<short> poly2) {
    size_t maxGrado = std::max(poly1.size(), poly2.size());

    // Redimensionar los polinomios al mismo tamaño
    poly1.resize(maxGrado, 0);
    poly2.resize(maxGrado, 0);

    std::vector<short> diferencia(maxGrado);
    for (size_t i = 0; i < maxGrado; ++i) {
        diferencia[i] = poly1[i] - poly2[i];
    }

    // Eliminar ceros al final del resultado
    while (!diferencia.empty() && diferencia.back() == 0) {
        diferencia.pop_back();
    }

    return diferencia;
}
