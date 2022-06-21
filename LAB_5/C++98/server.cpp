#include "server_util.hpp"

int main(void)
{
    char filename[50];
    printf("Enter the filename:\n");
    scanf("%s", filename);
    do
    {
        printf("Enter the number of employees:\n");
        scanf("%d", &numOfEmployees);
        if (numOfEmployees <= 0)
        {
            printf("Invalid input! Number of employees must be greater than zero!\n");
        }
    } while (numOfEmployees <= 0);
    
    listOfEmployees = (employee*)malloc(sizeof(employee) * numOfEmployees);
    for (int i = 0; i < numOfEmployees; i++)
    {
        listOfEmployees[i] = ReadEmployeeData(i);
    }
    WriteEmployeeData(filename, listOfEmployees, numOfEmployees);
    sortEmployees(listOfEmployees, numOfEmployees);
   
    InitializeCriticalSection(&cs);

    int clientCount = 2 + rand() % 3;
    
    HANDLE hStartALLClients = CreateEvent(NULL, TRUE, FALSE, "START_ALL");

    isModifying = (bool*)malloc(sizeof(bool) * numOfEmployees);
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
    OPEN_PIPES(clientCount);

    printf("\n");
    printEmployees(listOfEmployees, numOfEmployees);
    printf("\nPress any key to exit!\n");
    getch();
    
    WriteEmployeeData(filename, listOfEmployees, numOfEmployees);

    DeleteCriticalSection(&cs);

    free(hReadinessEvent);
    free(isModifying);
    free(listOfEmployees);
    return 0;
}