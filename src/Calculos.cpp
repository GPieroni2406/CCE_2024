#include "../include/Calculos.h"


short Calculos::Suma(int x, int y){
    return gfadd(x,y);
}

short Calculos::Resta(int x, int y){
    return gfadd(x,y);
}

short Calculos::Mult(int x, int y){
    return gfmul(x,y);
}

short Calculos::Inv(int num){
    return gfinv(num);
}

short Calculos::Division(int x, int y){
    return gfmul(x, gfinv(y));
}

int Calculos::Obtener_q(){
    return this->q;
}