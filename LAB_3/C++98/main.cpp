#include <cstdio>
#include <Windows.h> 
#include "threadInfo.h"

CRITICAL_SECTION cs;

int* createNewArray(const int& dimentions);
void PrintArray(int* const &, const int&);
DWORD WINAPI markerFucntion(LPVOID params);

int main()
{
    // read input from the terminal;
    int dimentions = 0; 
    do
    {
        printf("Enter the size of an array:\t");
        scanf("%d", &dimentions);
    } 
    while (0 >= dimentions);
    
    int *arr = createNewArray(dimentions);
       
    int numberOfMarkers = 0;
    do
    {
        printf("Enter a number of marker threads:\t");
        scanf("%d", &numberOfMarkers);
    } 
    while (0 >= numberOfMarkers);
    
    InitializeCriticalSection(&cs);
        
    HANDLE* hThread = new HANDLE[numberOfMarkers];
    threadInfo* markerInfo = new threadInfo[numberOfMarkers];

    bool* terminatedThreads = new bool[numberOfMarkers];
    HANDLE* halteredThreads = new HANDLE[numberOfMarkers];
    
    HANDLE start = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (NULL == start)
    {
        printf("Failed Starting an event!\n");
        return GetLastError();
    }

    for (int i = 0; i < numberOfMarkers; i++)
    {
        HANDLE stop = CreateEvent(NULL, TRUE, FALSE, NULL);
        HANDLE *state = new HANDLE[2];
        state[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
        state[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
        markerInfo[i] = threadInfo(arr, dimentions, i + 1, start,
        stop, state);
        halteredThreads[i] = markerInfo[i].Stop = CreateEvent(NULL, TRUE, FALSE, NULL);
        hThread[i] = CreateThread(NULL, 0, markerFucntion, (LPVOID)(&markerInfo[i]), NULL, NULL);
        
        terminatedThreads[i] = false;
    }
       
    if (!SetEvent(start))
    {
        printf("Failed Starting an event!\n");
        return GetLastError();
    }
    int numberOfHalteredThreads = 0;
    while (numberOfHalteredThreads != numberOfMarkers)
    {
        WaitForMultipleObjects(numberOfMarkers, halteredThreads,
            TRUE, INFINITE);
        fflush(stdout);
        printf("\nAll threads are halted:\n");
        PrintArray(arr, dimentions);

        int id = -1;
        
        while (0 >= id|| id > numberOfMarkers)
        {
            printf("\nEnter an id of a maker thread to kill:\t");
            scanf("%d", &id);
            if (id > numberOfMarkers || id <= 0)
            {
                printf("\nPlease enter valid id!\n");
                continue;
            }
            if (terminatedThreads[id - 1] == true)
            {
                printf("\nThe thread has been already terminated!\n");
                continue;
            }
            else
            { 
                SetEvent(markerInfo[id-1].TerminateOrContinue[0]);
                WaitForSingleObject(hThread[id-1], INFINITE);
                PrintArray(arr, dimentions);

                terminatedThreads[id-1] = true;
                numberOfHalteredThreads++;
                break;
            }
        }

        for (int i = 0; i < numberOfMarkers; ++i) {
			if (!terminatedThreads[i]) {
				ResetEvent(markerInfo[i].Stop);
				SetEvent(markerInfo[i].TerminateOrContinue[1]);
			}
		}
    }
    printf("All marker threads have been terminated!\n");


    if (!CloseHandle(start))
    {
        printf("Failed to close handle!\n");
        return GetLastError();
    }
    for (int i = 0; i < numberOfMarkers; i++)
    {
        bool closureResult = CloseHandle(hThread[i]) &&  CloseHandle(halteredThreads[i]) &&
            CloseHandle(markerInfo[i].TerminateOrContinue[0]) && CloseHandle(markerInfo[i].TerminateOrContinue[1]);
        if (!closureResult)
        {
            printf("Failed to close handle!\n");
            return GetLastError();
        }
    }


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
    EnterCriticalSection(&cs);
    for (int i = 0; i < size - 1; i++)
    {
        printf("%d\t", arr[i]);
    }
    printf("%d\n", arr[size - 1]);
    LeaveCriticalSection(&cs);
    return;    
}

DWORD WINAPI markerFucntion(LPVOID params)
{
    threadInfo* info = (threadInfo*)params;
    srand(info->idx);
    bool halt_thread = false;
    int numberOfMarkedElements = 0;

    WaitForSingleObject(info->Start, INFINITE);
    int index = -1;
    while (!halt_thread)
    {
        index = rand() % info->size;
        EnterCriticalSection(&cs);
        if (info->arr[index] == 0)
        {
            printf("thread #%d: index: %d, changed %d to %d\n", info->idx, index,
             info->arr[index], info->idx);
			info->arr[index] = info->idx;
            Sleep(5);
			numberOfMarkedElements++;
			LeaveCriticalSection(&cs);
			Sleep(5);
        }
        else
        {
            printf("[REPORT]thread #%d: Number of marked elements:\t%d;\tCan not mark the elenemt with index %d\n", 
            info->idx, numberOfMarkedElements, index);
            fflush(stdout);
            LeaveCriticalSection(&cs);

            SetEvent(info->Stop);
            int mainsResponse = WaitForMultipleObjects(2, info->TerminateOrContinue, FALSE, INFINITE) - WAIT_OBJECT_0;
            if (0 == mainsResponse)
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
