#include <cstdio>
#include <Windows.h> 
#include "threadInfo.h"

int* createNewArray(const int& dimentions);
void PrintArray(int* const &, const int&);

int main()
{
    int dimentions = 0; 
    do
    {
        printf("Enter the size of an array:\t");
        scanf("%d", &dimentions);
    } 
    while (dimentions <= 0);
    
    int *arr = createNewArray(dimentions);
       
    int numberOfMakers = 0;
    do
    {
        printf("Enter a number of marker threads:\t");
        scanf("%d", &numberOfMakers);
    } 
    while (numberOfMakers <= 0);
    
    if (!SetEvent(NULL))
    {
        printf("Failed Starting an event!");
        return GetLastError();
    }
    
    HANDLE* hThread = new HANDLE[numberOfMakers];
    threadInfo* markerInfo = new threadInfo[numberOfMakers];

    bool* terminatedThread = new bool[numberOfMakers];
    HANDLE* halteredThreads = new HANDLE[numberOfMakers];
    
    int numberOfHalteredThreads = 0;
    while (numberOfHalteredThreads != numberOfMakers)
    {
        WaitForMultipleObjects(numberOfMakers, halteredThreads,
            TRUE, INFINITE);
        PrintArray(arr, dimentions);

        int id = -1;
        do 
        {
            printf("Enter an id of a maker thread to kill:\t");
            scanf("%d", &id);
            if (terminatedThread[id] == true)
            {
                printf("The thread has been already terminated!\n");
            }
        }
        while(id < 0 && terminatedThread[id] == true);

        SetEvent(markerInfo[id].Terminate);
        WaitForSingleObject(hThread[id], INFINITE);
        PrintArray(arr, dimentions);

        terminatedThread[id] = true;
        halteredThreads++;
    }
    

    delete[] arr;
    delete[] halteredThreads;
    return 0;
}

int* createNewArray(const int& dimentions)
{
    int *arr = new int[dimentions];
    for (int i = 0; i < dimentions; i++)
    {
        arr[i] = 0;
    }
    return arr;
}

void PrintArray(int* const& arr, const int& size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d\t", arr[i]);
    }
    printf("%d\n", arr[size - 1]);
    return;    
}
