/********************************************************************************
        Description:
                This multi‐threaded program  solves  the burger buddies  problem.

        Algorithm:
                Define semaphores: rack, burger_from_cook, burger_from_cashier, cashire,
        customer, and mutex: output_mutex, cnt_mutex. output_mutext is used for output,
        cnt_mutex is used for count the number of customers.
                In this program, first read the number of cooks, cashier and customers;
        then create threads according to the number.
                There are three types of thread:  __cook, __cashier and __customer.
                In __cook, what it will do are as follows:
                        1. wait for rack, if rack is not full then put a burger on it, signal burger ,
                        2. check the number of customers, if it is 0 then break, if not then continue
                        to the next loop.
                In __cashier,  what it will do are as follows:
                        1. wait for a customer to come,
                        2. wait for a burger from cook, then takes a burger from rack and signal rack ,
                        3. present the burger to customer, signal burger_from_cashier,
                        4. jobs done, signal cashier , continue to the next loop.
                In __ customer, what it will do are as follows:
                        1. signal customer to let cashier know that a customer comes,
                        2. wait for cashier,
                        3. wait for a burger from cashier and have the burger,
                        4. decrement the number of customer, signal customer and leave.
                In main function, after create these threads, join() and wait for their termination,
                then the whole program ends.

                NOTICE
                Unfortunately I encountered a problem that after the last customer begins
                running its code, it is likely that many cashier threads will begin running their code,
                and then wait() customer, so there exists starvation, and I failed to figure out how to
                 solve this problem.
********************************************************************************/
#include "header.h"
#include <semaphore.h>
#define WAIT_TIME        3
void *__cook(void *);
void *__cashier(void *);
void *__customer(void *);

sem_t rack;//semaphore of the number of the  burgers on the rack
sem_t burger_from_cashier;//semaphore of the burger from cashier
sem_t burger_from_cook;//semaphore of the burger from cook
sem_t cashier;//semaphore of cashier
sem_t customer;//semaphore of customer
sem_t output_mutex;//mutex for the outpur info
sem_t cnt_mutex;//mutex for counting customers
int customerCnt;//the number of customers who does not leave the restaurant
pthread_t *t_cashier;
int Cashiers;

int main(int argc, char *argv[])
{

        int i, *id_cook, *id_cashier, *id_customer;
        /**********************************************************************
         read in the number of cook, cashiers, customers and the racksize from *argv[]
                and convert them from char to int
        **********************************************************************/

        int Cooks, Customers, RackSize;
        Cooks = atoi(argv[1]);
        Cashiers = atoi(argv[2]);
        Customers = atoi(argv[3]);
        RackSize = atoi(argv[4]);
        customerCnt = Customers;

        /**********************************************************************
        statement of semaphores , and init.
        **********************************************************************/

        sem_init(&rack,0,RackSize);
        sem_init(&burger_from_cashier,0,0);
        sem_init(&burger_from_cook,0,0);
        sem_init(&customer,0,0);
        sem_init(&cashier,0,Cashiers);
        sem_init(&output_mutex,0,1);
        sem_init(&cnt_mutex,0,1);

        /**********************************************************************
                Info
        **********************************************************************/

        cout<<"Cooks["<<Cooks<<"], Cashiers["<<Cashiers<<"], Customers["<<Customers<<"].\n"
                <<"Begin  run.\n";

        /**********************************************************************
        statement of threads , then create threads
        **********************************************************************/
        id_cashier = new int [Cashiers];
        id_cook = new int [Cooks];
        id_customer = new int [Customers];

        pthread_t *t_cook =  new pthread_t [Cooks];
        t_cashier=  new pthread_t [Cashiers];
        pthread_t *t_customer =  new pthread_t [Customers];

        for(i=0;i<Cashiers;i++)
        {
                id_cashier[i] = i+1;
                pthread_create(&t_cashier[i],NULL,__cashier,&id_cashier[i]);
        }
        for(i=0;i<Cooks;i++)
        {
                id_cook[i] = i+1;
                pthread_create(&t_cook[i],NULL,__cook,&id_cook[i]);
        }
        for(i=0;i<Customers;i++)
        {
                id_customer[i] = i+1;
                pthread_create(&t_customer[i],NULL,__customer,&id_customer[i]);
        }

        /**********************************************************************
                join thread
        **********************************************************************/
        for(i=0;i<Cashiers;i++)
                pthread_join(t_cashier[i],NULL);

        for(i=0;i<Cooks;i++)
                pthread_join(t_cook[i],NULL);

        for(i=0;i<Customers;i++)
                pthread_join(t_customer[i],NULL);

        cout<<"Finished.\n";

        /**********************************************************************
                destroy semaphores
        **********************************************************************/

        sem_destroy(&burger_from_cashier);
        sem_destroy(&burger_from_cook);
        sem_destroy(&cashier);
        sem_destroy(&customer);
        sem_destroy(&rack);

        delete [] t_cook;
        delete [] t_cashier;
        delete [] t_customer;
        delete [] id_cook;
        delete [] id_cashier;
        delete [] id_customer;

        return 0;
}

