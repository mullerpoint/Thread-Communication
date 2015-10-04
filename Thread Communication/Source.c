//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Gary Muller
// Fall 2015
// CS 420 Assignment 1
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//A program that will spawn a seccond thread and then attempt communication 
//it will utilize pthread_create() and the concept of shared address space to communicate between two threads
//***specific implementations of these functions mean that this code will likely only run on RHEL***
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Header files - Standard libraries and classes
#include <stdio.h> //standard IO include
#include <sys/types.h> //typedef for unistd
#include <unistd.h> //process control functions
#include <pthread.h> //thread control functions
#include <errno.h> //error handling
#include <strings.h> //strerror()
//


//Global Variables and Defines
int FLAG = 0; //spin wait flag
//

//Function Prototypes
void* threadProgram(void *);
//

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//Main Declaration
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void* main(int argv, char* argc[])
{
	//create variables
	pthread_t tid; //thread identifier
	int result; //result return for pthread_create()

	//print initial data
	printf("Main: PID is %ul\n", getpid());

	//spawn new thread
	result = pthread_create(&tid, NULL, threadProgram, NULL);
	/*
	tid - thread name return,
	NULL - no atributes defined,
	threadProgram - name of the function for thread 2 to run,
	NULL - no arguments for the function
	*/


	//check results and complete program accordingly
	if (result == 0)//successful creation of a seccond thread
	{
		//on success print the data about thread 2 from thread 1
		printf("Main: Successfully created a thread with TID of %ul\n", tid);

		//prompt user for input of a non-zero integer
		printf("Main: Enter a non-zero integer so that the other thread can get out of its spin loop:\n");


		do
		{
			scanf(FLAG, "%d");
		} while (FLAG == 0);
		
		while (FLAG != 0);
		

	}
	else //unsucessful creation of a seccond thread
	{
		//print out the error returned by pthread_create()
		printf(" ERR: error at line %d, error: %s", __LINE__, strerror(result));
	}

	/*Make sure the thread 2 has finished before getting to the end of main
	and killing the thread before it gets to execute*/
	pthread_join(tid, NULL);

	// final print out to end program
	printf("Main: Second thread has re-zeroed the flag, so I'm done.\n");
}//main





void* threadProgram(void * nothing)
{
	//print required info about thread 2
	printf("\tSecond thread: My TID is %ul, still in process #%ul\n", pthread_self(), getpid());

	//print data about what the thread is doing
	printf("\tSecond thread: I'm about to spin until main enters a non-zero value into the flag\n");

	//spin until the flag changes
	while(FLAG != 0);

	//print result of flag change
	printf("Second thread: Main set the flag to %d; I'll set it back to zero and then I'm done.", FLAG);

	FLAG = 0;
}