#include <cstdio>
#include <Windows.h> 
#include "threadInfo.h"

CRITICAL_SECTION cs;

int* createNewArray(const int& dimentions);
void PrintArray(int* const &, const int&);
DWORD WINAPI markerFucntion(LPVOID params);

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

    InitializeCriticalSection(&cs);
        
    HANDLE* hThread = new HANDLE[numberOfMakers];
    threadInfo* markerInfo = new threadInfo[numberOfMakers];

    bool* terminatedThreads = new bool[numberOfMakers];
    HANDLE* halteredThreads = new HANDLE[numberOfMakers];
    
    HANDLE start = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (start == NULL)
    {
        printf("Failed Starting an event!\n");
        return GetLastError();
    }

    for (int i = 0; i < numberOfMakers; i++)
    {
        HANDLE stop = CreateEvent(NULL, TRUE, FALSE, NULL);

        HANDLE *state = new HANDLE[2];
        state[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
        state[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
        markerInfo[i] = threadInfo(arr, dimentions, i + 1, start,
        stop, state);
        hThread[i] = CreateThread(NULL, 0, markerFucntion, (LPVOID)(&markerInfo[i]), NULL, NULL);
    }
    
    if (!SetEvent(start))
    {
        printf("Failed Starting an event!\n");
        return GetLastError();
    }

    int numberOfHalteredThreads = 0;
    while (numberOfHalteredThreads != numberOfMakers)
    {
        WaitForMultipleObjects(numberOfMakers, halteredThreads,
            TRUE, INFINITE);
        printf("All threads are halted:\n");
        PrintArray(arr, dimentions);

        int id = -1;
        do 
        {
            printf("\nEnter an id of a maker thread to kill:\t");
            scanf("%d", &id);
            if (id > numberOfMakers || id <= 0)
            {
                printf("\nPlease enter valid id!\n");
                continue;
            }
            if (terminatedThreads[id - 1] == true)
            {
                printf("\nThe thread has been already terminated!\n");
                continue;
            }
        }
        while (id <= 0 || id > numberOfMakers || terminatedThreads[id - 1]);

        SetEvent(markerInfo[id-1].TerminateOrContinue[0]);
        WaitForSingleObject(hThread[id-1], INFINITE);
        PrintArray(arr, dimentions);

        terminatedThreads[id-1] = true;
        numberOfHalteredThreads++;

        for (int i = 0; i < numberOfMakers; ++i) {
			if (!terminatedThreads[i]) {
				ResetEvent(markerInfo[i].Stop);
				SetEvent(markerInfo[i].TerminateOrContinue[1]);
			}
		}
    }
    printf("All marker threads have been terminated!\n");

    delete[] arr;
    delete[] hThread;
    delete[] markerInfo;
    delete[] halteredThreads;
    delete[] terminatedThreads;

    DeleteCriticalSection(&cs);
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
    for (int i = 0; i < size - 1; i++)
    {
        printf("%d\t", arr[i]);
    }
    printf("%d\n", arr[size - 1]);
    return;    
}

DWORD WINAPI markerFucntion(LPVOID params)
{
    threadInfo* info = (threadInfo*)params;
    srand(info->idx);
    bool halt_thread = false;
    int numberOfMarkedElements = 0;

    // printf("\nThread #%d started.\n", info->idx);
    WaitForSingleObject(info->Start, INFINITE);
    int index = rand() % info->size;
    while (!halt_thread)
    {
        index = rand() % info->size;
        EnterCriticalSection(&cs);
        if (info->arr[index] == 0)
        {
            Sleep(5);
            // printf("thread #%d: index: %d, changed %d to %d\n", info->idx, index,
            //  info->arr[index], info->idx);
			info->arr[index] = info->idx;
			LeaveCriticalSection(&cs);
			numberOfMarkedElements++;
			Sleep(5);
        }
        else
        {
            // printf("thread #%d: Number of marked elements:\t%d;Can not mark the elenemt with index %d\n", 
            // info->idx, numberOfMarkedElements, index);
            LeaveCriticalSection(&cs);
            SetEvent(info->Stop);

            int mainsResponse = WaitForMultipleObjects(2, info->TerminateOrContinue, FALSE, INFINITE) - WAIT_OBJECT_0;
            if (mainsResponse == 0)
            {
                halt_thread = true;
            }
        }
    }

    for (int i = 0; i < info->size; i++)
    {
        if (info->arr[i] == info->idx)
        {
            info->arr[i] = 0;
        }
    }
    return 0;
}
