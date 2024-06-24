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
    bool procesarArgumentos(int &n, int &r, std::string &symbolfile, std::string &erasfile, std::string &outputfile, int argc, char *argv[]);
    void cerrarArchivos();
};

#endif // MANEJADORDEARCHIVOS_H