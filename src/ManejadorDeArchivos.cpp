#include "../include/ManejadorDeArchivos.h" // Asegúrate de que la ruta sea correcta

ManejadorDeArchivos::ManejadorDeArchivos() {
}

bool ManejadorDeArchivos::abrirArchivos(const std::string& ruta_simbolos, const std::string& ruta_borraduras, const std::string& ruta_salida) {
    simbolos.open(ruta_simbolos, std::ios::binary);
    if (!simbolos.is_open()) {
        imprimirErrorEspecifico("Error, no se pueden leer los símbolos.");
        return false;
    }

    borraduras.open(ruta_borraduras, std::ios::binary);
    if (!borraduras.is_open()) {
        imprimirErrorEspecifico("Error, no se pueden leer las borraduras.");
        return false;
    }

    salida.open(ruta_salida,std::ofstream::binary);
    if (!salida.is_open()) {
        imprimirErrorEspecifico("Error, no se puede abrir el archivo de salida.");
        return false;
    }

    return true;
}

std::ifstream& ManejadorDeArchivos::obtener_simbolo() {
    return simbolos;
}

std::ifstream& ManejadorDeArchivos::obtener_borradura() {
    return borraduras;
}

std::ofstream& ManejadorDeArchivos::obtener_salida() {
    return salida;
}

void ManejadorDeArchivos::imprimirMensajeDeAyuda() {
    std::cerr << "Usted debe llamar al programa de esta forma -> rsdecode -n n -r r -s symbolfile -e erasimbolos -o salida:" << std::endl;
}

void ManejadorDeArchivos::imprimirErrorEspecifico(const std::string& mensaje) {
    std::cerr << "Error: " << mensaje << std::endl;
    std::cerr << "Ejecute sin argumentos para ver las instrucciones de uso." << std::endl;
}

bool ManejadorDeArchivos::verificar_indicador(const std::string &indicador, const std::string &valor, int &n, int &r, std::string &archivo_simbolos, std::string &archivo_borraduras, std::string &archivo_salida, bool &bandera_tamanio, bool &bandera_redundancia, bool &bandera_simbolo, bool &bandera_Borradura, bool &bandera_salida) {
    if (indicador == "-n") {
        n = std::atoi(valor.c_str());
        bandera_tamanio = true;
    } else if (indicador == "-r") {
        r = std::atoi(valor.c_str());
        bandera_redundancia = true;
    } else if (indicador == "-s") {
        archivo_simbolos = valor;
        bandera_simbolo = true;
    } else if (indicador == "-e") {
        archivo_borraduras = valor;
        bandera_Borradura = true;
    } else if (indicador == "-o") {
        archivo_salida = valor;
        bandera_salida = true;
    } else {
        imprimirErrorEspecifico("Argumento desconocido: " + indicador);
        return false;
    }
    return true;
}

bool ManejadorDeArchivos::procesarArgumentos(int &n, int &r, std::string &archivo_simbolos, std::string &archivo_borraduras, std::string &archivo_salida, int argc, char *argv[],int q) {
    int indice = 1;
    bool bandera_tamanio = false, bandera_redundancia = false, bandera_simbolo = false, bandera_Borradura = false, bandera_salida = false;

    if (argc == 1 || (argc == 2 && std::string(argv[1]) == "-h")) {
        imprimirMensajeDeAyuda();
        return false;
    }

    while (indice < argc - 1) { // Asegurarse de que haya un valor después de cada indicador
        std::string indicador = argv[indice];
        std::string valor = argv[indice + 1];

        if (!verificar_indicador(indicador, valor, n, r, archivo_simbolos, archivo_borraduras, archivo_salida, bandera_tamanio, bandera_redundancia, bandera_simbolo, bandera_Borradura, bandera_salida)) {
            return false; // Si hay un error, detener el procesamiento
        }
            indice += 2; // Avanzar al siguiente par de indicador-valor
    }

    // Verificar que todos los indicadores necesarios estén presentes y sean válidos
    if (!bandera_tamanio || !bandera_redundancia || !bandera_simbolo || !bandera_Borradura || !bandera_salida || n >= q || r < 0 || n <= 0 || n <= r) {
        imprimirErrorEspecifico("Error en los argumentos proporcionados. Recuerde que n debe ser mayor a 0, la redundancia r no puede ser negativa ni menor que n, y n debe ser menor a q (256). ");
        return false;
    }

    return true;
}

void ManejadorDeArchivos::cerrarArchivos() {
    if (simbolos.is_open()) simbolos.close();
    if (borraduras.is_open()) borraduras.close();
    if (salida.is_open()) salida.close();
}