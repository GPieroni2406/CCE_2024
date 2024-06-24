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

void imprimirMatrizParidad(const vector<vector<short>> &vec) {
    for (const auto &subVec : vec) {
        for (short val : subVec) {
            cout << val << ' ';
        }
        cout << endl;
    }
}

typedef struct {
    char fileType[4];
    int version;
    int recordCount;
} Header;

int main(int argc, char *argv[]) {
    int n, r;
    ManejadorDeArchivos archivos;

    string symbolFilePath = "misdatos.ech";
    string erasFilePath = "misdatos.eras";
    string outputFilePath = "salida.dat";

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

    imprimirContenidoArchivo(efile);

    auto inicioReloj = chrono::high_resolution_clock::now();
    Decodificador deco(n, r);

    int totales = 0;
    int corregidos = 0;
    int incorregibles = 0;
    int correctos = 0;

    vector<short> y = deco.leerBloqueSimbolos(sfile);
    vector<vector<short>> matrizParidad = deco.obtenerMatrizChequeo();
    while (!y.empty()) {
        vector<short> yDecodificado = y;
        vector<short> indiceBorraduras = deco.leerIndiceSimbolos(efile);

        if (incorregible(indiceBorraduras, deco)) {
            incorregibles++;
        } else {
            vector<short> syndromePolynomial = calcularSindrome(y, matrizParidad, deco);
            if (!syndromePolynomial.empty()) {
                vector<short> erasureLocatorPolynomial = deco.calcularPolBorraduras(indiceBorraduras);
                vector<short> modifiedSyndromePolynomial = deco.calcularSindromeModificado(syndromePolynomial, erasureLocatorPolynomial);

                pair<vector<short>, vector<short>> res = deco.euclides(modifiedSyndromePolynomial, deco.obtenerPolinomioXR());
                vector<short> errorLocatorPolynomial = res.first;
                vector<short> errorEvaluatorPolynomial = res.second;

                errorLocatorPolynomial = deco.obtenerPolinomioLocalizador(erasureLocatorPolynomial, errorLocatorPolynomial);
                vector<short> rootsIndexes = deco.raicesNoNulas(errorLocatorPolynomial);

                if (!rootsIndexes.empty()) {
                    pair<vector<short>, vector<short>> error = deco.forneys(rootsIndexes, errorLocatorPolynomial, errorEvaluatorPolynomial);
                    vector<short> errorValues = error.first;
                    vector<short> errorLocations = error.second;
                    if (errorValues.empty() || errorLocations.empty()) {
                        incorregibles++;
                    } else {
                        yDecodificado = deco.decodificar(y, errorLocations, errorValues);
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
        transform(yDecodificado.begin(), yDecodificado.end(), yDecodificado.begin(), [](short value) {
            return (value << 8) | ((value >> 8) & 0xFF);
        });

        outputFile.write(reinterpret_cast<char*>(yDecodificado.data()), yDecodificado.size() * sizeof(short));
        deco.incrementoBloque();
        totales++;
        y = deco.leerBloqueSimbolos(sfile);
    }

    archivos.cerrarArchivos();
    auto finReloj = chrono::high_resolution_clock::now();
    auto secondsDuration = chrono::duration_cast<chrono::seconds>(finReloj - inicioReloj);

    imprimirTiempos(secondsDuration, corregidos, incorregibles, correctos, totales);

    return 0;
}