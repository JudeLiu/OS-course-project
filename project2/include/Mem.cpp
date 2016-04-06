#include "Mem.h"


/*****************************************************
//
//  Construct the class
//
//  \return None
//
//***************************************************/
Mem::Mem()
{
    sem_init(&mem_mutex,0,1);
}


/*****************************************************
//
//  Destruct the class
//
//  \return None
//
//***************************************************/
Mem::~Mem()
{
    sem_destroy(&mem_mutex);
}

/**********************************************************
//
//  FIFO page replacement
//
//  \param addr is the address of the page that has page fault
//
//  \return None
//
//********************************************************/
void Mem::FIFOPageRep(const int &__pid, const int &__addr)
{
    if(memory.isFull())
    {
    /*memory is full, set the addr as valid
     choose the front of memory as victim(memory is a queue), because it first comes
     and makes it invalid*/
        pageTable[__pid][__addr] = 1;
        pageTable[memory.frontVal().pid][memory.frontVal().addr] = 0;
        memory.pop();
    }
    else
    {
    /*memory is not full, set addr as valid*/
        pageTable[__pid][__addr] = 1;
    }
    page p(__pid,__addr);
    memory.push(p);
}

/**********************************************************
//
//  If no page fault when the policy is lru, remove the page from
//  memory and push it to the tail of it
//
//  \param addr is the address of the page
//
//  \return none
//
//********************************************************/
void Mem::LRUNoFaultAdd(const int &__pid, const int &__addr)
{
    /*addr is already valid, so remove it and put it to the tail of memory*/
    sem_wait(&mem_mutex);
    page p(__pid,__addr);
    memory.remove(p);
    memory.push(p);
    sem_post(&mem_mutex);
}

/**********************************************************
//
//  LRU page replacement
//
//  \param addr is the address of the page that has page fault
//
//  \return none
//
//********************************************************/
void Mem::LRUPageRep(const int &__pid, const int &__addr)
{
    /*addr is invalid, there are two cases:
    if memory is full, then choose the front of memory as victim,
    because it is the first used process.
    if memory is not full, just set addr as valid.*/
    sem_wait(&mem_mutex);
    if(memory.isFull())
    {
        pageTable[memory.frontVal().pid][memory.frontVal().addr] = 0;
        pageTable[__pid][__addr] = 1;
        memory.pop();
    }
    else
    {
        pageTable[__pid][__addr] = 1;
    }
    page p(__pid,__addr);
    memory.push(p);
    sem_post(&mem_mutex);
}

/**********************************************************
//
//  If no page fault when the policy is second-chance algorithm,
//  set the reference bit as 1 because this page was just used
//
//  \param addr is the address of the page
//
//  \return none
//
//********************************************************/
void Mem::SCASetRefBit(const int &__pid, const int &__addr)
{
    /*no page fault, set the reference bit of addr as 1.*/
    page p(__pid,__addr);
    pageQueue.setRefBit(p);
}

/**********************************************************
//
//  Second-chance algorithm page replacement.     If memory is full,
//  find a page with refecence bit 0 in memory, and choose it as victim;
//  if not found, clear all reference bits as 0, choose the page that
//  pointer points as victim; if memory is not full, move addr into
//  memory and set its refecence bit as 1.
//
//  \param addr is the address of the page that has page fault
//
//  \return none
//
//********************************************************/
void Mem::SCAPageRep(const int &__pid, const int &__addr)
{
    int pos;
    page p;
    if(pageQueue.isFull())
    {
        pos = pageQueue.find();
        if(pos == -1)
        {
            pageQueue.clearAllRefBits();
            p = pageQueue.__move(pageQueue.getPointer());
            pageTable[p.pid][p.addr] = 0;
            pageTable[__pid][__addr] = 1;
            pageQueue.remove(pageQueue.getPointer());
        }
        else
        {
            p = pageQueue.__move(pos);
            pageTable[p.pid][p.addr] = 0;
            pageTable[__pid][__addr] = 1;
            pageQueue.remove(pos);
        }
        p.pid = __pid;
        p.addr = __addr;
        p.refBit = 1;
        pageQueue.push(p);
    }
    else
    {
        pageTable[__pid][__addr] = 1;

        p.pid = __pid;
        p.addr = __addr;
        p.refBit = 1;
        pageQueue.push(p);
    }
}
