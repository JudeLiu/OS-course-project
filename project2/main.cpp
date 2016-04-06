//***********************************************************************************************//
// include files:
// "header.h" : includes all header file needed and classes, and declare global variables and
// functions.
// "Queue.h" : includes two classes Queue and circularQueue and two structure _process
// and page. Structure _process is used to describe a process, and structure page is used by second-
// chance algorithm.
// class Queue is based on a linked list, and has several functions, you can check Queue.h to see
// how they are used.
// class circularQueue is based on a circular linked list, and is only used by second-chance algorithm.
// You can check Queue.h to see detailed informatiom.
// "priorityQueue.h" : includes a class priorityQueue, it is based on a array, and is used for
//  arrivalQueue in processSche, because arrivalQueue is sorted by the order of time.
// "Mem.h" :  includes a class Mem, it simulates how memory deals with page fault.
// This class has a Queue named memory to store page address, and is used by fifo and lru algorithm;
// it also has a priorityQueue named pageQueue, and is only used by second-chance algorithm.
//
// This program simulates how scheduler works and how to deal with page fault.
// There are two threads in main function, ProcessSche and PageRep.
// The thread ProcessSche deals with process scheduling, PageRep deals with page
// replacement.
// ProcessSche : in each loop, follows these steps:
//  1. if currentCycle equals to the first one of arrivalQueue, then the process will be pop from arrivalQueue,
//  and pushed to the back of  readyQueue. Then check if status is CONTEXTSWITCH, if so, continue to
//  the next loop; if not, go to step 2
//  2. if  there is no runningProcess, go to step 3; else go to step 6
//  3. if readyQueue is empty, if it is empty, go to step 4; if not, go to step 5
//  4. increase noExecTime, go to step 5
//  5. pop the front element of readyQueue, and let it be runningProcess, go to step 6
//  6. read the current instruction from the memory traces, and check if it is in memory,
//  if so, increase execTimeCycle;else push it to blockQueue, and let runningProcess be NULL
//  7. increase currentCycle by 1, go to step 1.
//  the thread will follow these steps until the number of finished process equals to the total number.
//
//  PageRep : in each loop, follow these steps:
//  1. if blockQueue is not empty, then pop() blockQueue, and do page replacement according to
//  the policy you enter, push this process to arrivalQueue, and continue to the next loop
//  2. if blockQueue is empty, continue to the next loop.
//  the thread will follow these steps until the number of finished process equals to the total number.
//
// Synchronization:
// Because thread ProcessSche and PageRep work at the same time, and move page from disk to
// memory costs 1000 cycles, so in the 1000 cycles to come, the process must be pushed back to
// arrivalQueue again,  or ProcessSche has to wait for it, otherwise the time will exceed 1000 plus
// the time when it blocked, and those processes which are supposed to be later than this process
// will be earlier than it. To implement this function, every time PageRep move a page from disk to
// memory, it signal a semaphore page_fault. In ProcessSche, after 1000 plus the time a page fault
// happens it wait the semaphore page_fault.
//
//  arrivalQueue blockQueue and validPage are shared by these two threads, so there are three mutex
//  arrivalQueue_mutex blockQueue_mutex and validPage_mutex
//***********************************************************************************************//
#include "header.h"
using namespace std;

