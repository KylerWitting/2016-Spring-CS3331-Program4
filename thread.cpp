// ----------------------------------------------------------- 
// NAME : Kyler Witting                      User ID: kmwittin 
// DUE DATE : 04/01/2016
// PROGRAM ASSIGNMENT 4
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
//      This Program thread has the Santa, Elves, and Reindeer Threads
//      Each simulates one person or animal 
//      Uses Methods from thread-support to do the specified tasks 
// -----------------------------------------------------------

#include <stdio.h> //Used for sprintf
#include <string.h> //Used for strlen

#include "thread.h"
#include "thread-support.h"

//Santa's Semaphore if he is sleeping or not 
static Semaphore SantaSleep("SantaSleep", 0 );

//The Elf Semaphores 
static Semaphore ElfQueue("ElfQueue", 0);
static Semaphore ElfQuestion("ElfQuestion", 1);
static Semaphore SantaAnswer("SantaAnswer", 0);

//The Reindeer Semaphores
static Semaphore ReindeerReturn("ReindeerReturn", 0);
static Semaphore ReindeerAttached("ReindeerAttached", 0);
static Semaphore ReindeerTurn("ReindeerTurn", 1 );
static Semaphore ReindeerWait("ReindeerWait", 0);
static Semaphore Sleigh("Sleigh", 0);
static Semaphore ToyDeliver("ToyDeliver", 0);

//---------------------------------------------------------------//
//  Santa Class: 
//     constructor
//---------------------------------------------------------------//

    SantaThread::SantaThread( int elf, int rein, int retire )
:elfCount( elf ), reinCount( rein ), retireCount( retire )
{
    char buffer[1024];
    sprintf( buffer, "Santa thread starts\n" );
    write( 1, buffer, strlen( buffer ) );
};

//---------------------------------------------------------------//
//  Santa Thread Class body: 
//      Runs santa according to the specifications of the program
//      Santa retires before he exits 
//      When Santa Retires, all else exits too
//      He doesn't care about anything once he retires 
//---------------------------------------------------------------//

void SantaThread::ThreadFunc( ) {
    Thread::ThreadFunc();
    int i, j;
    char buffer[1024];

    isSantaRetired( 0 );

    ElfQueue.Signal();
    ElfQueue.Signal();
    ElfQueue.Signal();

    for( i = 0; i < reinCount; i++ )
        ReindeerReturn.Signal();

    //Infinite Loop for Santa until he retires  
    for( i=0;; ) {
        //Santa starts by sleeping
        //If the Sleep returns 1, it is the reindeers that woke up Santa
        //If the Sleep returns 0, it is the elves that woke up Santa 
        if( Sleep( reinCount, &SantaSleep ) ) { 
            //Gather Reindeer 
            for( j = 0; j < reinCount;j++ )
                ReindeerWait.Signal();

            //put on sleigh
            sprintf( buffer, "Santa is preparing sleighs\n" );
            write( 1, buffer, strlen( buffer ) );
            for( j = 0; j < reinCount;j++ )
                Sleigh.Signal();

            //Makes sure that the reindeer are attached 
            for( j = 0; j < reinCount;j++ )
                ReindeerAttached.Wait();

            //fly off 
            sprintf( buffer, "The team flies off (%d)!\n", i+1 );
            write( 1, buffer, strlen( buffer ) );
            Delay(); //Delivering Toys 

            //If it was the last run, Santa retires 
            if( i+1 >= retireCount ){
                isSantaRetired( 1 );
                fflush( stdout );
                sprintf( buffer, "After (%d) deliveries, Santa retires and is on vacation!\n", retireCount );
                write( 1, buffer, strlen( buffer ) );
                for( j = 0; j < reinCount;j++ )
                    ToyDeliver.Signal();
                Exit();
            }
            //He releases the reindeer until next year if he doesn't retire
            else {
                sprintf( buffer, "Santa Releases the Reindeer\n" );
                write( 1, buffer, strlen( buffer ) );
                ReleaseReindeer(); 
                for( j = 0; j < reinCount;j++ )
                    ToyDeliver.Signal();
                for( j = 0; j < reinCount; j++ )
                    ReindeerReturn.Signal();
            }
            i++;
        }
        //Elves are the ones with the question
        else{ 
            //Answers Question
            sprintf( buffer, "Santa is helping elves\n" );
            write( 1, buffer, strlen( buffer ) );
            Delay();
            SantaAnswers();


            //Releases Elves 
            //AskQuestion( 0, &SantaSleep );
            SantaAnswer.Signal();
            SantaAnswer.Signal();
            SantaAnswer.Signal();

            //Answering questions doesn't bring him closer to 
            //retirement, even though he wishes it does. 
        }
    } 
}

