#ifndef MEM_H
#define MEM_H
#include "Queue.cpp"

extern bool **pageTable;

class Mem
{
        public:
                Mem();
                ~Mem();
                void FIFOPageRep(const int &id,const int &x);//fifo policy
                void LRUNoFaultAdd(const int &id,const int &x);//lru policy
                void LRUPageRep(const int &id,const int &x);//lru policy
                void SCASetRefBit(const int &id,const int &x);//2ch-alg policy
                void SCAPageRep(const int &id,const int &x);//2ch-alg policy

        private:
                sem_t mem_mutex;
                Queue memory;//memory, save  page address
                circularQueue pageQueue;
};
#endif // MEM_H
