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
    fgets( input, size+2, stdin);
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

void init_syn2( double syn0[], int size ){
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
        printf("##########################\n#Training data number: %d #\n##########################\n", data);
        printf("===SYNAPSES===\n");
        printf("Syn0:\n");
        for(int row = 0; row < o->size - 1; row++){
            for(int col = 0; col < 4; col++){
                printf("%f ",s->synapse0[row][col]);
            }
            printf("\n");
        }
        printf("\nSyn1:\n");
        for(int row = 0; row < 4; row++){
            for(int col = 0; col < 4; col++){
                printf("%f ",s->synapse1[row][col]);
            }
            printf("\n");
        }
        printf("\nSyn2:\n");
        for(int cell = 0; cell < 4; cell++){
            printf("%f ",s->synapse2[cell]);
        }
        printf("\n=============\n\n");
        
        //create train data input buffer based on largest size
        char input_buffer[o->size+1];
    
        //zero out the buffer
        for( int cell = 0; cell < o->size; cell++)
            input_buffer[ cell ] = '0';
        
        //get the input from stdin
        get_input( input_buffer, o->size );
        
        printf("INPUT: ");
        for(int cell = 0; cell < o->size-1; cell++){
            printf("%c, ",input_buffer[cell]);
        }

        //extract data length, solution flag, and data
        TData d = malloc( sizeof( TData_s ) );
        extract_data( d, input_buffer, o->size );
        printf("[%f]\n",d->solution);
        
        //##Layers###
        //
        double* L0 = d->data;
        double L1[4] = {0};
        double L2[4] = {0};
        double L3 = 0;
        //
        //###########

        //begin training    
        //FORWARD PROPEGATION
        //L1
        vm( o->size, 4, L0, s->synapse0, L1 );
        sigmoid_vector( 4, L1, 0 );
        //L2
        vm( 4, 4, L1, s->synapse1, L2 );
        sigmoid_vector( 4, L2, 0 );
        //L3
        L3 = sigmoid( vv( L2, s->synapse2, 4 ), 0 );

        printf("LAYERS:\n\tlayer0: ");
        for( int cell = 0; cell < o->size - 1; cell++ ){
            printf("%f ", L0[cell]);
        }
        printf("\n");
        printf("\tlayer1: ");
        for( int cell = 0; cell < 4; cell++ ){
            printf("%f ", L1[cell]);
        }
        printf("\n");
        printf("\tlayer2: ");
        for( int cell = 0; cell < 4; cell++ ){
            printf("%f ", L2[cell]);
        }
        printf("\n");
        printf("\tlayer3: %f\n", L3);

        //ERROR
        double L3_error = d->solution - L3;
        printf("ERROR:\n\tlayer3: %f\n",L3_error);

        //backpropegation
        double L3_delta = L3_error * sigmoid( L3, 1 );
        printf("\t\tlayer3 delta: %f\n",L3_delta);
        
        double L2_error[4] = {0};
        printf("\tlayer2: ");
        for( int cell = 0; cell < 4; cell++ ){
            L2_error[cell] = s->synapse2[cell] * L3_delta;
            printf("%f ",L2_error[cell]);
        }
        printf("\n\t\tlayer2 delta: ");
        double L2_delta[4] = {0};
        for( int cell = 0; cell < 4; cell++ ){
            L2_delta[cell] = L2_error[cell] * sigmoid( L2[cell], 1 );
            printf("%f ",L2_delta[cell]);
        }
        printf("\n\tlayer1: ");

        double L1_error[4] = {0};
        vm( 4, 4, L2_delta, s->synapse1, L1_error );
        for( int cell = 0; cell < 4; cell++ ){
            printf("%f ", L1_error[cell]);
        }
        printf("\n\t\tlayer1 delta: ");
        double L1_delta[4] = {0};
        for( int cell = 0; cell < 4; cell++ ){
            L1_delta[cell] = L1_error[cell] * sigmoid( L1[cell], 1 );
            printf("%f ",L1_delta[cell]);
        }
        printf("\n");
        
        //UPDATE synapses
        double syn0_update[o->size-1][4];

        //syn2 
        for( int cell = 0; cell < 4; cell++ ){
            s->synapse2[cell] += L2[cell] * L3_delta;
        }
        //syn1
        for( int row = 0; row < 4; row++ ){
            for( int col = 0; col < 4; col++ ){
                s->synapse1[row][col] += L1[row] * L2_delta[col];
            }
        }
        //syn0
        for( int row = 0; row < o->size-1; row++ ){
            for( int col = 0; col < 4; col++){
                s->synapse0[row][col] += L0[row] * L1_delta[col];
            }
        }

        //cleanup
        free(d->data);
        free(d);
    }

}

int analyze_mode(Options o){

    return 0;
}