//---------------------------------------------------------------//
//  Elf Class: 
//     constructor
//---------------------------------------------------------------//

    ElfThread::ElfThread( int Number )
:num( Number )
{
    char buffer[1024];
    sprintf( buffer, "          Elf %d Starts.\n", Number+1 );
    write( 1, buffer, strlen( buffer ) );
};

//---------------------------------------------------------------//
//  Elf Thread Class body: 
//      Runs an Elf in accordance with the program specifications 
//      He makes a toy, then askes a question, then rests, repeat
//      The elf has lower priority than reindeer 
//      There has to be 3 elves before they can ask Santa a question 
//---------------------------------------------------------------//

void ElfThread::ThreadFunc( ) {
    Thread::ThreadFunc();
    char buffer[1024];

    for( ;; ) {
        //Make Toys 
        Delay();

        if ( isSantaRetired( 2 ) )
            Exit();
        else {
            sprintf( buffer, "          Elf %d has a problem\n", num+1 );
            write( 1, buffer, strlen( buffer ) );
        }

        //3 Elves allowed in Queue at a time 
        ElfQueue.Wait();
        //One Elf allowed to modify AskQuestion at a time 
        ElfQuestion.Wait();

        //If Santa is retired, he won't care about the question!
        if ( isSantaRetired( 2 ) )
            Exit();

        //If 3 elves are waiting
        if( AskQuestion( num+1, &SantaSleep ) ) {
            //Waits on Santa to answer

            SantaAnswer.Wait();    
            AskQuestion( -1, &SantaSleep );
            //Signal that the queue is empty now
            //Then releases the Question Semaphore 
            ElfQueue.Signal();
            ElfQueue.Signal();
            ElfQueue.Signal();
            ElfQuestion.Signal();
        }
        else { 
            //Releases question semaphore and then waits for santa to wake up
            ElfQuestion.Signal();
            SantaAnswer.Wait();    
        }
        //Problem Solved, Take a rest
        Delay();
    }
    Exit();
}

//---------------------------------------------------------------//
//  Reindeer Class: 
//     constructor
//---------------------------------------------------------------//

    ReindeerThread::ReindeerThread( int Number, int Max )
:num( Number ), reinMax( Max )
{
    char buffer[1024];
    sprintf( buffer, "     Reindeer %d Starts.\n", Number+1 );
    write( 1, buffer, strlen( buffer ) );
};

//---------------------------------------------------------------//
//  Reindeer Thread Class body:
//      Each thread is a reindeer that belongs to santa
//      they rest on vacation, then return, and then go back on vacation
//      ReindeerReturn allows 9 into the queue 
//      ReindeerTurn allows 1 to modify variables in methods 
//      ReindeerWait make the reindeer wait until all have returned 
//      Sleigh make all reindeer wait until the sleigh is ready
//      ToyDeliver makes all the reindeers deliver at the same time 
//---------------------------------------------------------------//

void ReindeerThread::ThreadFunc( ) {
    Thread::ThreadFunc();
    int i;

    for( ;; ) {
        //If Santa is retired, the reindeer goes and does its own thing 
        if ( isSantaRetired( 2 ) )
            break;

        //Reindeer is vacationing 
        Delay();
        //Makes sure no speedy threads get through ahread of schedule 
        ReindeerReturn.Wait();

        //Marks the return of a reindeer 
        ReindeerTurn.Wait(); 
        ReindeerBack( num+1 );
        ReindeerTurn.Signal();

        //Reindeer wait for all other reindeer to stop tanning 
        WaitOthers( num+1, reinMax, &SantaSleep, &ReindeerWait );

        //Waits for Santa to attach the reindeer to the sled 
        WaitSleigh( &Sleigh );

        //Makes sure that all the reindeer are attached to the sleigh
        ReindeerAttached.Signal(); 

        //All reindeer flyoff together, and deliver together
        Flyoff( &ToyDeliver );

        //Resting before going on vacation 
        Delay( );
    }
    Exit();
}
