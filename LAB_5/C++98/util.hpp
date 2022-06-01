#pragma once
#include "Windows.h"
#include <cstdio>
#include <conio.h>

const char* lpPipeName = "\\\\.\pipe\pipeName"; 


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

void OPEN_PIPE(HANDLE& hPipe)
{
    hPipe = CreateNamedPipe(lpPipeName, PIPE_ACCESS_DUPLEX, 
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES, 0, 0, INFINITE, NULL);

    if (INVALID_HANDLE_VALUE == hPipe)
    {
        perror("Failed to create a named pipe\n");
        getch();
        return;
    }
}

DWORD WINAPI messaging(LPVOID ptr)
{
    return 0;
}