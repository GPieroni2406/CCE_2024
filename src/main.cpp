#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>  
#include "../include/Decodificador.h"
#include "../include/ManejadorDeArchivos.h"
using namespace std;

void imprimirTiempos(chrono::seconds secondsDuration, int corregidos, int incorregibles, int correctos, int totales) {
    cout << "Tiempo: " << secondsDuration.count() << endl;
    cout << "Bloques Corregidos: " << corregidos << endl;
    cout << "Bloques Incorregibles: " << incorregibles << endl;
    cout << "Bloques Correctos: " << correctos << endl;
    cout << "Total: " << totales << endl;
}

bool incorregible(const vector<short> &indiceBorraduras, Decodificador &deco) {
    return deco.bloqIncorregible(indiceBorraduras);
}

vector<short> calcularSindrome(const vector<short> &y, const vector<vector<short>> &matrizParidad, Decodificador &deco) {
    return deco.calcSindromePolinomial(y, matrizParidad);
}

void imprimirContenidoArchivo(ifstream &file) {
    if (!file.is_open() || file.fail()) {
        cerr << "El flujo de archivo no está abierto o está en un estado de error." << endl;
        return;
    }

    streampos currentPos = file.tellg();
    file.clear();
    file.seekg(0, ios::beg);

    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }

    file.clear();
    file.seekg(currentPos);
}

void imprimirVector(const vector<short> &vec) {
    for (short val : vec) {
        cout << val << ' ';
    }
    cout << endl;
}

void imprimirIndices(const std::vector<short>& indices) {
    if (indices.empty()) {
        std::cout << "No hay borraduras." << std::endl;
    } else {
        std::cout << "Índices de borraduras: ";
        for (short indice : indices) {
            std::cout << indice << " ";
        }
        std::cout << std::endl;
    }
}

void imprimirMatrizParidad(const vector<vector<short>> &vec) {
    for (const auto &subVec : vec) {
        for (short val : subVec) {
            cout << val << ' ';
        }
        cout << endl;
    }
}

void comprueboEndian(){
    unsigned int i = 1;
    char *c = reinterpret_cast<char*>(&i);
    if (*c) //0000000000001
        std::cout << "Little endian" << std::endl;
    else // 1000000000000
        std::cout << "Big endian" << std::endl;
}

typedef struct {
    char fileType[4];
    int version;
    int recordCount;
} Header;

int main(int argc, char *argv[]) {
    int n, r;
    ManejadorDeArchivos archivos;

    string symbolFilePath = "../simb_borr/misdatos.ech";
    string erasFilePath = "../simb_borr/misdatos.eras";
    string outputFilePath = "../salidas/salida.dat";

    bool verificarPrecondiciones = archivos.procesarArgumentos(n, r, symbolFilePath, erasFilePath, outputFilePath, argc, argv);

    if (!verificarPrecondiciones) {
        return 0;
    }

    bool abrir = archivos.abrirArchivos(symbolFilePath, erasFilePath, outputFilePath);
    if (!abrir) {
        return 0;
    }

    ifstream &sfile = archivos.getSymbolFile();
    ifstream &efile = archivos.getErasureFile();
    ofstream &outputFile = archivos.getOutputFile();

    auto inicioReloj = chrono::high_resolution_clock::now();
    Decodificador deco(n, r);

    int totales = 0;
    int corregidos = 0;
    int incorregibles = 0;
    int correctos = 0;

    vector<short> y = deco.leerBloqueSimbolos(sfile);
    vector<vector<short>> matrizParidad = deco.obtenerMatrizChequeo();
    printf("Esta es la matriz de chequeo\n");
    imprimirMatrizParidad(matrizParidad);
    int cont=0;
    while (!y.empty()) {
    //while (cont<1){
        vector<short> yDecodificado = y;
        printf("Este es el vector y que llego:\n");
        imprimirVector(yDecodificado);
        vector<short> indiceBorraduras = deco.leerIndiceBorraduras(efile);
        printf("Estas son las posiciones con borraduras en el bloque actual:\n");
        imprimirIndices(indiceBorraduras);
        if (incorregible(indiceBorraduras, deco)) {
            incorregibles++;
        } 
        else {
            vector<short> syndromePolynomial = calcularSindrome(y, matrizParidad, deco);
            printf("Este es el sindrome:\n");
            imprimirVector(syndromePolynomial);
            if (!syndromePolynomial.empty()) {
                vector<short> erasureLocatorPolynomial = deco.calcularPolBorraduras(indiceBorraduras);
                printf("Este es el Polinomio de Borraduras:\n");
                imprimirVector(erasureLocatorPolynomial);

                vector<short> modifiedSyndromePolynomial = deco.calcularSindromeModificado(syndromePolynomial, erasureLocatorPolynomial);
                printf("Este es el Sindrome Modificado:\n");
                imprimirVector(modifiedSyndromePolynomial);
                printf("Este es el polinomio XR:\n");
                imprimirVector(deco.obtenerPolinomioXR());
                pair<vector<short>, vector<short>> res = deco.euclides(modifiedSyndromePolynomial, deco.obtenerPolinomioXR());

                vector<short> errorLocatorPolynomial = res.first;
                printf("Este es el polinomio delta:\n");
                imprimirVector(errorLocatorPolynomial);
                
                vector<short> errorEvaluatorPolynomial = res.second;
                printf("Este es el polinomio gama:\n");
                imprimirVector(errorEvaluatorPolynomial);

                errorLocatorPolynomial = deco.obtenerPolinomioLocalizador(erasureLocatorPolynomial, errorLocatorPolynomial);
                printf("Este es el polinomio delta modificado:\n");
                imprimirVector(errorLocatorPolynomial);

                vector<short> rootsIndexes = deco.raicesNoNulas(errorLocatorPolynomial);

                if (!rootsIndexes.empty()) {                  
                    pair<vector<short>, vector<short>> error = deco.forneys(rootsIndexes, errorLocatorPolynomial, errorEvaluatorPolynomial);
                    vector<short> errorValues = error.first;
                    vector<short> errorLocations = error.second;
                    if (errorValues.empty() || errorLocations.empty()) {
                        incorregibles++;
                    } else {
                        yDecodificado = deco.decodificar(y, errorLocations, errorValues);
                        printf("Este es el vector y decodificado:\n");
                        imprimirVector(yDecodificado);
                        corregidos++;
                    }
                } else {
                    incorregibles++;
                }
            } else {
                correctos++;
            }
        }

        yDecodificado.erase(yDecodificado.end() - r, yDecodificado.end());
        //transform(yDecodificado.begin(), yDecodificado.end(), yDecodificado.begin(), [](short value) {
        //    return (value << 8) | ((value >> 8) & 0xFF);
        //});

        outputFile.write(reinterpret_cast<char*>(yDecodificado.data()), yDecodificado.size() * sizeof(short));
        deco.incrementoBloque();
        totales++;
        y = deco.leerBloqueSimbolos(sfile);
        cont ++;
    }
    archivos.cerrarArchivos();
    printf("---------------------                  TERMINO LA EJECUCION                         ----------------------\n");

    auto finReloj = chrono::high_resolution_clock::now();
    auto secondsDuration = chrono::duration_cast<chrono::seconds>(finReloj - inicioReloj);

    imprimirTiempos(secondsDuration, corregidos, incorregibles, correctos, totales);
    return 0;
}