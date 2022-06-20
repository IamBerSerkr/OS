#include "libs.hpp"


const char* kClient = "client.exe";
const char* lpPipeName = "\\\\.\\pipe\\pipeName"; 
const int kCommandSize = 10;

DWORD WINAPI client_messaging(LPVOID p)
{
    HANDLE* hPipe = (HANDLE*)p;
    printf("To quit press CTRL+Z\n");

    // logic loop
    while(true)
    {
        char commandBuffer[kCommandSize];
        scanf("%s", commandBuffer);
        // printf("\n%s\n", commandBuffer);

        if (commandBuffer[0] = CTRL_Z) 
        {
            printf("Quiting...");
            getch();
            break;
        }

        // sending the request
        DWORD bytesWritten;
        if (!WriteFile(*hPipe, commandBuffer, kCommandSize, &bytesWritten, NULL))
        {
            perror("Failed sending the message!\n");
            getch();
            return 1;
        }

        // receiving an answer
        

    }

    return 0;
}
