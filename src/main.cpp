#include <chrono>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>  
#include "../include/ManejadorDeArchivos.h"
#include "../include/Decodificador.h"
using namespace std;

void escribirEnArchivo(std::ofstream& salida, const std::vector<short>& vectorDecodificado) {
    size_t tamanioTotalBytes = vectorDecodificado.size() * sizeof(short);
    const char* datosEnBytes = reinterpret_cast<const char*>(vectorDecodificado.data());
    salida.write(datosEnBytes, tamanioTotalBytes);
}

void imprimirTiempos(std::chrono::seconds secondsDuration, int corregidos, int incorregibles, int correctos, int totales) {
    printf("Tiempo transcurrido: %lld\n", static_cast<long long>(secondsDuration.count()));
    printf("Incorregibles: %d bloques\n", incorregibles);
    printf("Correctos: %d bloques\n", correctos);
    printf("Corregidos: %d bloques\n", corregidos);
    printf("Total: %d bloques\n", totales);
}

bool incorregible(const vector<short> &indiceBorraduras, Decodificador &deco) {
    return deco.bloqIncorregible(indiceBorraduras);
}

void limpiarVector(vector<short> &v,int r) {
    for (int i = 0; i < r; ++i) {
        v.pop_back();
    }
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

int main(int argc, char *argv[]) {
    int n, r;
    int q = 256; //Por ahora fijo.
    ManejadorDeArchivos archivos;

    string archivo_simbolos;
    string archivo_borraduras;
    string archivo_salida;

    bool verificarPrecondiciones = archivos.procesarArgumentos(n, r, archivo_simbolos, archivo_borraduras, archivo_salida, argc, argv,q);

    if (!verificarPrecondiciones) {
        return 0;
    }

    bool abrir = archivos.abrirArchivos(archivo_simbolos, archivo_borraduras, archivo_salida);
    if (!abrir) {
        return 0;
    }

    //Inicializo bloques 
    int incorregibles = 0;
    int corregidos = 0;
    int correctos = 0;
    int totales = 0;

   //Inicio tiempo.
    std::chrono::time_point<std::chrono::high_resolution_clock> inicioReloj = std::chrono::high_resolution_clock::now();
    //Archivo de salida vacio.
    ofstream &salida = archivos.obtener_salida();
    //inicializo el decodificador
    Decodificador deco(n,q,r);


    //Obtengo matriz chequeo de paridad
    vector<vector<short>> matrizParidad = deco.obtenerMatrizChequeo();

    //Obtengo simbolos y borraduras.
    ifstream &simbolos = archivos.obtener_simbolo();

    //Leeo el primer bloque para entrar al while
    vector<short> y = deco.leerBloqueSimbolos(simbolos);

    //Leeo bloque de borraduras.
    ifstream &borraduras = archivos.obtener_borradura();
    
    while (!y.empty()) {
        vector<short> vector_decodificado = y;
        printf("Este es el vector y que llego:\n");
        imprimirVector(y);
        vector<short> indiceBorraduras = deco.leerIndiceBorraduras(borraduras);
        printf("Estas son las posiciones con borraduras en el bloque actual:\n");
        imprimirIndices(indiceBorraduras);
        if (incorregible(indiceBorraduras, deco)) {
            incorregibles++;
        } 
        else {
            vector<short> sindrome = calcularSindrome(y, matrizParidad, deco);
            printf("Este es el sindrome:\n");
            imprimirVector(sindrome);
            
            //Si el sindrome esta vacio gane, no hay errores!
            if(sindrome.empty()){
                correctos++;
            }
            else{ //Verifico errores .
                vector<short> localizador_borraduras = deco.calcularPolBorraduras(indiceBorraduras);
                printf("Este es el Polinomio de Borraduras:\n");
                imprimirVector(localizador_borraduras);

                vector<short> modifiedSindrome = deco.calcularSindromeModificado(sindrome, localizador_borraduras);
                printf("Este es el Sindrome Modificado:\n");
                imprimirVector(modifiedSindrome);
                printf("Este es el polinomio XR:\n");
                imprimirVector(deco.obtenerPolinomioXR());
                pair<vector<short>, vector<short>> res = deco.a_e_extendido(deco.obtenerPolinomioXR(),modifiedSindrome);

                vector<short> localizador_errores = res.first;
                printf("Este es el polinomio delta:\n");
                imprimirVector(localizador_errores);
                
                vector<short> evaluador_errores = res.second;
                printf("Este es el polinomio gama:\n");
                imprimirVector(evaluador_errores);

                localizador_errores = deco.obtenerPolinomioLocalizador(localizador_borraduras, localizador_errores);
                printf("Este es el polinomio delta modificado:\n");
                imprimirVector(localizador_errores);

                vector<short> indices_raices = deco.raicesNoNulas(localizador_errores);

                if (indices_raices.size()>0) {
                    //Aplico Forneys.       
                    auto error = deco.algoritmo_f(evaluador_errores,indices_raices, localizador_errores);

                    if (error.first.size() > 0 && error.second.size() > 0) {
                        vector<short> errores = error.first; //Valores de los errores
                        vector<short> ubicacion_errores = error.second; //Ubicacion de los errores
                        vector_decodificado = deco.decodificar(y,errores,ubicacion_errores);
                        printf("Este es el vector y decodificado:\n");
                        imprimirVector(vector_decodificado);
                        corregidos++;
                    } else {//Bloque Incorregible!!!
                        incorregibles++;
                    }
                } else {
                    incorregibles++;
                }
            }
        }
        limpiarVector(vector_decodificado,r);

        escribirEnArchivo(salida,vector_decodificado);

        //Agrego otro bloque a los totales
        totales++;

        //Incremento el contador
        deco.incrementoBloque();
        
        //Leeo el proximo bloque de simbolos
        y = deco.leerBloqueSimbolos(simbolos);
    }
    printf("---------------------                  TERMINO LA EJECUCION                         ----------------------\n");

    // Obtener el punto final en el tiempo
    std::chrono::high_resolution_clock::time_point finReloj = std::chrono::high_resolution_clock::now();

    // Calcular la duración en segundos
    std::chrono::seconds secondsDuration = std::chrono::duration_cast<std::chrono::seconds>(finReloj - inicioReloj);
    
    imprimirTiempos(secondsDuration, corregidos, incorregibles, correctos, totales);
    
    archivos.cerrarArchivos();
    
    return 0;
}