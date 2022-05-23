#include <cstdio>
#include <cstring>
#include <cwchar>
#include <cstdlib>
#include <cerrno>
#include <exception>
#include <Windows.h>

const char* kBinaryFileExtention = ".bin";
const wchar_t* kSenderExe =  L"sender.exe";

HANDLE START_PROCESS(const wchar_t*);

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
    int len = sprintf_s(path, "%s.bin", binFileNameBuffer);
    if (len == -1)
    {
        perror("Error! Failed creating a path string!\n");
        return ECANCELED;
    }
    strcpy(path, binFileNameBuffer);
    strcat(path, kBinaryFileExtention);
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

    wchar_t wcharPathBuffer[256];
    mbstowcs(wcharPathBuffer, path, 256);
    for (int i = 0; i < numberOfProcesses; i++)
    {
        wchar_t commandLineArguments[250];
        len = swprintf_s(commandLineArguments, L"%s %s %d %d", 
            kSenderExe, wcharPathBuffer, numberOfRecords, i);
        if (len == -1)
        {
            perror("Error! Failed creating a command line arguments string!\n");
            return ECANCELED;
        }
        // wprintf_s(L"%i: %ls\n", i, commandLineArguments);

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

    free(readinessFlag);
    CloseHandle(mutex);
    CloseHandle(writeSemaphore);
    CloseHandle(readSemaphore);
    free(senderHandle);
  
    return 0;
}

HANDLE START_PROCESS(const wchar_t* args)
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

