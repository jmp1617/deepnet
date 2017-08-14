///Math functions and routine implimentation

#include <stdlib.h>
#include <math.h>
#include "deepnet.h"

double sigmoid( double input, int derivative ){
    if( derivative )
        return input * ( 1 - input );
    return 1 / ( 1 + exp( input ) );
}
