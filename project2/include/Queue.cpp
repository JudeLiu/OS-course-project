#include "Queue.h"

/****************************************
//
//  Construct the class
//
//***************************************/
Queue::Queue()
{
    front = new node;
    front -> next = tail = new node;
    tail -> prev = front;
    currentLength = 0;
    pointer = 0;
}

/****************************************
//
//  Destruct the class
//
//***************************************/
Queue::~Queue()
{
    node *p=front->next,*q;
    front->next = tail;
    tail->prev = front;

    while(p != tail)
    {
        q = p->next;
        delete p;
        p = q;
    }
}

/*****************************************************
//
//  Move the node pointer to the i-th node of the queue
//
//  \param i is the position you want to move to
//
//  \return the i-th node of queue
//
//***************************************************/
Queue::node* Queue::move(int i)const
{
    node *p=front->next;
    if(i<0 || i>currentLength);
    while(i--) p=p->next;
    return p;
}

/*****************************************************
//
//  Remove the first node of queue
//
//  \param  None
//
//  \return None
//
//***************************************************/
void Queue::pop()
{
    node* tmp=front->next;
    front->next = tmp->next;
    tmp->next->prev = front;
    delete tmp;

    currentLength--;
}

/*****************************************************
//
//  Enqueue this page to the tail of queue
//
//  \param x is the page to be pushed in queue
//
//  \return None
//
//***************************************************/
void Queue::push(const page &x)
{
    node *tmp=new node(x,tail->prev,tail);
    tail->prev->next = tmp;
    tail->prev = tmp;

    currentLength++;
}

/*****************************************************
//
//  Search page x in the queue, return the position of it
//
//  \param x is the page to search
//
//  \return the position of this page in queue
//
//***************************************************/
int Queue::search(const page &x)const
{//return the position of x
    node *p=front->next;
    int index=0;
    while(p!=tail && p->data!=x)
    {
        index++;
        p = p->next;
    }
    if(p==tail) return -1;
    else return index;
}

/*****************************************************
//
//  Remove page x from queue. First search x to get its postion
//  then remove it.
//
//  \param x is the page to be removed
//
//  \return None
//
//***************************************************/
void Queue::remove(const page &x)
{
    int pos = search(x);
    node* p;
    if(pos!=-1)
    {
        p = move(pos);
        p->prev->next = p->next;
        p->next->prev = p->prev;
        delete p;
        currentLength--;
    }
}

/*****************************************************
/*****************************************************
/*****************************************************
//
//  Construct the class
//
//***************************************************/
circularQueue::circularQueue()
{
    currentLength = 0;
    pointer = 0;
    front = new node;
    front -> next = tail = new node;
    tail -> prev = front;
    front->prev = tail;
    tail->next = front;
}

/*****************************************************
//
//  Destruct the class
//
//***************************************************/
circularQueue::~circularQueue()
{
    node *p=front->next,*q;
    front->next = tail;
    tail->prev = front;

    while(p != tail)
    {
        q = p->next;
        delete p;
        p = q;
    }
}

/*****************************************************
//
//  Enqueue this page to the tail of queue
//
//  \param p is the page to be pushed in queue
//
//  \return None
//
//***************************************************/
void circularQueue::push(const page &p)
{//enqueue the element to the tail of queue
    node *tmp=new node(p,tail->prev,tail);
    tail->prev->next = tmp;
    tail->prev = tmp;

    currentLength++;
}

/*****************************************************
//
//  Remove the first node of queue
//
//  \param  None
//
//  \return None
//
//***************************************************/
void circularQueue::pop()
{//dequeue the first element of the queue
    node* tmp=front->next;
    front->next = tmp->next;
    tmp->next->prev = front;
    delete tmp;
    currentLength--;
}

/*****************************************************
//
//  Search page x in the queue, return the position of it
//
//  \param x is the page to search
//
//  \return the position of this page in queue
//
//***************************************************/
int circularQueue::search(const page &x)const
{//return the position of x, range from 0 to max-1,-1 means not found
    node *p=front->next;
    int index=0;
    while(p!=tail && p->data!=x)
    {
        index++;
        p = p->next;
    }
    if(p==tail) return -1;
    else return index;
}

/*****************************************************
//
//  Remove pos-th node from queue
//
//  \param pos is position of the node to be removed
//
//  \return None
//
//***************************************************/
void circularQueue::remove(const int &pos)
{
    node* p;
    if(pos!=-1)
    {
        p = move(pos);
        p->prev->next = p->next;
        p->next->prev = p->prev;
        delete p;
        currentLength--;
    }
}


/*****************************************************
//
//  In second-chance algorithm, when we want to do page
//  replacement, we should find a page whose reference bit
//  is 0, choose it to be victim and move it from memory to
//  disk, clear its valid bit.
//
//  \param None
//
//  \return the position of the victim
//
//***************************************************/
int circularQueue::find()
{
    int p=pointer;
    node* tmp=move(pointer);
    do
    {
        if(tmp->data.refBit==0)
        {
            pointer = p;
            return p;
        }
        else
        {
            tmp->data.refBit = 0;
            tmp = tmp->next;
            if(tmp == tail)
                tmp = front->next;
            p = (p+1)%memSize;
        }
    }while(((p+1)%memSize) != pointer);
    return -1;
}


/*****************************************************
//
//  Move the node pointer to position i
//
//  \param i is the position you want to move
//
//  \return the i-th node in the queue
//
//***************************************************/
circularQueue::node* circularQueue::move(int i)const
{
    node *p=front->next;
    if(i<0 || i>currentLength);
    while(i--) p=p->next;
    return p;
}


/*****************************************************
//
//  Set reference bit of page x
//
//  reference bit of \param x will be set as 1
//
//  \return None
//
//***************************************************/
void circularQueue::setRefBit(const page &x)
{
    int pos = search(x);
    if(pos>-1)
    {
        node* n=move(pos);
        n->data.refBit = 1;
    }
}

/*****************************************************
//
//  Clear reference bit of page x
//
//  reference bit of \param x will be clear as 0
//
//  \return None
//
//***************************************************/
void circularQueue::clearRefBit(const page &x)
{
    int pos = search(x);
    if(pos>-1)
    {
        node* n = move(pos);
        n->data.refBit = 0;
    }
}

/*****************************************************
//
//  Clear all reference bits of the pages in memory
//
//  \return None
//
//***************************************************/
void circularQueue::clearAllRefBits()
{
    node* p=front->next;
    while(p != tail)
    {
        p->data.refBit = 0;
        p = p->next;
    }
}
