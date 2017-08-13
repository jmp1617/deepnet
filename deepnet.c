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

    //select runtype
    if( o->mode == 't' ) //train
        train_mode( o );
    else
        analyze_mode( o );
    
    //cleanup
    free( o );
    free( o->file );

    return 0;
}
