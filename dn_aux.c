///implimentations`

#include "deepnet.h"
#include <string.h>
#include <ctype.h>
#include <time.h>

void print_usage( void ){
    fprintf( stderr, "Usage: deepnet [mode] [# data] [size] [neurons.brain]\n \
            \tmode: [t/a] either a 't' to specify training mode or 'a' to analyze\n \
            \t# data:  number of data for training or analysis\n \
            \tsize: the length of the largest data (including solution flag and length header if training).\n \
            \tneurons.brain: file to dump neurons to after training or load neurons for analysis\n" );
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

void get_input( char input[], int size ){
    fgets( input, size+1, stdin);
    char newline;//flush the newline
    fgets( &newline, size+1, stdin);
}

//char data extraction : TODO : raw binary extraction
void extract_data( TData d, char input_buffer[], int size ){
    d->data = malloc( sizeof( double ) * (size - 1 ) );
    for( int bit = 0; bit < size - 1; bit++){
        if( input_buffer[bit] == '1' )
            d->data[bit] = 1.0;
        else
            d->data[bit] = 0.0;
    }
    if( input_buffer[size-1] == '1' )
        d->solution = 1.0;
    else
        d->solution = 0.0;
}

void init_syn0( double syn0[], int size ){
    srand((unsigned) time(NULL));
    for( int cell = 0; cell < size; cell++ ){
        syn0[cell] = (double)rand()/RAND_MAX*2.0-1.0;
    }
}

void init_syn1( double** syn1, int rows, int cols ){
    srand((unsigned) time(NULL));
    for( int row = 0; row < rows; row++ ){
        for( int col = 0; col < cols; col++ ){
            syn1[row][col] = (double)rand()/RAND_MAX*2.0-1.0;
        }
    }
}

int train_mode(Options o, SynStore s){
    for( int data = 0; data < o->numdata; data++ ){
        
        printf("Training data number: %d\n", data);

        printf("Syn0: [");
        for(int cell = 0; cell < o->size-1; cell++){
            printf("%f, ",s->synapse0[cell]);
        }
        printf("]\n");
        
        //create train data input buffer based on largest size
        char input_buffer[o->size+1];
    
        //zero out the buffer
        for( int cell = 0; cell < o->size; cell++)
            input_buffer[ cell ] = 0;
    
        //get the input from stdin
        get_input( input_buffer, o->size );

        printf("Input: [");
        for(int cell = 0; cell < o->size-1; cell++){
            printf("%c, ",input_buffer[cell]);
        }

        //extract data length, solution flag, and data
        TData d = malloc( sizeof( TData_s ) );
        extract_data( d, input_buffer, o->size );
        printf("%f]\n",d->solution);

        //begin training    
        //forward propegation
        double layer1 = sigmoid( vv( s->synapse0, d->data, o->size - 1 ), 0 );
        printf("\tLayer1: %f\n", layer1);

        //get error
        double error = d->solution - layer1;
        printf("\tError: %f\n", error);

        //correct synapse
        //if neural net is really sure alter less but if its not alter more using sigmoid derivative
        double layer1_delta = error * sigmoid( layer1, 1 );
        printf("\tL1 Delta: %f\n", layer1_delta);
        for(int bit = 0; bit < o->size - 1; bit++)
            if( d->data[bit] > 0 )
                s->synapse0[bit] += layer1_delta;
        //cleanup
        free( d->data );
        free( d );
    }

}

int analyze_mode(Options o){
    return 0;
}
