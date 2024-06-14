#include "../include/Polinomio.h"

vector<short> Polinomio::Multiplicar_Polinomios(vector<short> p1, vector<short> p2){ 
    vector<short> res(p1.size() + p2.size() - 1, 0);  
    for (long unsigned int i = 0; i < p1.size(); i++){
        for (long unsigned int j = 0; j < p2.size(); j++){
            res[i+j] = calc.Suma(res[i+j], calc.Mult(p1[i], p2[j]));
        }
    }
    return res;
}

short Polinomio::Evaluar_Polinomio(vector<short> pol, short i){
// i its the i from alpha^i, therefore we can make use of the table
    short res = 0;
    for (long unsigned int j = 0; j < pol.size(); j++){
        res = calc.Suma(res, calc.Mult(pol[j],_gfalog[i*j % (calc.Obtener_q())]));
    }
    return res;
}

vector<short> Polinomio::Derivar_Polinomio(vector<short> pol){
    vector<short> res(pol.size()-1);
    for (long unsigned int i = 1; i < pol.size(); i++){
        res[i-1] = calc.Mult(i, pol[i]);
    }
    return res;
}

std::pair<std::vector<short>, std::vector<short>> Polinomio::Dividir_Polinomio(vector<short> numerator, vector<short> denominator) {
    std::vector<short> quotient, remainder;

    // Remove the leading zeros in the denominator (just in case, it shouldnt happen)
    while (!denominator.empty() && denominator.back() == 0) {
        denominator.pop_back();
    }

    if (denominator.empty()) {
        std::cerr << "Error, esta dividiendo entre cero!" << std::endl;
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
        int term = calc.Division(remainder[i], denominator[denDegree]);

        quotient[i - denDegree] = term;

        for (int j = 0; j <= denDegree; ++j) {
            remainder[i - j] = calc.Resta(remainder[i-j],calc.Mult(term,denominator[denDegree - j]));
        }
    }

    // Remove extra zeros
    while (remainder.size()!=1 && remainder.back() == 0) {
        remainder.pop_back();
    }

    return std::make_pair(quotient, remainder);
}

std::vector<short> Polinomio::Restar_Polinomios(vector<short> p1, vector<short> p2) {
    size_t maxDegree = std::max(p1.size()-1, p2.size()-1);

    // Make the polnomials have the same size
    std::vector<short> pSumaedp1(maxDegree+1, 0);
    std::vector<short> pSumaedp2(maxDegree+1, 0);

    for (size_t i = 0; i < p1.size(); ++i) {
        pSumaedp1[i] = p1[i];
    }

    for (size_t i = 0; i < p2.size(); ++i) {
        pSumaedp2[i] = p2[i];
    }

    std::vector<short> result(maxDegree+1);
    for (size_t i = 0; i < maxDegree+1; ++i) {
        result[i] = calc.Resta(pSumaedp1[i], pSumaedp2[i]);
    }

    // Remove leading zeros in the result
    while (!result.empty() && result.back() == 0) {
        result.pop_back();
    }

    return result;
}