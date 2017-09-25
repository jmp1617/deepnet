///implimentations

#include "deepnet.h"
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>

void print_usage( void ){
    fprintf( stderr, "Usage: deepnet [mode] [# data] [size] [neurons.brain] [visualize]\n \
            \tmode: [t/a] either a 't' to specify training mode or 'a' to analyze\n \
            \t# data:  number of data for training or analysis\n \
            \tsize: the length of the largest data (including solution flag and length header if training).\n \
            \tneurons.brain: file to dump neurons to after training or load neurons for analysis\n \
            \tvisualize: [y/n] opengl visualization of training (slower)\n" );
}

int check_args( char* argv[] ){
    
    //check mode: if its not a single char or its not the char t or a
    if( strlen( argv[1] ) != 1 || !( argv[1][0] == 't' || argv[1][0] == 'a' ) ){
        fprintf( stderr, "Invalid mode: options are 't' to train or 'a' to analyze\n\n" );
        print_usage();
        return 1;
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

    if( strlen( argv[5] ) != 1 || !( argv[5][0] == 'y' || argv[5][0] == 'n' ) ){
        fprintf( stderr, "Invalid mode: y (yes) or n (no)\n\n" );
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
    
    Opengl gl = malloc( sizeof( Opengl_s ) );
    
    if( o->visualize == 'y' )
        init_opengl( (void*)gl );
    
    int num_prims = 4*(o->size-1)*6;
    double primatives[num_prims];

    generate_colors( o, primatives );
   
    for( int data = 0; data < o->numdata; data++ ){
#ifdef DEBUG
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
#endif
        //create train data input buffer based on largest size
        char input_buffer[o->size+1];
    
        //zero out the buffer
        for( int cell = 0; cell < o->size; cell++)
            input_buffer[ cell ] = '0';
        
        //get the input from stdin
        get_input( input_buffer, o->size );
#ifdef DEBUG
        printf("INPUT: ");
        for(int cell = 0; cell < o->size-1; cell++){
            printf("%c, ",input_buffer[cell]);
        }
#endif

        //extract data length, solution flag, and data
        TData d = malloc( sizeof( TData_s ) );
        extract_data( d, input_buffer, o->size );
#ifdef DEBUG
        printf("[%f]\n",d->solution);
#endif        
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
        vm( o->size-1, 4, L0, s->synapse0, L1 );
        sigmoid_vector( 4, L1, 0 );
        //L2
        vm( 4, 4, L1, s->synapse1, L2 );
        sigmoid_vector( 4, L2, 0 );
        //L3
        L3 = sigmoid( vv( L2, s->synapse2, 4 ), 0 );
#ifdef DEBUG
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
#endif
        //ERROR
        double L3_error = d->solution - L3;
#ifdef DEBUG
        printf("ERROR:\n\tlayer3: %f\n",L3_error);
#endif
        //backpropegation
        double L3_delta = L3_error * sigmoid( L3, 1 );
#ifdef DEBUG
        printf("\t\tlayer3 delta: %f\n",L3_delta);
#endif      
        double L2_error[4] = {0};
#ifdef DEBUG
        printf("\tlayer2: ");
#endif
        for( int cell = 0; cell < 4; cell++ ){
            L2_error[cell] = s->synapse2[cell] * L3_delta;
#ifdef DEBUG
            printf("%f ",L2_error[cell]);
#endif
        }
#ifdef DEBUG
        printf("\n\t\tlayer2 delta: ");
#endif
        double L2_delta[4] = {0};
        for( int cell = 0; cell < 4; cell++ ){
            L2_delta[cell] = L2_error[cell] * sigmoid( L2[cell], 1 );
#ifdef DEBUG
            printf("%f ",L2_delta[cell]);
#endif
        }
#ifdef DEBUG
        printf("\n\tlayer1: ");
#endif
        double L1_error[4] = {0};
        vm( 4, 4, L2_delta, s->synapse1, L1_error );
#ifdef DEBUG
        for( int cell = 0; cell < 4; cell++ ){
            printf("%f ", L1_error[cell]);
        }
        printf("\n\t\tlayer1 delta: ");
#endif
        double L1_delta[4] = {0};
        for( int cell = 0; cell < 4; cell++ ){
            L1_delta[cell] = L1_error[cell] * sigmoid( L1[cell], 1 );
#ifdef DEBUG
            printf("%f ",L1_delta[cell]);
#endif
        }
#ifdef DEBUG
        printf("\n");
#endif        
        //UPDATE synapses
        
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

        //prepare for opengl
        generate_primatives( s, o, primatives ); 
#ifdef DEBUG
        printf("\nPrimatives\n");
        for(int i = 0; i<num_prims; i+=6){
            printf("%f %f %f %f %f %f\n",primatives[i],primatives[i+1],primatives[i+2], \
                    primatives[i+3],primatives[i+4],primatives[i+5]);
        }
#endif
        if( o->visualize == 'y' )
            render_primatives( primatives, gl, num_prims );
        
        //cleanup
        free(d->data);
        free(d);
#ifndef DEBUG
        printf( "\033[2J" );
        fflush( stdout );
        printf( "\033[%d;%dH", 1, 0 );
        //progress bar
        printf("");
        //
        printf("%f%%\n",(double)data/o->numdata*100);
        printf("%d of %d\n",data,o->numdata);
#endif
    }
    free( gl );
    return 0;
}

int analyze_mode(Options o, SynStore s){
    
    printf("\n===SYNAPSES===\n");
    printf("Syn0:\n");
    for(int row = 0; row < o->size; row++){
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
    for( int data = 0; data < o->numdata; data++ ){
        printf("##########################\n#Analysis data number: %d #\n##########################\n", data);
                
        //create train data input buffer based on largest size
        char input_buffer[o->size+1];
    
        //zero out the buffer
        for( int cell = 0; cell < o->size; cell++)
            input_buffer[ cell ] = '0';
        
        //get the input from stdin
        get_input( input_buffer, o->size );
    
        //extract
        double data[o->size];

        printf("INPUT: ");
        for(int cell = 0; cell < o->size; cell++){
            printf("%c ",input_buffer[cell]);
            if( input_buffer[cell] == '1' )
                data[cell] = 1.0;
            else
                data[cell] = 0.0;
        }
        //FORWARD PROPEGATION
        //##Layers###
        //
        double L1[4] = {0};
        double L2[4] = {0};
        double L3 = 0;
        //
        //###########

        //begin training    
        //FORWARD PROPEGATION
        //L1
        vm( o->size, 4, data, s->synapse0, L1 );
        sigmoid_vector( 4, L1, 0 );
        //L2
        vm( 4, 4, L1, s->synapse1, L2 );
        sigmoid_vector( 4, L2, 0 );
        //L3
        L3 = sigmoid( vv( L2, s->synapse2, 4 ), 0 );

        printf("\nLAYERS:\n\tlayer0: ");
        for( int cell = 0; cell < o->size; cell++ ){
            printf("%f ", data[cell]);
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

        printf("RESULT: %f%%\n", L3*100);

        
    }
    return 0;
}

void export_brain( SynStore s, Options o ){    
    FILE* fp = fopen( o->file, "w" );
    
    //write synapse0
    for( int row = 0; row < o->size-1; row++ ){
        for( int col = 0; col < 4; col++ ){
            fprintf(fp, "%f\n",s->synapse0[row][col]);
        }
    }
    //syn1
    for( int row = 0; row < 4; row++ ){
        for( int col = 0; col < 4; col++ ){
            fprintf(fp, "%f\n",s->synapse1[row][col]);
        }
    }
    //syn2
    for( int cell = 0; cell < 4; cell++ ){
        fprintf(fp,"%f\n",s->synapse2[cell]);
    }

    fclose(fp);
}

void import_brain( SynStore s, Options o ){
    FILE* fp = fopen( o->file, "r" );
    char tempbuf[11] = {0};

    //read in synapse0
    for( int row = 0; row < o->size; row++ ){
        for( int col =0; col < 4; col++ ){
            fgets( tempbuf, 11, fp );
            s->synapse0[row][col] = atof(tempbuf);
        }
    }
    //read in synapse1
    for( int row = 0; row < 4; row++ ){
        for( int col =0; col < 4; col++ ){
            fgets( tempbuf, 11, fp );
            s->synapse1[row][col] = atof(tempbuf);
        }
    }
    //read in synapse2
    for( int cell = 0; cell < 4; cell++ ){
        fgets( tempbuf, 11, fp );
        s->synapse2[cell] = atof(tempbuf);
    }
    printf("Synapses loaded\n");
    
    fclose(fp);
}

void generate_primatives( SynStore s, Options o, double primatives[] ){
    //synapse 0 x, synapse 1 y, synapse 2 z
    //number of primatives = grid size of synapse 0 * 3
    //fill
    for( int prim = 0; prim < ( 4 * (o->size-1) ); prim++ ){
        primatives[prim*6] = prim_norm(s->synapse0[prim%(o->size-1)][prim/(o->size-1)]);
        primatives[prim*6+1] = prim_norm(s->synapse1[(prim%(4*4))%4][(prim%(4*4))/4]);
        primatives[prim*6+2] = prim_norm(s->synapse2[prim%4]); 
    }
}

void generate_colors( Options o, double primatives[] ){
    srand((unsigned) time(NULL));
    for( int prim = 0; prim < ( 4 * (o->size-1) ); prim++ ){
        primatives[prim*6+3] = (double)rand()/RAND_MAX;
        primatives[prim*6+4] = (double)rand()/RAND_MAX;
        primatives[prim*6+5] = (double)rand()/RAND_MAX;
    }
}


