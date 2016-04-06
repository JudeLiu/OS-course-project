/****************************************************************************
        Description:
               This program is a mini‐shell which is able to execute command lines with
        commands connected by pipes.

        Algorithm:
                In the main loop, we get a line of command at a time, then invoke parseLine()
        and seperate the command by "|", next check the commands already seperated to
        see if this command is the same as the previous unseperated command, if it is the
        same, it indicates that there is no pipe in this command, we can handle this command
        as "MoreShell" has done; if not, there exists pipes. What we have to do are as follows:

        1. invoke parseLine() agian and seperate the command by "|", save the former part of
        it in char *command_arrayA[], the latter part in char *command_arrayB[];
        2. create a pipe, which in this program is int myPipe[2];
        3. fork() two child processes;
        4. in childA handles the former part of the whole command, close the read end of pipe
        and standard output, dup2() the write pipe onto standard output and then close the write
        end, finally invoke execvp(commandA[0],commandA);
        5. in childB handles the latter part of the whole command using the similar technique like
        childA
        6. in parent process, close both end of pipe and wait two child processes;

        If the steps above finished, continue the next loop.


        Sample input and output:
        ----------------------------------------------------
        MyShell>date
        2015年 04月 20日 星期一 13:10:37 CST
        MyShell>date|wc
        1       6      43
        ----------------------------------------------------
****************************************************************************/
#ifndef DUPSHELL
#define DUPSHELL
#include <cstring>
#include "header.h"
#define READ_END 0
#define WRITE_END 1
using namespace std;
#endif // DUPSHELL

int parseLine(char *,char *[]);
int check(char *);

int main(int argc, char *argv[])
{
        pid_t fpid,fpid1,fpid2;
        char *line;
        char str[20]="";//str is used to get the input
        char *command_array[9], *command_arrayA[9], *command_arrayB[9];//for saving commands
        //create a pipe
        int mypipe [2];
        if(pipe(mypipe)){
                fprintf(stderr,"Pipe Failed.\n");
                return -1;
        }

        do
        {
                cout<<"MyShell>";
                cin.getline(str,20);
                line = str;

                if(str[0]==0) continue;//directly enter, input no instruction

                char tmp[20];      strcpy(tmp,str);
                char *tmp_ptr = strtok(tmp,"|");
                if(strcmp(tmp_ptr,str)==0)
                {
                        //no pipe,act as MyShell.cpp
                        int count = parseLine(line,command_array);
                        if(strcmp(command_array[0],"exit") == 0) break;

                        fpid = fork();
                        if(fpid < 0)
                                cout<<"Error:Faild to fork.\n";
                        else if(fpid ==0)
                        {
                                if(execvp(command_array[0],command_array)==-1)
                                {
                                        cout<<"Error: running command: '"<<line<<"'\n";
                                        exit(0);
                                }
                        }
                        else wait(NULL);
                }
                else
                {
                        //pipe exists
                        int mypipe[2];
                        if(pipe(mypipe))
                        {
                                cout<<"Pipe Failed.\n";
                                return -1;
                        }

                        //seperate the whole command by "|" and save the command before "|" to command_arrayA
                        //the command after"|" to command_arrayB
                        char *line1,*line2;
                        char *p=strtok(line,"|");
                        line1 = p;
                        p = strtok(NULL,"|");
                        line2 = p;

                        int count1,count2;
                        count1 = parseLine(line1,command_arrayA);
                        count2 = parseLine(line2,command_arrayB);
                        fpid1 = fork();
                        if(fpid1 < 0)
                                cout<<"Error:Faild to fork.\n";
                        else if(fpid1 == 0)
                        {
                                //child1,handle the command before "|"
                                close(mypipe[READ_END]);
                                close(1);
                                dup2(mypipe[WRITE_END],STDOUT_FILENO);
                                close(mypipe[WRITE_END]);
                                if(execvp(command_arrayA[0],command_arrayA)==-1)
                                        cout<<"Error: running command: "<<line1<<endl;
                        }
                        else
                        {

                                fpid2 = fork();
                                if(fpid2 < 0)
                                        cout<<"Error:Faild to fork.\n";
                                else if(fpid2 == 0)
                                {
                                        //child2, handle the command after "|"
                                        close(mypipe[WRITE_END]);
                                        close(0);
                                        dup2(mypipe[READ_END],STDIN_FILENO);
                                        close(mypipe[READ_END]);
                                        if(execvp(command_arrayB[0],command_arrayB)==-1)
                                                cout<<"Error: running command: "<<line2<<endl;
                                }
                                else
                                {
                                        //parent, wait two children
                                        close(mypipe[WRITE_END]);
                                        close(mypipe[READ_END]);
                                        wait4(fpid1,NULL,0,NULL);
                                        wait4(fpid2,NULL,0,NULL);
                                }
                        }
                }
        }while(true);

        return 0;
}

int parseLine(char *line, char *command_array[])
{
        char *p;
        int count=0;
        p = strtok(line, " ");
        while (p && strcmp(p,"|")!=0)
        {
                command_array[count] = p;
                count++;
                p = strtok(NULL," ");
        }
        command_array[count] = NULL;
        return count;
}
