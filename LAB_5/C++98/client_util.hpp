#include "libraries.hpp"

const char* kClient = "client.exe";
const char* lpPipeName = "\\\\.\\pipe\\pipeName"; 
const int kCommandSize = 10;
const int kErrorID = -1;


DWORD WINAPI client_messaging(LPVOID p)
{
    HANDLE hPipe = (HANDLE)p;
    printf("To quit press CTRL+Z\n");

    // logic loop
    while(true)
    {
        char commandBuffer[kCommandSize];
        printf("\nEnter r/w (read/write) followed by an ID of the employee:\n");
        fgets(commandBuffer, sizeof(kCommandSize), stdin);
        printf("\n%s\n", commandBuffer);

        getchar();

        if (CTRL_Z == commandBuffer[0]) 
        {
            printf("Quiting...");
            getch();
            break;
        }

        // sending the request
        DWORD bytesWritten;
        if (!WriteFile(hPipe, commandBuffer, kCommandSize, &bytesWritten, NULL))
        {
            printf("Failed sending the message!\n");
            getch();
            return 1;
        }

        // receiving an answer
        employee targetEmployee;
        DWORD readBytes;
        if (!ReadFile(hPipe, &targetEmployee, sizeof(targetEmployee), &readBytes, NULL))
        {
            printf("Error! Failed to receive the answer!\n");
            continue;
        }
        else
        {
                if (kErrorID == targetEmployee.id)
            {
                printf("Employee not found or is being modyfied!\n");
                continue;
            }
            
            printEmployee(targetEmployee);
            printf("\n");


            if ( 'w' != commandBuffer[0])
            {
                continue;
            }
            targetEmployee = ReadEmployeeData();
            
            if (WriteFile(hPipe, &targetEmployee, sizeof(targetEmployee), &bytesWritten, NULL))
            {
                printf("New data is parsed to the server.\n");
            }
            else
            {
                printf("Failed to send the information.\n");
                getch();
                break;
            }
        }
    }

    return 0;
}
