#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
#define CAPACITY 100
#include "struct.h"

class priorityQueue
{
    public:
        priorityQueue() {array = new _process* [CAPACITY]; maxSize = CAPACITY; currentSize = 0;}
        ~priorityQueue(){delete [] array;}

        bool isEmpty() const {return currentSize == 0;}
        _process* top() const {return array[1];}

    private:
        int currentSize;
        _process **array;
        int maxSize;

    public:
        void push(_process* x)
        {
            if(currentSize == maxSize - 1) doubleSpace();

            int hole = ++currentSize;
            for(;hole>1 && x->startTime<array[hole/2]->startTime;hole/=2)
                array[hole] = array[hole/2];
            array[hole] = x;
        }

        void pop()
        {
            array[1] = array[currentSize--];
            percolateDown(1);
        }

    private:
        void percolateDown(int hole)
        {
            int child;
            _process* tmp = array[hole];

            for(;hole*2 <= currentSize;hole=child)
            {
                child = hole * 2;
                if(child != currentSize && array[child+1]->startTime<array[child]->startTime)
                    child++;
                if(array[child]->startTime < tmp->startTime) array[hole] = array [child];
                else break;
            }
            array[hole] = tmp;
        }

        void doubleSpace()
        {
            _process** tmp = array;
            maxSize *= 2;
            array = new _process* [maxSize];

            for(int i=0;i<=currentSize;i++)
                array[i] = tmp[i];

            delete [] tmp;
        }
};

#endif // PRIORITYQUEUE_H
