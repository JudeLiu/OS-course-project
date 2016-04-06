/****************************************************************************
        Description:
                This is a single threaded program to implement mergesort.

        Algorithm:
                Function MergeSort() can sort the former half part of input array and the
                latter half part and then merge them.
                Recrusively invoke fuction MergeSort() can sort the whole array.

        Sample input:
        -------------------------------------------------
        ~/project1$ ./MegesortSingle
        -------------------------------------------------
****************************************************************************/
#include "header.h"
#define INT_MAX 30000
using namespace std;

void MergeSort(int *arr,int size);
void MergeSort(int *arr, int low, int high);
void Merge(int *arr,int low, int mid, int high);

int main()
{
        //timer starts here
        clock_t start, end;
        double elapsed;
        start = clock();

        ifstream fin("data.in");//defne input file
        ofstream fout("data.out");//define output file
        int num,i;

        //read numbers
        fin>>num;
        int *arr = new int [num];
        for(i=0;i<num;i++)
                fin>>arr[i];

        //merge sort
        MergeSort(arr,num);

        //print the mergesorted array
        for(i=0;i<num;i++)
                fout<<arr[i]<<' ';

        fin.close();
        fout.close();
        delete [] arr;
        arr = NULL;

        //timer stops here
        end = clock();
        elapsed = ((double) (end - start)) /
        CLOCKS_PER_SEC * 1000;
        printf("Time used: %f millisecond\n", elapsed);
        return 0;
}

void MergeSort(int *arr,int size)
{
/*******************************
        arr : the array to be mergesorted
        size: the size of arr

        This is the package funtion of MergeSort.
********************************/
        MergeSort(arr,0,size-1);
}

void MergeSort(int *arr, int low, int high)
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
        MergeSort(arr,low,mid);
        MergeSort(arr,mid+1,high);
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
and latter half part of arr.
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
