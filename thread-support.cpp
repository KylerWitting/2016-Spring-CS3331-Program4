// ----------------------------------------------------------- 
// NAME : Kyler Witting                      User ID: kmwittin 
// DUE DATE : 04/01/2016
// PROGRAM ASSIGNMENT r
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
//      Class implemention file for the thread-support
//      Methods are called by thread.cpp
// -----------------------------------------------------------

#include <stdio.h> //for sprintf 
#include <string.h> //strlen 
#include "thread-support.h"

//Elf Variables
static int elfCount = 0;
static int elves[4];

//Reindeer Variables 
static int reinIn = 0;

//Tells if Santa is retiring 
static int santaRetired = 0;

// ­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­  
// FUNCTION AskQuestion : (AskQuestion)                           
//     This is for when the elves ask a question                             
// PARAMETER USAGE :                                            
//    elfNum- The name of elf that asked the question
//		sleep- the Semaphore that will wake up santa when there is 3 elves                 
// FUNCTION CALLED :                                            
//    sprintf, write, strlen, Signal           
// ­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­
int AskQuestion( int elfNum, Semaphore *sleep ) { 
    char buffer[1024];
   //elfNum = -1 is the signal the the elves are getting back to work  
    if ( elfNum == -1 ) {
        sprintf( buffer, "          Elves %d, %d, %d return to work\n"
                , elves[0], elves[1], elves[2] );   
        write( 1, buffer, strlen( buffer ) );
        return 1;
    }
    
    //Gets array of elves currently waiting 
    elves[elfCount] = elfNum;
    elfCount++;

    //If less than 3, not important enough for santa
    if( elfCount < 3 ) {
        return 0;
    }
    //Finally important enough for Santa, go wake him up!
    else if( elfCount == 3 ) {
        sleep->Signal();
        sprintf( buffer, "          Elves %d, %d, %d wake up the Santa\n"
                , elves[0], elves[1], elves[2] );   
        write( 1, buffer, strlen( buffer ) );
        return 1;
    }
}

// ­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­  
// FUNCTION  ReindeerBack : (ReindeerBack)                           
//     Signals that the reindeer returned from vacation
//		Iterates reindeer count                             
// PARAMETER USAGE :                                            
//    num- The number of the reindeer that returned                 
// FUNCTION CALLED :                                            
//   sprintf, strlen           
// ­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­
void ReindeerBack( int num ) { 
    char buffer[1024];
    //Reindeer returns 
    reinIn++;
    sprintf( buffer, "     Reindeer %d returns\n", num );
    write( 1, buffer, strlen(buffer) );
}

// ­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­  
// FUNCTION  WaitOthers : (WaitOthers)                           
//     All reindeers will wait for the all the reindeers to return 
//		The last reindeer signals Santa they are ready to deliver packages                              
// PARAMETER USAGE :                                            
//    num- The name of the reindeer calling the method 
//		max- The max number of reindeer 
//		sleep- The Semaphore to call Santa 
//		ReindeerWait - The reindeer wait here for Santa gather them                
// FUNCTION CALLED :                                            
//   Signal, sprintf, strlen, Wait          
// ­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­
void WaitOthers( int num, int max, Semaphore *sleep, Semaphore *ReindeerWait ) {
    char buffer[1024];
    //Last reindeer talks to santa 
    if( reinIn >= max ) {
        sleep->Signal();
        sprintf( buffer, "     The last reindeer %d wakes up Santa\n", num );
        write( 1, buffer, strlen(buffer) );
    }
    ReindeerWait->Wait();
}

// ­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­  
// FUNCTION  WaitSleigh : (WaitSleigh)                           
//     All Reindeer wait here to be attached to the sleigh                              
// PARAMETER USAGE :                                            
//    Sleigh- The sleigh the reindeer are waiting for                 
// FUNCTION CALLED :                                            
//    Wait           
// ­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­
void WaitSleigh( Semaphore *Sleigh ) { 
    Sleigh->Wait();
}

// ­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­  
// FUNCTION  Flyoff : (Flyoff)                           
//     All Reindeer wait here to Flyoff with santa                               
// PARAMETER USAGE :                                            
//    ToyDeliver- The Reindeer are delivering toys with santa                  
// FUNCTION CALLED :                                            
//    Wait           
// ­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­
void Flyoff( Semaphore *ToyDeliver ) {
    ToyDeliver->Wait();
}

// ­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­  
// FUNCTION  Sleep : (Sleep)                           
//     Santa Sleeps here until it is signaled to wake up                              
// PARAMETER USAGE :                                            
//    reinTotal- Used to check when all the reindeer are in  
//		sleep- The semaphore that signals santa to wake up               
// FUNCTION CALLED :                                            
//    Wait           
// ­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­
int Sleep( int reinTotal, Semaphore *sleep ) { 
    sleep->Wait();
    if( reinIn >= reinTotal )
        return 1;
    if ( elfCount == 3 ) 
        return 0; 
}

// ­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­  
// FUNCTION  isSantaRetired : (isSantaRetired)                           
//     Checking to see if Santa is retired, or setting if he is or not                             
// PARAMETER USAGE :                                            
//    n- 2= checking to see if he is, 0/1= set if he is retired to that
// FUNCTION CALLED :                                                     
// ­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­
int isSantaRetired( int n ) {
    //if n is 2, they are checking status 
    if( n != 2 )
        santaRetired = n;
    return santaRetired;
}

// ­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­  
// FUNCTION  ReleaseReindeer : (ReleaseReindeer)                           
//     Release all the reindeers                             
// PARAMETER USAGE :                                            
//    N/A             
// FUNCTION CALLED :                                                    
// ­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­
void ReleaseReindeer( ) { 
    reinIn = 0;
}

// ­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­  
// FUNCTION  SantaAnswers : (SantaAnswers)                           
//     Santa Answers the question asked by the elves                              
// PARAMETER USAGE :                                            
//    N/A              
// FUNCTION CALLED :                                            
//    sprintf, strlen        
// ­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­­
void SantaAnswers( ) {
    char buffer[1024];
    sprintf( buffer, "Santa answers the question posted by elves %d, %d, %d\n"
            , elves[0], elves[1], elves[2] );   
    write( 1, buffer, strlen(buffer) );
    elfCount = 0;
}
