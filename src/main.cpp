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


bool Incorregible(const vector<short> indexErasures, Decodificador deco){
    if (deco.BloqIncorregible(indexErasures)){
        return true;
    }
    else return false;
}

vector<short> CalcularSindrome(const vector<short> y,const vector<vector<short>> ObtenerMatrizChequeo, Decodificador deco){
    vector<short> sindrome = deco.CalcSindromePolinomial(y, ObtenerMatrizChequeo); 
    return sindrome;
}



int main(int argc, char *argv[]) {     
   
    int n,r;
    ManejadorDeArchivos archivos;
    
    //POR DEFECTO DEJO LOS PATHS
    string symbolFilePath = "utils/misdatos.ech"; 
    string erasFilePath = "utils/misdatos.eras"; 
    string outputFilePath = "utils/salida.dat";
   
    bool verificarPrecondiciones = archivos.ProcesarArgumentos(n, r, symbolFilePath, erasFilePath, outputFilePath, argc, argv);

    if (!verificarPrecondiciones) {
        return 0;
    }

    bool abrirArch = archivos.AbrirArchivos(symbolFilePath, erasFilePath, outputFilePath);
    // Intentar abrir los archivos
    if (!abrirArch) {
        return 0;
    }
    // Obtener los flujos de archivo a través del manejador
    std::ifstream& sfile = archivos.GetSymbolFile();
    std::ifstream& efile = archivos.GetErasureFile();
    std::ofstream& outputFile = archivos.GetOutputFile();
    
    auto inicioReloj = chrono::high_resolution_clock::now();
    Decodificador deco = Decodificador(n, r);

    int totales = 0;
    int corregidos = 0;
    int incorregibles = 0;
    int correctos = 0;

    vector<short> y = deco.LeerBloqueSimbolos(sfile);
    vector<vector<short>> ObtenerMatrizChequeo = deco.ObtenerMatrizChequeo();
    
    while (!y.empty()) {

        vector<short> yDecodificado = y;
        vector<short> indexErasures = deco.LeerIndiceSimbolos(efile);
        
        if(Incorregible(indexErasures,deco)){
            incorregibles++;
        }
        else {
            vector<short> syndromePolynomial; 
            syndromePolynomial = CalcularSindrome(y,ObtenerMatrizChequeo,deco);
            if (!syndromePolynomial.empty()){
                vector<short> erasureLocatorPolynomial = deco.CalcularPolBorraduras(indexErasures); 

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
