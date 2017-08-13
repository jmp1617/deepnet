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

//prints the usage message
void print_usage( void );

//check to make sure all of the arguments are valid
int check_args( char* argv[] );

//train routine
int train_mode(Options o);

//analyze mode
int analyze_mode(Options o);

#endif
