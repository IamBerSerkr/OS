#include <cstdio>
#include <Windows.h>


int main(int argc, char* argv[])
{
    // gain control over Synchronization Objects
    HANDLE readSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS,
        FALSE, "READ_SEMAPHORE");
    HANDLE writeSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, 
        FALSE, "WRITE_SEMAPHORE");
    HANDLE mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "MUTEX");

    char readinessEventName[256];
    int len = sprintf_s(readinessEventName, "%s READY", argv[3]);
    // printf_s("%s\n", &readinessEventName);
    if (len == -1)
    {
        perror("Error! Failed creating a readiness event name string!\n");
        return ECANCELED;
    }
    HANDLE readiness = OpenEvent(EVENT_ALL_ACCESS, 
        FALSE, readinessEventName);

    bool failedToStart = !readSemaphore || !writeSemaphore ||
        !mutex || !readiness;
    if (failedToStart)
    {
        perror("Error! Failed gaining control over synchronization objects!\n");
        return GetLastError();
    }

    SetEvent(readiness);
    
    FILE* fptr = fopen(argv[1], "a");
    
    // FILE* fptr = fopen(argv[1], "wb+");
    int userChoice = -1;

    char messageBuffer[21];

    while(true)
    {
        printf_s("1. Write message\n2. Exit\n");
        scanf_s("%d", &userChoice);
        
        if (userChoice != 1 && 
        userChoice != 2)
        {
            printf_s("Invalid choice! Please choose between option 1 and 2:\n");
            continue;
        }
        if (userChoice == 2)
        {
            break;
        }
        
        printf_s("Enter message[20 characters max]:\t");        
        scanf_s("%s", messageBuffer);

        WaitForSingleObject(writeSemaphore, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        
        fprintf_s(fptr, "%s\n", messageBuffer);
        printf_s("The Message: %s\n", messageBuffer);

        ReleaseMutex(mutex);
        ReleaseSemaphore(readSemaphore, 1, NULL);
        printf_s("Message has been written successfully!\n");
    }

    fclose(fptr);
    free(fptr);
    CloseHandle(readiness);
    CloseHandle(mutex);
    CloseHandle(writeSemaphore);
    CloseHandle(readSemaphore);
    return 0;
}