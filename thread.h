// ----------------------------------------------------------- 
// NAME : Kyler Witting                      User ID: kmwittin 
// DUE DATE : 04/01/2016
// PROGRAM ASSIGNMENT 4
// FILE NAME : thread.h
// PROGRAM PURPOSE :
//      Head file for class Thread classes definitions
// -----------------------------------------------------------

#ifndef _THREAD_H
#define _THREAD_H

// header file
#include "ThreadClass.h"

//---------------------------------------------------------------//
//  Santa Thread Class: 
//      Contstuctor for the Santa Thread      
//---------------------------------------------------------------//
class SantaThread : public Thread
{
    public:
        // constructor
        SantaThread( int elf, int rein, int retire );

    private:
        void ThreadFunc();  // thread body
        int elfCount;	
        int reinCount;
        int retireCount;
        int i;
};

//---------------------------------------------------------------//
//  Elf Thread Class: 
//      Contstuctor for the Elf Thread 
//---------------------------------------------------------------//
class ElfThread : public Thread
{
    public:
        // constructor
        ElfThread( int Number );

    private:
        void ThreadFunc();  // thread body
        int num;
};

//---------------------------------------------------------------//
//  Santa Thread Class: 
//       Contstuctor for the Reindeer Thread
//---------------------------------------------------------------//
class ReindeerThread : public Thread
{
    public:
        // constructor
        ReindeerThread( int Number, int Max );

    private:
        void ThreadFunc();  // thread body
        int num;
        int reinMax;
};
#endif