int main(int argc, char *argv[])
{
    int i=0,j=0;
    /*initialize semaphores*/
    sem_init(&page_fault,0,0);
    sem_init(&blockQueue_mutex,0,1);
    sem_init(&validPage_mutex,0,1);
    sem_init(&arrivalQueue_mutex,0,1);

    /*declare thread*/
    pthread_t t_cpu,t_mem;

    /*read data*/
    sche_trace_in.open(argv[4],ios::in);
    int pNum;//number of processes
    pNum = ReadIn();

    /*mem reference*/
    mem_trace = new ifstream [pNum];
    string *pName=new string [pNum];

    /* create file name of mem reference,
        e.g. If process is P1, the path is ./Testdata/dataL/
        the file name is "./Testdata/dataL/P1.mem"*/
    for(i=0;i<pNum;i++)
    {
            pName[i] = path(argv[4]);
            pName[i] += process[i].name;
            pName[i] += ".mem";
            mem_trace[i].open(pName[i].c_str(),ios::in);
    }

    /*initialize page table, set all page as invalid*/
    pageTable = new bool *[pNum];
    for(i=0;i<pNum;i++)
        pageTable[i] = new bool[PAGESIZE];
    for(i=0;i<pNum;i++)
        for(j=0;j<PAGESIZE;j++)
            pageTable[i][j] = 0;

    /*create the name of output file,
    e.g. if configuration is 50 memory size,
    10000 quntum, fifo policy, the output file
    name is "./result/50-10000-fifo"*/
    string s_tmp("./result/");
    s_tmp += argv[1];
    s_tmp += '_';
    s_tmp += argv[2];
    s_tmp += '_';
    s_tmp += argv[3];
    fout.open(s_tmp.c_str(),ios::out);

    memSize = atoi(argv[1]);

    PSarg psArgs;
    psArgs.processNum = pNum;
    psArgs.quntumSize = atoi(argv[2]);
    psArgs.policy = argv[3];

    PRarg prArgs;
    prArgs.policy = psArgs.policy;
    prArgs.processNum = pNum;

    /*create threads*/
    pthread_create(&t_cpu,NULL,ProcessSche,&psArgs);
    pthread_create(&t_mem,NULL,PageRep,&prArgs);

    /*join threads*/
    pthread_join(t_cpu,NULL);
    pthread_join(t_mem,NULL);

    /*print result*/
    PrintResult(pNum);

   /*destroy semaphores*/
    sem_destroy(&page_fault);
    sem_destroy(&blockQueue_mutex);
    sem_destroy(&arrivalQueue_mutex);
    sem_destroy(&validPage_mutex);

    /*delete pointers and close files*/
    for(i=0;i<pNum;i++)
        delete [] pageTable[i];
    delete [] pageTable;
    delete [] process;
    for(i=0;i<pNum;i++)
            mem_trace[i].close();
    delete [] pName;
    delete [] mem_trace;
    sche_trace_in.close();
    fout.close();
    return 0;
}

/**********************************************************************
//
//  Read in the content of scheduling traces file, get the name, start time
//  and CPU time of all processes
//
//  \param none
//
//  \return the number of processes
//
//**********************************************************************/
int ReadIn()
{
        int i,num;
        string arr[1000];
        string p;

        i = 0;
        while(sche_trace_in>>p)
            arr[i++] = p;
        num = i/4;
       process = new _process [num];
        for(i=0;i<num;i++)
        {
                process[i].name = arr[i*4];
                process[i].startTime = atof(arr[i*4+1].c_str())*100000;
                process[i].cpuTime = atof(arr[i*4+2].c_str())*100000;
                process[i].IOCount = atoi(arr[i*4+3].c_str());
                process[i].id = i;
        }

        /*initialize arrival time queue*/
        for(i=0;i<num;i++)
                arrivalQueue.push(&process[i]);

        return num;
}

