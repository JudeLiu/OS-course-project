/**********************************************************************
//
//  two classes: Queue and circularQueue.
//
//  class Queue is similar to STL class queue, but I add several function to it to make
//  efficient in this project. It is used in class Mem and thread ProcessSche
//
//  class circularQueue is a circular queue, it is used when page replacement policy is
//  second-chance algorithm
//
//********************************************************************/
#ifndef QUEUE_H
#define QUEUE_H
#include "struct.h"

extern int memSize;

class Queue
{
    public:
        Queue();
        ~Queue();
        bool isEmpty() const {return tail->prev==NULL;}
        bool isFull() const {return currentLength>=memSize;}
        void pop();//remove the first element of queue
        void remove(const page &x);//remove x from queue
        void push(const page &x);//push x to the tail of queue
        int search(const page &x)const;//search x in queue, return the position
        page frontVal() const {return front->next->data;}
        page tailVal() const {return tail->prev->data;}

    private:
        struct node
        {
            page data;
            node *next;
            node *prev;
            node (const page &x, node *P= NULL, node *N=NULL){data=x;prev=P;next=N;}
            node():prev(NULL),next(NULL){}
            ~node() {}
        };
        node *front,*tail;
        int currentLength;
        int pointer;

        node* move(int i)const;
};

class circularQueue
{
    public:
        circularQueue();
        ~circularQueue();

        bool isEmpty() const {return tail->prev==NULL;}
        bool isFull() const {return currentLength>=memSize;}
        page __move(int i)const {return (move(i)->data);}
        int getPointer() const{return pointer;}
        page frontVal()const {return front->next->data;}
        page tailVal()const {return tail->prev->data;}

        void setRefBit(const page &x);//set reference bit of x as 1
        void clearRefBit(const page &x);//clear reference bit of x
        void clearAllRefBits();
        void pop();
        void push(const page&);
        void remove(const int &pos);//remove the pos-th node from the queue
        int find();//find a page whose reference bit is 0, return the position of it
        int search(const page &p)const;//search page p in the queue, return the positioin

    private:
        struct node
        {
            page data;
            node *next;
            node *prev;

            node (const page &x, node *P= NULL, node *N=NULL){data=x;prev=P;next=N;}
            node():prev(NULL),next(NULL){}
        }*front,*tail;

        int pointer;
        int currentLength;

        node* move(int i)const;

};
#endif // QUEUE_H
