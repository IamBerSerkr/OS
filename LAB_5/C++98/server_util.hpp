#include "libraries.hpp"

const char* kClient = "client.exe";

CRITICAL_SECTION cs;

const char* lpPipeName = "\\\\.\\pipe\\pipeName"; 
const int kCommandSize = 10;

employee* listOfEmployees = NULL;
int numOfEmployees = 0;
bool *isModifying = NULL;

HANDLE START_PROCESS(char* args)
{
    STARTUPINFO startupInfo;
    PROCESS_INFORMATION processInformation;

    ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
    startupInfo.cb = sizeof(startupInfo);

    if (!CreateProcess(NULL, args, NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL, NULL, &startupInfo, &processInformation))
    {
        printf("Error! Failed to create a process!\n");
        CloseHandle(processInformation.hThread);
        CloseHandle(processInformation.hProcess);
        return NULL;
    }

    CloseHandle(processInformation.hThread);
    return processInformation.hProcess;
}

const int kErrorID = -1;

DWORD WINAPI server_messaging(LPVOID p)
{
    HANDLE hPipe = (HANDLE)p;

    employee* errorEmployee = (employee*)malloc(sizeof(employee));
    errorEmployee->id = -1;
    
    // logic loop
    while(true)
    {
        // receiving a request
        DWORD readBytes;
        char commandBuffer[kCommandSize];
        if (!ReadFile(hPipe, commandBuffer, kCommandSize, &readBytes, NULL))
        {
            DWORD lastError = GetLastError();
            if (lastError == ERROR_BROKEN_PIPE)
            {
                printf("Clinet disconnected!\n");
            }
            else
            {
                printf("Error while reading a message!\n");
            }
            break;
        }

        // sending the answer
        if (strlen(commandBuffer) > 0)
        {
            char mode = '\0';
            int id = -1;

            sscanf(commandBuffer, "%c %d", &mode, &id);
            
            EnterCriticalSection(&cs);
                employee* targetEmployee = findEmployee(listOfEmployees, numOfEmployees, id);
            LeaveCriticalSection(&cs);

            if (targetEmployee == NULL)
            {
                targetEmployee = errorEmployee;
            }
            else
            {
                int arrIndex = targetEmployee - listOfEmployees;

                if (isModifying[arrIndex])
                {
                    targetEmployee = errorEmployee;
                }
                else
                {
                    switch (mode)
                    {
                    case 'r':
                        printf("Read request!\n");
                        break;
                    case 'w':
                        printf("Modidy request!\n");
                        isModifying[arrIndex] = true;
                        break;
                    default:
                        printf("Unknown request!\n");
                        targetEmployee = errorEmployee;
                        break;
                    }
                }
            }
            DWORD bytesWritten;
            if (WriteFile(hPipe, targetEmployee, sizeof(employee), &bytesWritten, NULL))
            {
                printf("The Answer is sent!\n");
            }
            else
            {
                printf("Failed sending the answer!\n");
            }

            // receiving a changed employee data
            if (mode == 'w' && targetEmployee != errorEmployee)
            {
                if (ReadFile(hPipe, targetEmployee, sizeof(employee), &readBytes, NULL))
                {
                    printf("Employee record changed!\n");
                    isModifying[targetEmployee - listOfEmployees] = false;
                    EnterCriticalSection(&cs);
                        sortEmployees(listOfEmployees, numOfEmployees);
                    LeaveCriticalSection(&cs);
                }
                else
                {
                    printf("Failed to read a message!\n");
                    break;
                }
            }
        }
    }

    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
    free(errorEmployee);
    
    return 0;
}


void OPEN_PIPES(const int& numberOfClients)
{
    HANDLE hPipe = NULL;
    HANDLE *hThreads = (HANDLE*)malloc(numberOfClients * sizeof(HANDLE));

    for (int i = 0; i < numberOfClients; i++)
    {
        hPipe = CreateNamedPipe(lpPipeName, PIPE_ACCESS_DUPLEX, 
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES, 0, 0, INFINITE, NULL);

        if (INVALID_HANDLE_VALUE == hPipe)
        {
            printf("Failed to create a named pipe\n");
            getch();
            return;
        }

        if (!ConnectNamedPipe(hPipe, NULL))
        {
            printf("No connected clients!\n");
            getch();
            break;
        }
        hThreads[i] = CreateThread(NULL, 0, server_messaging, (LPVOID)hPipe, 0, NULL);
    }

    printf("All clients have connected to the pipe!\n");
    WaitForMultipleObjects(numberOfClients, hThreads, TRUE, INFINITE);
    printf("All clients have been disconnected from the pipe!\n");
    
    for (int i = 0; i < numberOfClients; i++)
    {
        CloseHandle(hThreads[i]);
    }
    CloseHandle(hPipe);
    free(hThreads);
}








