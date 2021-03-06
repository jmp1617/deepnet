//Math functions and routine implimentation

#include <stdlib.h>
#include <math.h>
#include "deepnet.h"

double sigmoid( double input, int derivative ){
    if( derivative )
        return input * ( 1 - input );
    return 1 / ( 1 + exp( -input ) );
}

void sigmoid_vector( int size, double v[], int dir ){
    for( int cell = 0; cell < size; cell++ ){
        v[cell] = sigmoid( v[cell], dir );
    }
}

void sigmoid_matrix( int rows, int cols, double m[][cols], int dir ){
    for( int row = 0; row < rows; row++ ){
        for( int col = 0; col < cols; col++ ){
            m[row][col] = sigmoid( m[row][col], dir );
        }
    }
}

double prim_norm(double p){
    return 2* ( ( 1 / ( 1 + exp( (-1.0/10.0) * p ) ) ) - 0.5 );
}

double vv( double v1[], double v2[], int size ){
    double result = 0.0;
    for( int cell = 0; cell < size; cell++ ){
        result += v1[cell] * v2[cell];
    }
    return result;
}

void vm( int center, int right, double v1[], double** m1, double result[] ){
    for( int row = 0; row < right; row++ ){
        for( int col = 0; col < center; col++ ){
            result[row] += m1[row][col] * v1[col];
        }
    }
}

void mm( int left, int center, int right, double** m1, double** m2, double result[][right] ){
    for( int row = 0; row < left; row++ ){
        for( int col = 0; col < right; col++ ){
            for( int inner = 0; inner < center; inner++ ){
                result[row][col] += m1[row][inner] * m2[inner][col];
            }
        }
    }
}
