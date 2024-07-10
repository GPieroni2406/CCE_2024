// ManejadorDeArchivos.h
#ifndef MANEJADORDEARCHIVOS_H
#define MANEJADORDEARCHIVOS_H

#include <fstream>
#include <iostream>
#include <string>
using namespace std; 

class ManejadorDeArchivos {
private:
    std::ifstream sfile;
    std::ifstream efile;
    std::ofstream outputFile;

public:
    ManejadorDeArchivos();
    bool abrirArchivos(const std::string& symbolFilePath, const std::string& erasureFilePath, const std::string& outputFilePath);
    std::ifstream& getSymbolFile();
    std::ifstream& getErasureFile();
    std::ofstream& getOutputFile();
    void imprimirMensajeDeAyuda();
    void imprimirErrorEspecifico(const std::string& mensaje);
    bool procesarArgumentos(int &n, int &r, std::string &archivo_simbolos, std::string &archivo_borraduras, std::string &archivo_salida, int argc, char *argv[]);
    void cerrarArchivos();
};

#endif // MANEJADORDEARCHIVOS_H