/**********************************************************************
//
//  This is the thread for simulating process scheduling, and is introduced in the front
//  of this file and Project2Intro file.
//
//  \param psArgs receives the value of the number of processes, quntum size and
//  page replacement policy
//
//  \return none
//
//**********************************************************************/
void *ProcessSche(void *psArgs)
{
        PSarg args = *(PSarg *)psArgs;
        unsigned long long currentCycle = 0;
        unsigned long long switchStartCycle;//the start time of switch
        unsigned long long quntumStartCycle;
        queue<unsigned long long> waitBackTime;
        int i;
        _process* runningProcess=NULL;

        #define NOPROCESS 0
        #define RUNNING 1
        #define READY 2
        #define CONTEXTSWITCH 4
        #define PAGEFAULT 8
        int status = NOPROCESS;

        _process* arrivalProcess=NULL;

        //main loop
        while(true)
        {
                if(status & PAGEFAULT)
                {
                    /*this thread seems faster than thread PageRep, so it has to
                    wait for the fault page to be pushed in the arrival queue, or wrong result may happen*/
                    if(currentCycle == (waitBackTime.front()-100))
                    {
                        if(waitBackTime.empty())
                            status &= ~PAGEFAULT;
                        waitBackTime.pop();
                        sem_wait(&page_fault);
                    }
                }
                sem_wait(&arrivalQueue_mutex);
                if(currentCycle == (arrivalQueue.top()->startTime))
                {
                        arrivalProcess = arrivalQueue.top();
                        readyQueue.push(arrivalProcess);
                        arrivalQueue.pop();
                        sem_post(&arrivalQueue_mutex);
                        if(status==NOPROCESS)
                                status |= READY;
                }
                else
                    sem_post(&arrivalQueue_mutex);

                if(status & CONTEXTSWITCH)
                {
                        if(currentCycle == (switchStartCycle+50))
                                status &= ~CONTEXTSWITCH;
                        idleTimeCycle++;
                }
                else
                {//no context switch
                        if(runningProcess==NULL)
                        {//no process is running
                                if(readyQueue.empty())
                                        idleTimeCycle++;
                                else
                                {//the first cycle a new process starts
                                        runningProcess = readyQueue.front();
                                        readyQueue.pop();
                                        quntumStartCycle = currentCycle;

                                        //read instruction
                                        if(runningProcess->isFirstStart == FIRSTSTART)
                                        {//the first time the process starts
                                                runningProcess->elapsedTimeStart = currentCycle;
                                                mem_trace[runningProcess->id]>>hex>>(runningProcess->currentInst);
                                                execTimeCycle += runningProcess->cpuTime;
                                                runningProcess->isFirstStart = NOTFIRSTSTART;
                                        }

                                        sem_wait(&validPage_mutex);
                                        if(pageTable[runningProcess->id][runningProcess->currentInst])
                                        {//no page fault
                                                sem_post(&validPage_mutex);

                                                if(args.policy == policies[1])//policy is lru
                                                    mem.LRUNoFaultAdd(runningProcess->id,runningProcess->currentInst);
                                                else if(args.policy == policies[2])//policy is 2ch-alg
                                                    mem.SCASetRefBit(runningProcess->id,runningProcess->currentInst);

                                                runningProcess->cpuTime --;
                                                runningProcess->currentInstAddr ++;

                                                /*a process finishes if it has used up its CPU time*/
                                                /*if all processes finished, the thread will exit*/
                                               if(runningProcess->cpuTime == 0)
                                                {
                                                    finishCnt++;
                                                    runningProcess->elapsedTime = currentCycle - runningProcess->elapsedTimeStart;
                                                    cout<<currentCycle<<": "<<runningProcess->name<<" finish\n";
                                                    fout<<currentCycle<<": "<<runningProcess->name<<" finish\n";
                                                    runningProcess = NULL;
                                                    if(finishCnt==args.processNum)
                                                        break;
                                                }
                                        }
                                        else
                                        {//page fault occurs
                                                sem_post(&validPage_mutex);
//cout<<currentCycle<<" : page fault\n";
                                                status |= PAGEFAULT;
                                                waitBackTime.push(currentCycle + 1000);
                                                runningProcess->startTime = currentCycle;

                                                sem_wait(&blockQueue_mutex);
                                                blockQueue.push(runningProcess);
                                                sem_post(&blockQueue_mutex);

                                                runningProcess->pageFaultCnt++;
                                                if(!readyQueue.empty())
                                                {
                                                        status |= CONTEXTSWITCH;
                                                        switchStartCycle = currentCycle;
                                                }
                                                runningProcess = NULL;
                                                idleTimeCycle++;
                                        }
                                }
                        }
                        else
                        {//a process is running
                                //read instruction
                                mem_trace[runningProcess->id]>>hex>>(runningProcess->currentInst);
                                runningProcess->currentInstAddr ++;

                                sem_wait(&validPage_mutex);
                                if(pageTable[runningProcess->id][runningProcess->currentInst] == 1)
                                {//no page fault
                                        sem_post(&validPage_mutex);

                                        if(args.policy == policies[1])//policy is lru
                                            mem.LRUNoFaultAdd(runningProcess->id,runningProcess->currentInst);
                                        else if(args.policy == policies[2])//policy is 2ch-alg
                                            mem.SCASetRefBit(runningProcess->id,runningProcess->currentInst);

                                        runningProcess->cpuTime--;
                                        /*a process finishes if it has used up its CPU time*/
                                        /*if all processes finished, the thread will exit*/
                                        if(runningProcess->cpuTime == 0)
                                        {
                                                finishCnt++;
                                                runningProcess->elapsedTime = currentCycle - runningProcess->elapsedTimeStart;
                                                cout<<currentCycle<<": "<<runningProcess->name<<" finish\n";
                                                fout<<currentCycle<<": "<<runningProcess->name<<" finish\n";
                                                runningProcess = NULL;
                                                if(finishCnt==args.processNum)
                                                    break;
                                        }
                                }
                                else
                                {//page fault occurs
                                        sem_post(&validPage_mutex);
//cout<<currentCycle<<" : page fault\n";
                                        status |= PAGEFAULT;
                                        waitBackTime.push(currentCycle + 1000);
                                        runningProcess->startTime = currentCycle;

                                        sem_wait(&blockQueue_mutex);
                                        blockQueue.push(runningProcess);
                                        sem_post(&blockQueue_mutex);
                                        runningProcess->pageFaultCnt++;
                                        if(!readyQueue.empty())
                                        {
                                                status |= CONTEXTSWITCH;
                                                switchStartCycle = currentCycle;
                                        }
                                        runningProcess = NULL;
                                        idleTimeCycle++;
                                        currentCycle++;
                                        continue;
                                }

                                if(currentCycle == (quntumStartCycle+args.quntumSize))
                                {
                                    if(!readyQueue.empty())
                                    {
                                        status |= CONTEXTSWITCH;
                                        switchStartCycle  = currentCycle;
                                    }
                                    readyQueue.push(runningProcess);
                                    runningProcess = NULL;
                            }
                    }
            }
            currentCycle++;
    }
}

