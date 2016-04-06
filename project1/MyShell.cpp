/****************************************************************************
        Description:
                This is a mini-shell program allowing arguments to the commands This program
        will read the command from the input and execute the command. Because it is
        easy to solve problem 6 at the same time of this program is just a copy of
        "MoreShell.cpp".

        Algorithm:
                Get a line of command and save it as  char[]. Use function parsline()
        to seperate the whole string into char array, which contains the commands and
        arguments. Invoke execvp() system call to accomplish the command.

        Sample input and output:
        -----------------------------------------------------
        ~/project1$ ./MyShell
        MyShell> date
        2015年 04月 20日 星期一 23:19:32 CST
        MyShell>exit
        ~/project1$
        -----------------------------------------------------
****************************************************************************/
#ifndef MYSHELL
#define MYSHELL
#include <cstring>
#include "header.h"
using namespace std;
#endif // MYSHELL

int parseLine(char *, char * []);

int main()
{
        pid_t fpid;
        char *line;
        char str[10]="";//stores the input line
        char *command_array[10];//stores the seperated command line
        do
        {
                /****************************************************
                in each loop, first invoke parseLine() to seperate the whole
                command line into several strings, including command and
                arguments, if the command is exit or noting then continue
                loop,
                if not then fork() the process, in child process we invoke
                execvp() funcion to accomplish the command as input,
                in parent process we just wait for the temination of child
                process.
                ****************************************************/
                cout<<"MyShell>";
                cin.getline(str,10);
                line = str;

                 if(str[0]==0) continue;//directly enter, input no instruction

                int count = parseLine(line,command_array);

                if(strcmp(command_array[0],"exit") == 0) break;

                fpid = fork();
                if(fpid < 0)
                        //fork error
                        cout<<"Error:Faild to fork.\n";
                else if(fpid ==0)
                {
                        //child process
                        if(execvp(command_array[0],command_array)==-1)
                        {
                                cout<<"Error: running command: '"<<line<<"'\n";
                                exit(0);
                        }
                }
                //parent process
                else wait(NULL);
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
