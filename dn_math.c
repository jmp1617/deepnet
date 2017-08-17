///Math functions and routine implimentation

#include <stdlib.h>
#include <math.h>
#include "deepnet.h"

double sigmoid( double input, int derivative ){
    if( derivative )
        return input * ( 1 - input );
    return 1 / ( 1 + exp( -input ) );
}

double vv( double v1[], double v2[], int size ){
    double result = 0.0;
    for( int cell = 0; cell < size; cell++ ){
        result += v1[cell] * v2[cell];
    }
    return result;
}
