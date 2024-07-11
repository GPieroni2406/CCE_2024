// ManejadorDeArchivos.h
#ifndef MANEJADORDEARCHIVOS_H
#define MANEJADORDEARCHIVOS_H

#include <fstream>
#include <iostream>
#include <string>
using namespace std; 

class ManejadorDeArchivos {
private:
    std::ifstream simbolos;
    std::ifstream borraduras;
    std::ofstream salida;

public:
    ManejadorDeArchivos();
    bool abrirArchivos(const std::string& ruta_simbolos, const std::string& ruta_borraduras, const std::string& ruta_salida);
    std::ifstream& obtener_simbolo();
    std::ifstream& obtener_borradura();
    std::ofstream& obtener_salida();
    void imprimirMensajeDeAyuda();
    void imprimirErrorEspecifico(const std::string& mensaje);
    bool procesarArgumentos(int &n, int &r, std::string &archivo_simbolos, std::string &archivo_borraduras, std::string &archivo_salida, int argc, char *argv[],int q);
    void cerrarArchivos();
    bool verificar_indicador(const std::string &indicador, const std::string &valor, int &n, int &r, std::string &archivo_simbolos, std::string &archivo_borraduras, std::string &archivo_salida, bool &bandera_tamanio, bool &bandera_redundancia, bool &bandera_simbolo, bool &bandera_Borradura, bool &bandera_salida);
};

#endif // MANEJADORDEARCHIVOS_H