// ----------------------------------------------------------- 
// NAME : Kyler Witting                      User ID: kmwittin 
// DUE DATE : 04/01/2016
// PROGRAM ASSIGNMENT 4
// FILE NAME : thread-main.cpp
// PROGRAM PURPOSE :
//      This is a Santa Program. It simulates santa, his reindeer and his 
//      elves. 
// -----------------------------------------------------------

#include <stdio.h>

#include "thread.h"

//---------------------------------------------------------------//
//  main program 
//---------------------------------------------------------------//

int main( int argc, char *argv[] ) {
    //Program Variables
    int elfCount, reinCount, retireCount; 	
    //loop iterator
    int i;

    //checks arguments, if 0 sets to defaults
    //guarantees no negative numbers 
    if ( argc ==  4 ){ 
        elfCount    = atoi( argv[1] );
        reinCount   = atoi( argv[2] );
        retireCount = atoi( argv[3] );

        if( elfCount == 0 )
            elfCount = 7;
        if( reinCount == 0 )
            reinCount = 9;
        if( retireCount == 0 )
            retireCount = 5;
    }
    else { 
        fprintf( stdout, "Incorrect Number of arguments. Now exiting.\n" );
        return -1; 
    }

    //starts the santa thread
    SantaThread *sThread;
    sThread = new SantaThread( elfCount, reinCount, retireCount );
    sThread->Begin();

    //Starts the elf threads
    ElfThread *eThread[ elfCount ];
    for( i = 0; i < elfCount; i++ ) { 
        eThread[i] = new ElfThread( i );
        eThread[i]->Begin();
    }

    //Starts the reindeer threads
    ReindeerThread *rThread[ reinCount ];
    for( i = 0; i < reinCount; i++ ) { 
        rThread[i] = new ReindeerThread( i, reinCount );
        rThread[i]->Begin();
    }

    //Only Joins santa thread, the other ones exit 
    //Santa doesn't care after retiring 
    sThread->Join();
    Exit();
    return 0; 
}
