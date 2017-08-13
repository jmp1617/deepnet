///implimentations`

#include "deepnet.h"
#include <string.h>
#include <ctype.h>

void print_usage( void ){
    fprintf( stderr, "Usage: deepnet [mode] [# data] [size] [neurons.brain]\n \
            \tmode: [t/a] either a 't' to specify training mode or 'a' to analyze\n \
            \t# data:  number of data for training or analysis\n \
            \tsize: the size in bits of the largest data (including solution flag and length byte).\n \
            \tneurons.brain: file to dump neurons to after training or load neurons for analysis.\n" );
}

int check_args( char* argv[] ){
    
    //check mode: if its not a single char or its not the char t or a
    if( strlen( argv[1] ) != 1 || !( argv[1][0] == 't' || argv[1][0] == 'a' ) ){
        fprintf( stderr, "Invalid mode: options are 't' to train or 'a' to analyze\n\n" );
        print_usage();
        //return 1;
    }

    //check # data: if it is a number greater than 0
    for( unsigned int c = 0; c < strlen( argv[2] ); c++ ){
        if( !isdigit( argv[2][c] ) ){
            fprintf( stderr, "# data parameter must be a number greater than 0\n\n" );
            print_usage();
            return 1;
        }
    }
    if( strtol( argv[2], NULL, 10 ) < 1 ){
        fprintf( stderr, "# data must be greater than 0\n\n" );
        print_usage();
        return 1;
    }

    //check size: if it is a number greater than 0
    for( unsigned int c = 0; c < strlen( argv[3] ); c++ ){
        if( !isdigit( argv[3][c] ) ){
            fprintf( stderr, "size parameter must be a number greater than 0\n\n" );
            print_usage();
            return 1;
        }
    }
    if( strtol( argv[3], NULL, 10 ) < 1 ){
        fprintf( stderr, "size must be greater than 0\n\n" );
        print_usage();
        return 1;
    }

    //check file: make sure filename is less than 50 chars long
    if( strlen( argv[4] ) > 49 ){
        fprintf( stderr, "File name cannot exceed 50 characters\n\n" );
        print_usage();
        return 1;
    }

    return 0;
}

int train_mode(Options o){
    return 0;
}

int analyze_mode(Options o){
    return 0;
}   