/**********************************************************************
//
//  This is the thread for simulating page replacement, and is introduced in the
//  front of this file and Project2Intro file.
//
//  \param prArgs receives the value of the number of processes, memory size
//  and the page replacement policy
//
//  \return none
//
//*********************************************************************/
void* PageRep(void *prArgs)
{
        PRarg args=*(PRarg *)prArgs;

        _process* currentProcess;
        unsigned long long lastTimeCycle=0;

        while(finishCnt<args.processNum)
        {
                sem_wait(&blockQueue_mutex);
                if(blockQueue.empty())
                    sem_post(&blockQueue_mutex);
                else
                {
                    currentProcess = blockQueue.front();
                    blockQueue.pop();
                    sem_post(&blockQueue_mutex);

                    if(currentProcess->startTime < lastTimeCycle)
                            currentProcess->startTime = lastTimeCycle;

                    sem_wait(&validPage_mutex);
                    if (args.policy == policies[0])//FIFO
                        mem.FIFOPageRep(currentProcess->id,currentProcess->currentInst);
                    else if(args.policy == policies[1])//LRU
                        mem.LRUPageRep(currentProcess->id,currentProcess->currentInst);
                    else if(args.policy == policies[2])//2-ch alg
                        mem.SCAPageRep(currentProcess->id,currentProcess->currentInst);
                    else
                    {
                        pageTable[currentProcess->id][currentProcess->currentInst] = 1;
                    }
                    sem_post(&validPage_mutex);

                    currentProcess->startTime += 1000;
                    lastTimeCycle = currentProcess->startTime;

                    sem_wait(&arrivalQueue_mutex);
                    arrivalQueue.push(currentProcess);
                    sem_post(&arrivalQueue_mutex);
                    currentProcess = NULL;

                    sem_post(&page_fault);
            }
        }
}

/**********************************************************************
//  Find the path
//
//  \param str is a file name, e.g. "./Testdata/dataL/sche_traces_big.txt"
//
//  Because scheduling trace files of testdata are in different directories,
//  so I have to find out the path of .mem files.
//  This function returns the characters before the last '/' of \param str,
//  e.g. if  str="./Testdata/Data/sche_traces_big.txt", then return value is
//  "./Testdata/Data/".
//
//  \return the path of \param str
//
//**********************************************************************/
char *path(char *str)
{
    int i=0,pos=0;
    char *p=str;
    while(*p!='\0')
    {
        if(*p=='/' || *p=='\'' )
            pos = i;
        i++;
        p++;
    }
    for(i=0,p=str;i<pos;i++)
        p[i] = str[i];
    p[pos+1] = '\0';
    return p;
}

/**********************************************************************
//
//  Print all the results on the screen and output file
//
//  \param pNum is the number of processes
//
//  \return none
//
//*********************************************************************/
void PrintResult(const int &pNum)
{
    int totalPageFaultCnt=0;
    int i;
    unsigned long long averageElapsedTime=0;
    cout<<"\n\tpage fault\telapsed time\n";
    fout<<"\tpage fault\telapsed time\n";
    for(i=0;i<pNum;i++)
    {
        totalPageFaultCnt += process[i].pageFaultCnt;
        averageElapsedTime += process[i].elapsedTime;
        cout<<process[i].name<<'\t'<<process[i].pageFaultCnt<<"\t\t"<<process[i].elapsedTime<<endl;
        fout<<process[i].name<<'\t'<<process[i].pageFaultCnt<<"\t\t"<<process[i].elapsedTime<<endl;
    }

    cout<<"\ntotal elapsed time :\t"<<execTimeCycle+idleTimeCycle
    <<"\ntotal idle time :\t"<<idleTimeCycle<<endl;
    fout<<"\ntotal elapsed time :\t"<<execTimeCycle+idleTimeCycle
    <<"\ntotal idle time :\t"<<idleTimeCycle<<endl;
    averageElapsedTime /= pNum;
    cout<<"total page fault :\t"<<totalPageFaultCnt<<endl;
    fout<<"total page fault :\t"<<totalPageFaultCnt<<endl;
    cout<<"average elapsed time :\t"<<averageElapsedTime<<endl;
    fout<<"average elapsed time :\t"<<averageElapsedTime<<endl;
}
