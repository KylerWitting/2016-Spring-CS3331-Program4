// ----------------------------------------------------------- 
// NAME : Kyler Witting                      User ID: kmwittin 
// DUE DATE : 04/01/2016
// PROGRAM ASSIGNMENT 4
// FILE NAME : thread-support.h
// PROGRAM PURPOSE :
//      Head file  
//      Helper Methods for Santa Simulator 
// -----------------------------------------------------------

#ifndef _THREAD_SUPPORT_H
#define _THREAD_SUPPORT_H

// header file
#include "ThreadClass.h"

int AskQuestion( int elfNum, Semaphore *sleep );
void ReindeerBack( int num );
void WaitOthers( int num, int max, Semaphore *sleep , Semaphore *Sleigh );
void WaitSleigh( Semaphore *ReindeerWait );
void Flyoff( Semaphore *ToyDeliver );
int Sleep( int reinTotal, Semaphore *sleep);

int  isSantaRetired( int n );
void ReleaseReindeer( void );
void SantaAnswers( void );
#endif
