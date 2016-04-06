 /****************************************************************************
        Description:
                This program is used to read in a file and copy the content of this file in child process.
        Algorithm: read a character from source file, then fork() the process. Execute MyCopy.cpp
        in child process, wait child process in parent process.

        Sample input:
        -----------------------------------------------------
        ~/project1$ ./ForkCopy data.in data.out
        -----------------------------------------------------
        The sample input means that "data.in" is source file, and "data.out" is destination
        file.

        Sample Output:
        -----------------------------------------------------
        MyCopy Time used:0.033300 miliisecond(s)
        ForkCopy Time used:0.001400 miliisecond(s)
        -----------------------------------------------------
        Output indicates the execution time of each process.

        Notice that you should change "data.in" before running this program.
****************************************************************************/
#include "header.h"

int main(int argc, char *argv[])
{
	pid_t ForkPID;
	ForkPID = fork();

        clock_t  start,end;//for timing
	double elapsed;

        start = clock();
	switch(ForkPID)
	{
		//-1,fork failure
		case -1:
			printf("Error:Faild to fork.\n");break;
		//0,this is the child process, execute MyCopy1
		case 0:
		execlp("./MyCopy",argv[0],argv[1],argv[2],NULL);
		break;
		//>0,parent process and the PID is the child's PID
		default:
		wait(NULL);
                end = clock();
                elapsed = ((double)(end-start)) / CLOCKS_PER_SEC * 100;
                printf("ForkCopy Time used:%f miliisecond(s)\n",elapsed);
		break;
	}

	return 1;

}
