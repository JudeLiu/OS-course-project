/****************************************************************************
        Description:
                This program is used to copy a file by using pipe technique.

        Algorithm:
                create a pipe, fork the process, every time in child process read a character
        from source file and send it to the write end of pipe,  in parent process read a character
        from the read end of pipe and put it to destination file, loop until  read an EOF.

        Sample input:
        -----------------------------------------------------
        ~/project1$ ./PipeCopy data.in data.out
        -----------------------------------------------------
        The sample input means that "data.in" is source file, and "data.out" is destination
        file.

        Sample Output:
        -----------------------------------------------------
        PipeCopy Time used:0.027800 miliisecond(s)
        -----------------------------------------------------
        Output indicates the execution time.

        Notice that you should change "data.in" before running this program.
****************************************************************************/
#include "header.h"
#define READ_END 0
#define WRITE_END 1
int main(int argc, char *argv[])
{
        clock_t  start,end;
	double elapsed;//for timing

        start = clock();

        char ch;
        FILE *src,*dest;
        src = fopen(argv[1],"r");
        dest = fopen(argv[2],"w+");

        int mypipe [2];
        if(pipe(mypipe)){
                fprintf(stderr,"Pipe Failed.\n");
                return -1;
        }
        pid_t pid;
        pid = fork();

        if(pid<0){
                //fork error
                fprintf(stderr,"Fork Failed.\n");
                return  -1;
        }
        else if(pid>0){
                /******************************************************
                Parent process. First close the write end of pipe,
                then read a character from the read end of pipe at a time,
                and put it to destination file, loop until read an EOF.
                ******************************************************/
                close(mypipe[WRITE_END]);
                while(read(mypipe[READ_END],&ch,1)>0)
                {
                        fputc(ch,dest);
                }
                close(mypipe[WRITE_END]);
                end = clock();
                elapsed = ((double)(end-start)) / CLOCKS_PER_SEC * 100;
                printf("PipeCopy Time used:%f miliisecond(s)\n",elapsed);
        }
        else{
                /******************************************************
                Child procss. First close the read end of pipe,
                the read a character from the source file and put it to
                the write end of pipe, loop until read an EOF.
                ******************************************************/
                close(mypipe[READ_END]);
                while((ch=fgetc(src)) != EOF)
                        write(mypipe[WRITE_END],&ch,1);
                close(mypipe[WRITE_END]);

        }
        fclose(src);
        fclose(dest);
        return 0;
}