void *__cook(void *arg)
{
        /*
        thread of cooks, steps are as follows:
        1. wait for rack, if rack is not full then put a burger on it, signal burger,
        2. check the number of customers, if it is 0 then break, if not then continue to the next loop.
        */
        int id = *(int *)arg;
        while(true)
        {
                sleep(rand()%WAIT_TIME);

                //step 1
                sem_wait(&rack);
                sem_post(&burger_from_cook);

                sem_wait(&output_mutex);
                cout<<"Cook ["<<id<<"] makes a burger.\n";
                sem_post(&output_mutex);

                //step 2
                sem_wait(&cnt_mutex);
                if(customerCnt==0)
                {
                        sem_post(&cnt_mutex);
                        break;
                }
                else
                        sem_post(&cnt_mutex);
        }
        return NULL;
}

void *__cashier(void *arg)
{
        /*
        thread of cashiers, steps are as follows:
        1. wait for a customer to come,
        2. wait for a burger from cook, then takes a burger from rack and signal rack ,
        3. present the burger to customer, signal burger_from_cashier,
        4. jobs done, signal cashier , continue to the next loop.
        */
        int id = *(int *)arg;
        while(true)
        {
                sleep(rand()%WAIT_TIME);
                //step 1
                sem_wait(&customer);

                if(customerCnt < 1)
                {
                        sem_post(&cashier);
                        break;
                }

                sem_wait(&output_mutex);
                cout<<"Cashier ["<<id<<"] acccepts an order.\n";
                sem_post(&output_mutex);

                //step 2
                sem_wait(&burger_from_cook);
                sem_post(&rack);

                //step 3
                sem_post(&burger_from_cashier);

                sem_wait(&output_mutex);
                cout<<"Cashier ["<<id<<"] takes a burger to customer.\n";
                sem_post(&output_mutex);

                //step 4
                sem_post(&cashier);

                if(customerCnt < 1)
                {
                        sem_post(&cashier);
                        break;
                }
        }
        return NULL;
}

void *__customer(void *arg)
{
        /*
        thread of customers, steps are as follows:
        1. signal customer to let cashier know that a customer comes,
        2. wait for cashier,
        3. wait for a burger from cashier and have the burger,
        4. decrement the number of customer, signal customer and leave.
        */
        int id = *(int *)arg;

        sleep(rand()%(2*WAIT_TIME));

        sem_wait(&output_mutex);
        cout<<"Customer ["<<id<<"] comes.\n";
        sem_post(&output_mutex);

        //step 1
        sem_post(&customer);

        //step 2
        sem_wait(&cashier);

        //step 3
        sem_wait(&burger_from_cashier);

        //step 4
        sem_wait(&cnt_mutex);
        customerCnt--;
        if(customerCnt==0)
        {
                for(int i=0;i<Cashiers;i++)
                        pthread_cancel(t_cashier[i]);
                sem_post(&cnt_mutex);
                int r;
                do
                {
                        sleep(3);
                        sem_getvalue(&customer,&r);
                        if(r<0)
                                sem_post(&customer);
                        else break;
                }while(true);
        }
        else
                sem_post(&cnt_mutex);

        return NULL;
}
