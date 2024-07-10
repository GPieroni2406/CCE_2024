#include "../include/Calculos.h"


short Calculos::suma(int x, int y){
    return gfadd(x,y);
}

short Calculos::resta(int x, int y){
    return gfadd(x,y);
}

short Calculos::mult(int x, int y){
    return gfmul(x,y);
}

short Calculos::inv(int num){
    return gfinv(num);
}

short Calculos::division(int x, int y){
    return gfmul(x, gfinv(y));
}