/********************************************************
//
//  Define all the structures needed in this program
//
//******************************************************/
#ifndef STRUCT_H
#define STRUCT_H
#include <string>

#define FIRSTSTART 1
#define NOTFIRSTSTART 0

/********************************************************
//
//  Contains all variables needed to describe a process
//
********************************************************/
struct _process
{
        std::string name;//name of process, e.g. "P1"
        unsigned long long startTime;// the start time
        unsigned long long cpuTime;// the cpu time
        int pageFaultCnt;// the number of page fault of this process
        double IOCount;// no use
        bool isFirstStart;// a flag to judge whether this process runs the first time
        int currentInst;//the current instruction
        int currentInstAddr;// the position in mem trace of current instruction
        unsigned long long elapsedTime;
        int elapsedTimeStart;//the first start cycle of this process
        unsigned long long elapsedTimeEnd;//the cycle when this process finishes
        int id;// e.g. id of P1 is 0, P2 is 1

        _process()
        {
            isFirstStart = FIRSTSTART;
            currentInst = 0;
            currentInstAddr = 0;
            elapsedTime = 0;
        }
};

/*****************************************************************
//
//  Two variables, addr is the address of page, refBit if the reference bit
//
//***************************************************************/
struct page
{
    int pid;//process id
    int addr;//address of page
    int refBit;//reference bit

    page(){pid=0;refBit=0;addr=0;}
    page(const int &i,const int &a,const int &rb=0) {pid=i;addr=a;refBit=rb;}
    page(const page &p){pid=p.pid;addr=p.addr;refBit=p.refBit;}
    page& operator=(const page &p){pid=p.pid;addr=p.addr;refBit=p.refBit;return *this;}
    friend bool operator!=(const page&a,const page &b) {return a.pid!=b.pid || a.addr!=b.addr;}
};

///transmit parameters to thread PageRep
struct PRarg
{
    int processNum;
    int pageSize;
    std::string policy;
    PRarg() {processNum=0;pageSize=0;policy="";}
};

///transmit parameters  to thread ProcessSche
struct PSarg
{
    int processNum;
    int quntumSize;
    std::string policy;
    PSarg(){processNum=0;quntumSize=0;policy="";}
};

#endif//STRUCT_H
