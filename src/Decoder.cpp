#include "../include/Decoder.h"
#include <algorithm>


vector<short>  Decoder::LeerBloque(ifstream & file, const int &n){        
    int bytesToRead = n; // Cada palabra en un octeto

    // Add offset to the read
    file.seekg(this->cantBloquesLeidos*bytesToRead, ios::beg);

    vector<short> buffer(n);
    file.read(reinterpret_cast<char*>(buffer.data()), bytesToRead);

    if (file.gcount() < static_cast<streamsize>(bytesToRead)) {
        return {};
    }

    transform(buffer.begin(), buffer.end(), buffer.begin(), [](short value) {
        return (value << 8) | ((value >> 8) & 0xFF);
    });


    return buffer;
}


vector<short> Decoder::EncontrarBorraduras(ifstream & file, const int &n) {  
    vector<short> indices;

    short bytesToRead = n;
    
    file.seekg(this->cantBloquesLeidos*bytesToRead,  ios::beg);

    char byte;
    short index = 0;
    while (file.read(&byte, 1) && (index<n)) {
        if (byte == 1) {
            indices.push_back(n-index-1);
        }
        index++;
    }
    return indices;
}



Decoder::Decoder(const int &n, const int &r){
    this->n = n;
    this->r = r;
    this->cantBloquesLeidos = 0;
    for (int i = 0; i < this->r; i++){
        this->xr.push_back(0);
    }
    this->xr.push_back(1);
}


void Decoder::IncrementoBloque(){
    this->cantBloquesLeidos++;
}


vector<short> Decoder::ObtenerPolinomioXR(){
    return this->xr;
}


vector<short> Decoder::LeerBloqueSimbolos(ifstream & symbolfile){
    vector<short> res = this->LeerBloque(symbolfile, this->n);
    return res;
}


vector<short> Decoder::LeerIndiceSimbolos(ifstream & erasfile){
    vector<short> res = this->EncontrarBorraduras(erasfile, this->n);
    return res;
}


vector<vector<short>> Decoder::ObtenerMatrizChequeo(){
    vector<vector<short>> res;
    for (int i = 0; i < this->r; i++){
        vector<short> row;
        for (int j = 0; j < this->n; j++){
            row.push_back(_gfalog[((i+1)*j)%(255)]);
        }
        res.push_back(row);
    }
    return res;
}



vector<short> Decoder::CalcSindromePolinomial(const vector<short> &symbols, const vector<vector<short>> &H){       
    vector<short> res;
        
    for (int i = 0; i < this->r; i++){    
        short sum = 0;
        for (int j = 0; j < this->n; j++){
            sum = calc.Suma(sum,calc.Mult(symbols[this->n - j- 1],H[i][j]));
        }
        res.push_back(sum);
    }

    // remove leading zeros
    while (!res.empty() && res.back() == 0) {
        res.pop_back();
    }
    
    return res;
}


vector<short> Decoder::CalcularPolBorraduras(const vector<short> &indexErasures){
    vector<short> res(1,1);     
    for (short indice : indexErasures){
        vector<short> aux = {1, calc.Resta(0,_gfalog[indice])};
        res = pol.Multiplicar_Polinomios(res, aux);
    } 
    return res;
}


bool Decoder::BloqIncorregible(vector<short> indexErasures){
    this->rho = (int)indexErasures.size();
    return this->rho > this->r;
}


vector<short> Decoder::CalcularSindromeModificado(const vector<short> &syndromePolynomial,const  vector<short> &erasureLocatorPolynomial){
    vector<short> res = pol.Multiplicar_Polinomios(syndromePolynomial, erasureLocatorPolynomial);
    return pol.Dividir_Polinomio(res, this->xr).second;
}


pair<vector<short>, vector<short>> Decoder::Euclides(const vector<short> &p1,const vector<short> &p2) {
    double stop = (this->r + this->rho) / 2.0;
    vector<short> r0 = p2;
    vector<short> r1 = p1;
    vector<short> t0 = {0};
    vector<short> t1 = {1};
    while (!(r1.size() - 1 < stop && stop <= r0.size() - 1)) {
        pair <vector<short>, vector<short>> res = pol.Dividir_Polinomio(r0, r1);
        vector<short> quotient = res.first;
        r0 = r1;
        r1 = res.second;

        vector<short> temp = t0;
        t0 = t1;
        t1 = pol.Restar_Polinomios(temp, pol.Multiplicar_Polinomios(quotient, t1));
    }
    return make_pair(t1,r1);
}



vector<short> Decoder::ObtenerPolinomioLocalizador(const vector<short> &erasureLocatorPoly,const vector<short> &calculateErrorLocatorPoly){
    return pol.Multiplicar_Polinomios(erasureLocatorPoly, calculateErrorLocatorPoly);
}


vector<short> Decoder::RaicesNoNulasChien(const vector<short> &poli){
    vector<short> index = {};        
    int amountRoots = 0;    
    int degree = (int)poli.size() - 1;  

     if (!poli.empty()){
         for (int i=1; i < 256 && amountRoots < degree; i++){
            if (pol.Evaluar_Polinomio(poli, i) == 0){
                index.push_back(i);
                amountRoots++;
            }
        }
        }
    if (amountRoots == degree){
        return index;
    }
    else {
        return {};
    }

    
}


pair<vector<short>, vector<short>> Decoder::Forneys(const vector<short> &errorLocatorPolyRootsIndexes,const  vector<short> &errorLocatorPoly,const vector<short> &errorEvaluatorPolynomial){
    vector<short> errorValues;
    vector<short> errorLocations;
    vector<short> derivative = pol.Derivar_Polinomio(errorLocatorPoly);

    if (errorLocatorPolyRootsIndexes.empty()){
        return make_pair(errorValues, errorLocations);
    }
    
    for (short i : errorLocatorPolyRootsIndexes){
        errorValues.push_back(calc.Mult(calc.Obtener_q(), calc.Division(pol.Evaluar_Polinomio(errorEvaluatorPolynomial, i), pol.Evaluar_Polinomio(derivative, i))));
        errorLocations.push_back((calc.Obtener_q() - i) % calc.Obtener_q());
    }
    
    return make_pair(errorValues, errorLocations);
}

vector<short> Decoder::Decodificar(const vector<short> &receivedWord,const vector<short> &errorLocations,const vector<short> &errorValues){
    int j=0;
    vector<short> decodedWord = receivedWord;
    for (auto i: errorLocations){
        if (i < (int)receivedWord.size()){
            decodedWord[this->n - i - 1] = calc.Resta(receivedWord[this->n - i - 1], errorValues[j]);
        }
        j++;
    }

    return decodedWord;
}

