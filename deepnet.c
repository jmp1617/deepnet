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

    //init synapse storage
    SynStore s = malloc( sizeof( SynStore_s ) );
    s->synapse0 = malloc( sizeof( double ) * o->size - 1 );
    init_syn0(s->synapse0, o->size - 1);

    //select runtype
    if( o->mode == 't' ) //train
        train_mode( o, s );
    else
        analyze_mode( o );
    
    //cleanup
    free( o->file );
    free( o );
    free( s->synapse0 );
    free( s );

    return 0;
}
