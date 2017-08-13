///Multipurpose deep learning neural network
///author: Jacob Potter (jmp1617)
//

#include "deepnet.h"

int main( int argc, char* argv[] ){
    
    if( argc != 5){
        fprintf( stderr, "Incorrect argument amount\n\n" );
        print_usage();
        return 1;
    }

    if( check_args( argv ) ){
        return 1;
    }

    return 0;
}
