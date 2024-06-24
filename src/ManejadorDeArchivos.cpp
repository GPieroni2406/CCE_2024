#include "../include/ManejadorDeArchivos.h" // Asegúrate de que la ruta sea correcta

ManejadorDeArchivos::ManejadorDeArchivos() {
    // Implementación del constructor si es necesario
}

bool ManejadorDeArchivos::abrirArchivos(const std::string& symbolFilePath, const std::string& erasureFilePath, const std::string& outputFilePath) {
    sfile.open(symbolFilePath, std::ios::binary);
    if (!sfile.is_open()) {
        imprimirErrorEspecifico("Error, no se pueden leer los símbolos.");
        return false;
    }

    efile.open(erasureFilePath, std::ios::binary);
    if (!efile.is_open()) {
        imprimirErrorEspecifico("Error, no se pueden leer las borraduras.");
        return false;
    }

    outputFile.open(outputFilePath);
    if (!outputFile.is_open()) {
        imprimirErrorEspecifico("Error, no se puede abrir el archivo de salida.");
        return false;
    }

    return true;
}

std::ifstream& ManejadorDeArchivos::getSymbolFile() {
    return sfile;
}

std::ifstream& ManejadorDeArchivos::getErasureFile() {
    return efile;
}

std::ofstream& ManejadorDeArchivos::getOutputFile() {
    return outputFile;
}

void ManejadorDeArchivos::imprimirMensajeDeAyuda() {
    std::cerr << "Uso de: rsdecode -n n -r r -s symbolfile -e erasfile -o outputfile:" << std::endl;
    std::cerr << "Ejemplo: rsdecode -n numeroN -r numeroR -s etc/codificados.ech -e etc/codificados.eras -o output.sdat" << std::endl;
}

void ManejadorDeArchivos::imprimirErrorEspecifico(const std::string& mensaje) {
    std::cerr << "Error: " << mensaje << std::endl;
    std::cerr << "Ejecute sin argumentos para ver las instrucciones de uso." << std::endl;
}

bool ManejadorDeArchivos::procesarArgumentos(int &n, int &r, std::string &symbolfile, std::string &erasfile, std::string &outputfile, int argc, char *argv[]) {
    bool flagR = false;
    bool flagN = false;
    if (((argc == 2) && (std::string(argv[1]) == "-h")) || argc == 1){
        imprimirMensajeDeAyuda();
        return false; 
    }
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-n" && i + 1 < argc) {
            n = std::atoi(argv[i + 1]);
            flagN = true;
            i++; 
        } else if (arg == "-r" && i + 1 < argc) {
            r = std::atoi(argv[i + 1]);
            flagR = true;
            i++; 
        } else if (arg == "-s" && i + 1 < argc) {
            symbolfile = argv[i + 1];
            i++; 
        } else if (arg == "-e" && i + 1 < argc) {
            erasfile = argv[i + 1];
            i++; 
        } else if (arg == "-o" && i + 1 < argc) {
            outputfile = argv[i + 1];
            i++;
        } else {
            imprimirMensajeDeAyuda();
            return false; 
        }
    }

    if (!flagN || !flagR || n <= 0 || r < 0 || n <= r || n >= 256){
        imprimirErrorEspecifico("Error en los argumentos proporcionados.");
        return false;
    }

    return true;
}

void ManejadorDeArchivos::cerrarArchivos() {
    if (sfile.is_open()) sfile.close();
    if (efile.is_open()) efile.close();
    if (outputFile.is_open()) outputFile.close();
}