#ifndef DEEPNET_H
#define DEEPNET_H

#include <stdlib.h>
#include <stdio.h>
#include "render.h"

//structure to hold parameter results
typedef struct{
    char mode;
    int numdata;
    int size;
    char* file;
    char visualize;
} Options_s;

typedef Options_s* Options;

//struct to represent training data
typedef struct{
    double solution;
    double* data;
} TData_s;

typedef TData_s* TData;

//synapse storage
typedef struct{
    double* synapse2;
    double** synapse0;
    double** synapse1;
} SynStore_s;

typedef SynStore_s* SynStore;

//prints the usage message
void print_usage( void );

//check to make sure all of the arguments are valid
int check_args( char* argv[] );

//train routine
int train_mode(Options o, SynStore s);

//analyze mode
int analyze_mode(Options o, SynStore s);

//get input from stdin
void get_input( char input[], int size );

//extract size and solution
void extract_data( TData d, char input_buffer[], int size );

//fill the synapses with random numbers mean 0
void init_syn2( double syn0[], int size );
void init_syn1( double** syn1, int rows, int cols );

///Math
//sigmoid function
double sigmoid( double input, int derivative );
void sigmoid_vector( int size, double v[], int dir );
void sigmoid_matrix( int rows, int cols, double m[][cols], int dir );
//primatives normalization
double prim_norm(double p);
//dot products
double vv( double v1[], double v2[], int size );
//[left, center]x[center, right] = [left, right]
void vm( int center, int right, double v1[], double** m1, double result[] );
void mm( int left, int center, int right, double** m1, double** m2, double result[][right] );

//export the brain to specified file
void export_brain( SynStore s, Options o );

//import the brain from file
void import_brain( SynStore s, Options o );

//generate primatives for opengl
void generate_primatives( SynStore s, Options o, double primatives[] );

//generate random colors for synapse streams
void generate_colors( Options o, double primatives[] );

#endif
