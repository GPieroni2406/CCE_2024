#include <iostream>
#include <string>
#include <vector>
#include "Decodificador.h"

void run_test(const std::vector<short>& receivedWord, 
              const std::vector<short>& errorLocations, 
              const std::vector<short>& errorValues, 
              const std::vector<short>& expected, 
              const std::string& test_name) {
    const int n = 10; // Usa los valores adecuados para n y r según tu caso
    const int r = 5;  // Usa los valores adecuados para n y r según tu caso
    Decodificador decodificador(n, r);
    std::vector<short> result = decodificador.Decodificar(receivedWord, errorLocations, errorValues);
    
    if (result == expected) {
        std::cout << "[PASSED] " << test_name << std::endl;
    } else {
        std::cerr << "[FAILED] " << test_name << std::endl;
        std::cerr << "Expected: ";
        for (short val : expected) std::cerr << val << " ";
        std::cerr << ", but got: ";
        for (short val : result) std::cerr << val << " ";
        std::cerr << std::endl;
    }
}

void run_all_tests() {
    // Ejemplo de datos de prueba (debes ajustarlos según tus necesidades)
    std::vector<short> receivedWord1 = {1, 2, 3, 4, 5};
    std::vector<short> errorLocations1 = {0, 2};
    std::vector<short> errorValues1 = {1, 1};
    std::vector<short> expected1 = {0, 2, 2, 4, 5};

    std::vector<short> receivedWord2 = {2, 4, 6, 8, 10};
    std::vector<short> errorLocations2 = {1, 3};
    std::vector<short> errorValues2 = {2, 2};
    std::vector<short> expected2 = {2, 2, 6, 6, 10};

    run_test(receivedWord1, errorLocations1, errorValues1, expected1, "Test 1: Basic decoding");
    run_test(receivedWord2, errorLocations2, errorValues2, expected2, "Test 2: Basic decoding");

    // Agrega más casos de prueba según sea necesario
}

int main() {
    run_all_tests();
    return 0;
}
