#ifndef HEADER_H_
#define HEADER_H_
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <string>
#include <iomanip>
#include <stdlib.h>
#include <list>
#include <queue>
#include <semaphore.h>
#include "priorityQueue.h"
#include "Mem.cpp"
#include "struct.h"

//#define PAGESIZE 2097152//2^21
#define PAGESIZE 256//2^8
using namespace std;

ifstream *mem_trace;//memory file of each process
ifstream sche_trace_in;//shceduling trace file
ofstream fout;

bool** pageTable;// valid page
int finishCnt=0;//number of finished processes
int memSize;//size of memory,read as the fisrt parameter of main function
unsigned long long noExecTimeCycle;
unsigned long long idleTimeCycle;
int execTimeCycle;

string policies[3]={"fifo","lru","2ch-alg"};

sem_t page_fault;
sem_t blockQueue_mutex;
sem_t validPage_mutex;
sem_t arrivalQueue_mutex;

_process *process;

queue<_process*> readyQueue,blockQueue;
priorityQueue  arrivalQueue;

Mem mem;

int ReadIn();//function to read data from scheduling trace file
void *ProcessSche(void *);//process scheduling
void *PageRep(void *);//page replacement
void PrintResult(const int&);//print result
char *path(char *);//find the path of memory file
#endif /*HEADER_H_*/
