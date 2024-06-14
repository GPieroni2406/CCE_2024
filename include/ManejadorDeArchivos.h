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
    bool AbrirArchivos(const std::string& symbolFilePath, const std::string& erasureFilePath, const std::string& outputFilePath);
    std::ifstream& GetSymbolFile();
    std::ifstream& GetErasureFile();
    std::ofstream& GetOutputFile();
    void ImprimirMensajeDeAyuda();
    void ImprimirErrorEspecifico(const std::string& mensaje);
    bool ProcesarArgumentos(int &n, int &r, std::string &symbolfile, std::string &erasfile, std::string &outputfile, int argc, char *argv[]);
    void CerrarArchivos();
};

#endif // MANEJADORDEARCHIVOS_H