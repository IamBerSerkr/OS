#include <Windows.h>
#include <conio.h>
#include <cstdio>
#include "util.hpp"

int main(int argc, char* argv[])
{
    HANDLE hReadinessEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, argv[1]);
    HANDLE hStartEvent = OpenEvent(SYNCHRONIZE, FALSE, "START_ALL");
    if(NULL == hStartEvent || NULL == hReadyEvent){
        perror("Error in opening start/ready Event.\n");
        getch();
        return GetLastError();
    }
    SetEvent(hReadyEvent);
    printf("Process is ready.\n");
    WaitForSingleObject(hStartEvent, INFINITE);

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
    messaging(hPipe);    

    CloseHandle(hPipe);
    CloseHandle(hReadinessEvent);
    CloseHandle(hStartEvent);

    return 0;
}