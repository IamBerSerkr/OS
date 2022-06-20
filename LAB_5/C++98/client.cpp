#include <Windows.h>
#include <conio.h>
#include <cstdio>
#include "client_util.hpp"

int main(int argc, char* argv[])
{
    printf("Howdy!\n");
    getch();
    HANDLE hReadinessEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, argv[1]);
    HANDLE hStartEvent = OpenEvent(SYNCHRONIZE, FALSE, "START_ALL");
    if(NULL == hStartEvent || NULL == hReadinessEvent){
        perror("Error in opening start/ready Event.\n");
        getch();
        return GetLastError();
    }
    SetEvent(hReadinessEvent);
    printf("The Process is ready.\n");
    WaitForSingleObject(hStartEvent, INFINITE);
    printf("The process has just started.\n");
    // connecting to the pipe
    HANDLE hPipe = NULL;
    while (true)
    {
        hPipe = CreateFile(lpPipeName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ,
            NULL, OPEN_EXISTING, 0, NULL);
        
        if (INVALID_HANDLE_VALUE != hPipe)
        {
            break;
        }
        if (!WaitNamedPipe(lpPipeName, 5000))
        {
            printf("5 second timed out!\n");
            getch();
            return 1;
        }
    }

    printf("Connected to the pipe!\n");
    client_messaging(hPipe);    

    CloseHandle(hPipe);
    CloseHandle(hReadinessEvent);
    CloseHandle(hStartEvent);

    return 0;
}