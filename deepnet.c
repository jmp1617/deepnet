///Multipurpose deep learning neural network
///author: Jacob Potter (jmp1617)
//

#include "deepnet.h"
#include <string.h>

int main( int argc, char* argv[] ){
    
    if( argc != 5){
        fprintf( stderr, "Incorrect argument amount\n\n" );
        print_usage();
        return 1;
    }

    if( check_args( argv ) ){
        return 1;
    }

    //set options
    Options o = malloc( sizeof( Options_s ) );
    
    o->mode = argv[1][0];
    o->numdata = strtol( argv[2], NULL, 10 );
    o->size = strtol( argv[3], NULL, 10 );
    o->file = malloc( sizeof( char ) * 50 );
    memcpy( o->file, argv[4], strlen( argv[4] ) + 1 );

    //prepare synapse storage
    SynStore s = malloc( sizeof( SynStore_s ) );
    s->synapse2 = malloc( sizeof( double ) * 4 );
    //init synapse 0
    s->synapse0 = malloc( sizeof( double* ) * o->size );
    s->synapse0[0] = malloc( sizeof( double ) * o->size * 4 );
    for( int i = 0; i < o->size; i++ )
        s->synapse0[i] = ( *s->synapse0 + 4 * i );
    //init synapse 1
    s->synapse1 = malloc( sizeof( double*) * 4 );
    s->synapse1[0] = malloc( sizeof( double ) * 16 );
    for( int i = 0; i < 4; i++ )
        s->synapse1[i] = ( *s->synapse1 + 4 * i );

    //select runtype
    if( o->mode == 't' ){ //train
        //init synapses
        init_syn2( s->synapse2, 4 );
        init_syn1( s->synapse0, o->size - 1, 4 );
        init_syn1( s->synapse1, 4, 4 );
        train_mode( o, s );
        
        printf( "\033[%d;%dH", 1, 0 );
        printf("%f%%\n",(double)100);
        printf("%d of %d\n",o->numdata,o->numdata);

        printf("\nDONE TRAINING\n");
        export_brain( s, o );
    }
    else{
        import_brain( s, o );
        analyze_mode( o, s );
    }
    
    //cleanup
    free( o->file );
    free( o );
    free( s->synapse2 );
    free( s->synapse1[0] );
    free( s->synapse1 );
    free( s->synapse0[0] );
    free( s->synapse0 );
    free( s );

    return 0;
}
