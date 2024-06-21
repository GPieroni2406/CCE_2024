#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>  
#include "../include/Decodificador.h"
#include "../include/ManejadorDeArchivos.h"
using namespace std;

void imprimirTiempos(std::chrono::seconds secondsDuration,int corregidos,int incorregibles,int correctos,int totales){
    cout << "Tiempo: " << secondsDuration.count() << endl;
    cout << "Bloques Corregidos: " << corregidos << endl;
    cout << "Bloques Incorregibles: " << incorregibles << endl;
    cout << "Bloques Correctos: " << correctos << endl;
    cout << "Total: " << totales << endl;
}


bool Incorregible(const vector<short> indiceBorraduras, Decodificador deco){
    if (deco.BloqIncorregible(indiceBorraduras)){
        return true;
    }
    else return false;
}

vector<short> CalcularSindrome(const vector<short> y,const vector<vector<short>> matrizParidad, Decodificador deco){
    vector<short> sindrome = deco.CalcSindromePolinomial(y, matrizParidad); 
    return sindrome;
}

void ImprimirContenidoArchivo(std::ifstream& file) {
    // Verificar si el flujo de archivo es válido y si se ha abierto correctamente
    if (!file.is_open() || file.fail()) {
        std::cerr << "El flujo de archivo no está abierto o está en un estado de error." << std::endl;
        return;
    }

    // Guardar la posición actual del puntero de lectura
    std::streampos currentPos = file.tellg();

    // Limpiar los flags de error y mover el puntero al inicio del archivo
    file.clear();
    file.seekg(0, std::ios::beg);

    // Leer y mostrar el contenido del archivo
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }

    // Restaurar la posición del puntero de lectura
    file.clear();
    file.seekg(currentPos);
}

void ImprimirVector(const std::vector<short>& vec) {
    for (short val : vec) {
        std::cout << val << ' ';
    }
    std::cout << std::endl;
}

void ImprimirMatrizParidad(const std::vector<std::vector<short>>& vec) {
    for (const auto& subVec : vec) {
        for (short val : subVec) {
            std::cout << val << ' ';
        }
        std::cout << std::endl; // Salto de línea después de cada sub-vector
    }
}

typedef struct {
    char fileType[4];   // Example header field: 4-byte file type
    int version;        // Example header field: version number
    int recordCount;    // Example header field: number of records
} Header;

int main(int argc, char *argv[]) {     
   
    int n,r;
    ManejadorDeArchivos archivos;
    
    //POR DEFECTO DEJO LOS PATHS
    string symbolFilePath = "misdatos.ech"; 
    string erasFilePath = "misdatos.txt"; 
    string outputFilePath = "salida.dat";
   
    bool verificarPrecondiciones = archivos.ProcesarArgumentos(n, r, symbolFilePath, erasFilePath, outputFilePath, argc, argv);
    
    if (!verificarPrecondiciones) {
        return 0;
    }

    bool abrir = archivos.AbrirArchivos(symbolFilePath, erasFilePath, outputFilePath);
    // Intentar abrir los archivos
    if (!abrir) {
        return 0;
    }
    // Obtener los flujos de archivo a través del manejador
    std::ifstream& sfile = archivos.GetSymbolFile();
    std::ifstream& efile = archivos.GetErasureFile();
    std::ofstream& outputFile = archivos.GetOutputFile();

    ImprimirContenidoArchivo(efile);

    auto inicioReloj = chrono::high_resolution_clock::now();
    Decodificador deco = Decodificador(n, r);

    int totales = 0;
    int corregidos = 0;
    int incorregibles = 0;
    int correctos = 0;

    vector<short> y = deco.LeerBloqueSimbolos(sfile);
    vector<vector<short>> matrizParidad = deco.ObtenerMatrizChequeo();
    while (!y.empty()) {

        vector<short> yDecodificado = y;
        vector<short> indiceBorraduras = deco.LeerIndiceSimbolos(efile);
        
        if(Incorregible(indiceBorraduras,deco)){
            incorregibles++;
        }
        else {
            vector<short> syndromePolynomial; 
            syndromePolynomial = CalcularSindrome(y,matrizParidad,deco);
            if (!syndromePolynomial.empty()){
                vector<short> erasureLocatorPolynomial = deco.CalcularPolBorraduras(indiceBorraduras); 

                vector<short> modifiedSyndromePolynomial = deco.CalcularSindromeModificado(syndromePolynomial, erasureLocatorPolynomial); 
               
                pair<vector<short>, vector<short>> res = deco.Euclides(modifiedSyndromePolynomial, deco.ObtenerPolinomioXR()); 
                vector<short> errorLocatorPolynomial = res.first; 
                vector<short> errorEvaluatorPolynomial = res.second; 
                
                errorLocatorPolynomial = deco.ObtenerPolinomioLocalizador(erasureLocatorPolynomial, errorLocatorPolynomial);

                vector<short> rootsIndexes = deco.RaicesNoNulasChien(errorLocatorPolynomial);

                if (!rootsIndexes.empty()) {
                    pair<vector<short>, vector<short>> error = deco.Forneys(rootsIndexes, errorLocatorPolynomial, errorEvaluatorPolynomial);
                    vector<short> errorValues = error.first;
                    vector<short> errorLocations = error.second;
                    if ((errorValues.empty() || (errorLocations.empty()))){
                        incorregibles++;
                    }
                    else{
                        yDecodificado = deco.Decodificar(y, errorLocations, errorValues);
                        corregidos++;
                    }
                }
                else {
                    incorregibles++;
                }
            }
            else {
                correctos++;
            }  
        }
        yDecodificado.erase(yDecodificado.end() - r, yDecodificado.end());

        transform(yDecodificado.begin(), yDecodificado.end(), yDecodificado.begin(), [](short value) {
            return (value << 8) | ((value >> 8) & 0xFF);
        });       
        
        outputFile.write((char*)yDecodificado.data(), yDecodificado.size() * sizeof(short));
        
        deco.IncrementoBloque();

        totales++;

    }

    archivos.CerrarArchivos();

    auto finReloj= chrono::high_resolution_clock::now();
    auto secondsDuration = chrono::duration_cast<chrono::seconds>(finReloj- inicioReloj);

    imprimirTiempos(secondsDuration,corregidos,incorregibles,correctos,totales);

    return 0;
}
