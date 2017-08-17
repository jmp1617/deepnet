#ifndef DEEPNET_H
#define DEEPNET_H

#include <stdlib.h>
#include <stdio.h>

//structure to hold parameter results
typedef struct{
    char mode;
    int numdata;
    int size;
    char* file;
} Options_s;

typedef Options_s* Options;

//struct to represent training data
typedef struct{
    int solution;
    int* data;
} TData_s;

typedef TData_s* TData;

//prints the usage message
void print_usage( void );

//check to make sure all of the arguments are valid
int check_args( char* argv[] );

//train routine
int train_mode(Options o);

//analyze mode
int analyze_mode(Options o);

//sigmoid function
double sigmoid( double input, int derivative );

//get input from stdin
void get_input( char input[], int size );

//extract size and solution
void extract_data( TData d, char input_buffer[], int size );

//fill the synapse with random numbers mean 0
void init_syn0( double syn0[], int size );

#endif
