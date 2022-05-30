#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <Windows.h>
#include "fileManipulations.h"

// const char* kBinaryFileExtention = ".txt";
const char* kBinaryFileExtention = ".bin";
const char* kSenderExe =  "sender.exe";

const int MessageLength = 20;

HANDLE START_PROCESS(char*);

int main()
{
    char binFileNameBuffer[100];
    int numberOfRecords = 0;
    
    printf_s("Enter the binary file name:\t");
    scanf_s("%[^\n]%*c", binFileNameBuffer);
    
    do
    {
        printf_s("\nEnter number of records:\t");
        scanf_s("%d",  &numberOfRecords);

        if (numberOfRecords <= 0)
        {
            printf_s("Invalid input! Number of records must be greater than one!\n");
        }
    } while (numberOfRecords <= 0);

    char path[100 + 4 + 1];
    int len = sprintf_s(path, "%s%s", binFileNameBuffer, kBinaryFileExtention);
    if (len == -1)
    {
        perror("Error! Failed creating a path string!\n");
        return ECANCELED;
    }
    
    // FILE *fptr = fopen(path, "w"); 
    FILE *fptr = fopen(path, "wb"); 
    fclose(fptr);

    int numberOfProcesses = 0;

    do
    {
        printf_s("\nEnter number of processes:\t");
        scanf_s("%d", &numberOfProcesses);

        if (numberOfProcesses <=  0)
        {
            printf_s("Invalid input! Number of processes must be greated than zero");
        }
    } while (numberOfProcesses <= 0);

    HANDLE* senderHandle = (HANDLE*)malloc(sizeof(HANDLE) * numberOfProcesses); 

    HANDLE mutex = CreateMutex(NULL, FALSE, "MUTEX");
    HANDLE writeSemaphore = CreateSemaphore(NULL, numberOfRecords, numberOfRecords, "WRITE_SEMAPHORE");
    HANDLE readSemaphore = CreateSemaphore(NULL, 0, numberOfRecords, "READ_SEMAPHORE");
    
   if ((mutex && writeSemaphore && readSemaphore) == NULL)
   {
       perror("Error! Failed creating synchronization objects\n");
       return GetLastError();
   } 

    // create processes
    HANDLE* readinessFlag = (HANDLE*)malloc(sizeof(HANDLE) * numberOfProcesses);

    char charPathBuffer[256];
    len = sprintf_s(charPathBuffer, "%s", path);
    for (int i = 0; i < numberOfProcesses; i++)
    {
        char commandLineArguments[250];
        len = sprintf_s(commandLineArguments, "%s %s %d %d", 
            kSenderExe, charPathBuffer, numberOfRecords, i);
        if (len == -1)
        {
            perror("Error! Failed creating a command line arguments string!\n");
            return ECANCELED;
        }
        printf_s("%i: %s\n", i, commandLineArguments);

        // creating ready 
        char readinessEventName[256];
        len = sprintf_s(readinessEventName, "%d READY", i);
        if (len == -1)
        {
            perror("Error! Failed creating a readiness event name string!\n");
            return ECANCELED;
        }
        HANDLE ready = CreateEvent(NULL, FALSE, FALSE, readinessEventName);
        if (ready == NULL)
        {
            perror("Error! Failed creating event!\n");
            return GetLastError();
        }
        readinessFlag[i] = ready; 

        HANDLE hProcess = START_PROCESS(commandLineArguments);
        if (!hProcess)
        {
            perror("Error! Failed to start a process!\n");
            return GetLastError();
        }
        senderHandle[i] = hProcess;
    }

    // wait for all processes
    WaitForMultipleObjects(numberOfProcesses, readinessFlag, TRUE, INFINITE);

    // logic loop
    char messageBuffer[MessageLength];
    int userChoice = -1;
    while (true)
    {
        memset(messageBuffer, 0, MessageLength);
        printf_s("1. Read message\n2. Exit\n");
        scanf_s("%d", &userChoice);
        
        if (userChoice != 1 && userChoice != 2)
        {
            printf_s("Invalid choice! Please choose between option 1 and 2:\n");
            continue;
        }
        if (userChoice == 2)
        {
            break;
        }
        WaitForSingleObject(readSemaphore, INFINITE);
        WaitForSingleObject(mutex, INFINITE);

        
        // fptr = fopen(path, "r");
        fptr = fopen(path, "rb");
        fseek(fptr, 0, SEEK_SET);
        // fscanf(fptr, "%s", messageBuffer);
        fread(messageBuffer, MessageLength, 1, fptr);
        printf_s("\nNew message:\t%s\n", messageBuffer);

        // delete read message
        fseek(fptr, 0, SEEK_END); // move cursor to the end of file
        int n = ftell(fptr); // number of positions (bytes?) in a file
        fseek(fptr, 0, SEEK_SET);
        char *fileBuffer = (char*)malloc(sizeof(char) * n);
        fread(fileBuffer, n, 1, fptr);
        fclose(fptr);

        fptr = fopen(path, "wb");
        fwrite(fileBuffer + MessageLength, n - MessageLength, 1, fptr);
        fclose(fptr);

        free(fileBuffer);
        ReleaseMutex(mutex);
        ReleaseSemaphore(writeSemaphore, 1, NULL);        
    }
    
    for (int i = 0; i < numberOfProcesses; i++)
    {
        CloseHandle(senderHandle[i]);
        CloseHandle(readinessFlag[i]);
    }

    free(readinessFlag);
    CloseHandle(mutex);
    CloseHandle(writeSemaphore);
    CloseHandle(readSemaphore);
    free(senderHandle);
    free(fptr);
    return 0;
}

HANDLE START_PROCESS(char* args)
{
    STARTUPINFO startupInfo;
    PROCESS_INFORMATION processInformation;

    ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
    startupInfo.cb = sizeof(startupInfo);

    if (!CreateProcess(NULL, args, NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL, NULL, &startupInfo, &processInformation))
    {
        return NULL;
    }

    CloseHandle(processInformation.hThread);
    return processInformation.hProcess;
}

