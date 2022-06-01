#include "Windows.h"
#include "employee.hpp"
#include "util.hpp"
#include <cstdio>
#include <cstdlib>

CRITICAL_SECTION cs;

const char* kClient = "client.exe";

int main(void)
{
    char filename[50];
    printf("Enter the filename:\n");
    scanf("%s", filename);
    int numOfEmployees = 0;
    do
    {
        printf("Enter the number of employees:\n");
        scanf("%d", &numOfEmployees);
        if (numOfEmployees <= 0)
        {
            printf("Invalid input! Number of employees must be greater than zero!\n");
        }
    } while (numOfEmployees <= 0);
    
    employee* listOfEmployees = (employee*)malloc(sizeof(employee) * numOfEmployees);
    for (int i = 0; i < numOfEmployees; i++)
    {
        listOfEmployees[i] = ReadEmployeeData(i);
    }
    WriteEmployeeData(filename, listOfEmployees, numOfEmployees);

    InitializeCriticalSection(&cs);

    int clientCount = 2;
    
    HANDLE hStartALLClients = CreateEvent(NULL, TRUE, FALSE, "START_ALL");

    bool *isModifying = (bool*)malloc(sizeof(bool) * numOfEmployees);
    memset(isModifying, 0, numOfEmployees);

    HANDLE* hReadinessEvent = (HANDLE*)malloc(sizeof(HANDLE) * clientCount);
    for (int i = 0; i < clientCount; i++)
    {
        char eventName[50];
        int len = sprintf(eventName, "READY_%d", i);
        hReadinessEvent[i] = CreateEvent(NULL, TRUE, FALSE, eventName);

        char arguments[50];
        len = sprintf(arguments, "%s %s", kClient, eventName);
        printf("%s\n", arguments);
        if (!START_PROCESS(arguments))
        {
            return GetLastError();
        }
    }

    WaitForMultipleObjects(clientCount, hReadinessEvent, TRUE, INFINITE);
    
    printf("All processes are ready to start.\n");
    SetEvent(hStartALLClients);
    printf("Starting...");

    // creating pipes
    HANDLE hPipe = NULL;
    HANDLE* hThreads = (HANDLE*)malloc(sizeof(HANDLE) * clientCount);
    for (int i = 0; i < clientCount; i++)
    {
        OPEN_PIPE(hPipe);

        if (!ConnectNamedPipe(hPipe, NULL))
        {
            printf("No connected clients!\n");
            break;
        }
        
        hThreads[i] = CreateThread(NULL, 0, messaging, (LPVOID)hPipe, 0, NULL);
    }
    printf("All clients have connected to the pipe!\n");
    WaitForMultipleObjects(clientCount, hThreads, TRUE, INFINITE);
    printf("All clients have been disconnected from the pipe!\n");


    DeleteCriticalSection(&cs);

    CloseHandle(hPipe);
    for (int i = 0; i < clientCount; i++)
    {
        CloseHandle(hThreads[i]);
    }
    free(hThreads);
    free(hReadinessEvent);
    free(isModifying);
    free(listOfEmployees);
    return 0;
}