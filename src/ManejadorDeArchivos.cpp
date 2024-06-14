#include "../include/ManejadorDeArchivos.h" // Asegúrate de que la ruta sea correcta

ManejadorDeArchivos::ManejadorDeArchivos() {
    // Implementación del constructor si es necesario
}

bool ManejadorDeArchivos::AbrirArchivos(const std::string& symbolFilePath, const std::string& erasureFilePath, const std::string& outputFilePath) {
    sfile.open(symbolFilePath, std::ios::binary);
    if (!sfile.is_open()) {
        ImprimirErrorEspecifico("Error, no se pueden leer los símbolos.");
        return false;
    }

    efile.open(erasureFilePath, std::ios::binary);
    if (!efile.is_open()) {
        ImprimirErrorEspecifico("Error, no se pueden leer las borraduras.");
        return false;
    }

    outputFile.open(outputFilePath);
    if (!outputFile.is_open()) {
        ImprimirErrorEspecifico("Error, no se puede abrir el archivo de salida.");
        return false;
    }

    return true;
}

std::ifstream& ManejadorDeArchivos::GetSymbolFile() {
    return sfile;
}

std::ifstream& ManejadorDeArchivos::GetErasureFile() {
    return efile;
}

std::ofstream& ManejadorDeArchivos::GetOutputFile() {
    return outputFile;
}

void ManejadorDeArchivos::ImprimirMensajeDeAyuda() {
    std::cerr << "Uso de: rsdecode -n n -r r -s symbolfile -e erasfile -o outputfile:" << std::endl;
    std::cerr << "Ejemplo: rsdecode -n numeroN -r numeroR -s etc/codificados.ech -e etc/codificados.eras -o output.sdat" << std::endl;
}

void ManejadorDeArchivos::ImprimirErrorEspecifico(const std::string& mensaje) {
    std::cerr << "Error: " << mensaje << std::endl;
    std::cerr << "Ejecute sin argumentos para ver las instrucciones de uso." << std::endl;
}

bool ManejadorDeArchivos::ProcesarArgumentos(int &n, int &r, std::string &symbolfile, std::string &erasfile, std::string &outputfile, int argc, char *argv[]) {
    bool flagR = false;
    bool flagN = false;
    if (((argc == 2) && (std::string(argv[1]) == "-h")) || argc == 1){
        ImprimirMensajeDeAyuda();
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
            ImprimirMensajeDeAyuda();
            return false; 
        }
    }

    if (!flagN || !flagR || n <= 0 || r < 0 || n <= r || n >= 256){
        ImprimirErrorEspecifico("Error en los argumentos proporcionados.");
        return false;
    }

    return true;
}

void ManejadorDeArchivos::CerrarArchivos() {
    if (sfile.is_open()) sfile.close();
    if (efile.is_open()) efile.close();
    if (outputFile.is_open()) outputFile.close();
}