#include <Windows.h>
#include <conio.h>
#include <cstdio>

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

    CloseHandle(hReadinessEvent);
    CloseHandle(hStartEvent);
}