/****************************************************************************
        Description:
                This is a multi-threaded program to implement mergesort.

        Algorithm:
                Function MergeSort() can mergesort the input array.
                In main, we fork() a child process, in child process invokes MergeSort();
                in parent just waits for the child process to terminate.

        Sample input:
        -------------------------------------------------
        ~/project1$ ./MegesortMulti
        -------------------------------------------------
****************************************************************************/
#include "header.h"

void Merge(int *arr,int low,int mid,int high);
void _MergeSort(int *arr,int low,int high);
void *MergeSort(void *);
void readArguments(int *,int);

struct thread_arguments
{
        int *arr;
        int size;
};

int main()
{
        //timer starts here
        clock_t start, end;
        double elapsed;
        start = clock();

        pthread_t t1;
        pthread_attr_t attr1;
        pthread_attr_init(&attr1);
        //pthread_attr_setscope(&attr1,THREAD_SCOPE_SYSTEM);
       // pthread_attr_setdetachstate(&attr1,THREAD_CREATE_JOINABLE);
        ifstream fin("data.in");
        ofstream fout("data.out");
        thread_arguments arg;

        //read in arr
        fin>>arg.size;
        arg.arr = new int [arg.size];
        for(int i=0;i<arg.size;i++)
                fin>>arg.arr[i];


        int rc = pthread_create (&t1, &attr1, MergeSort, &arg);
        if (rc)
        {
                cout<<"ERROR; return code from pthread_create(t1) is "<<rc<<endl;
                exit(-1);
        }
        //void *status;
        rc = pthread_join(t1,NULL);
        if(rc)
        {
                cout<<"ERROR; return code from pthread_join(t1)is "<<rc<<endl;
                exit(-1);
        }
        else
                {
                //display arr
                for(int i=0;i<arg.size;i++)
                        fout<<arg.arr[i]<<' ';
                }

        delete [] arg.arr;

        //timer stops here
        end = clock();
        elapsed = ((double) (end - start)) /
        CLOCKS_PER_SEC * 1000;
        printf("Time used: %f millisecond\n", elapsed);
        return 0;
}

void *MergeSort(void *args)
{
        thread_arguments *a;
        a = (thread_arguments *)args;
        _MergeSort(a->arr,0,a->size-1);
        pthread_exit(NULL);
}

void _MergeSort(int *arr, int low, int high)
{
/*******************************
        arr: the array to be mergesorted
        low: the lowest index of arr
        high: the highest index of arr

        This function is recursive. Firstly
        mergesort the former half part of
        arr, then the latter half part.
        Finally merge the two part together.
*******************************/
        if(low == high) return;
        int mid = (high + low) / 2;
        _MergeSort(arr,low,mid);
        _MergeSort(arr,mid+1,high);
        Merge(arr,low,mid+1,high);
}

void Merge(int *arr, int low, int mid, int high)
{
/*******************************
arr: the array to be merged
low: the lowest index of arr
mid: the middle index of arr
high: the highest index of arr

This function is to merge the former
and latter part of arr.
*******************************/
        int *result = new int [high - low+ 1];//the temp result of the merged array
        int i=low,j=mid,k=0;

        //compare the former and latter part of arr,and save the less one into result
        while(i<mid && j<=high)
                if(arr[i]<arr[j])
                        result[k++] = arr[i++];
                else result[k++] = arr[j++];

        //move the rest to result
        while(i<mid)    result[k++] = arr[i++];
        while(j<=high)   result[k++] = arr[j++];

        //move result to arr,and delete result
        for(i=0,k=low;k<=high;)
                arr[k++] = result[i++];

        delete [] result;
        result = NULL;
}
