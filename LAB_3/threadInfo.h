#pragma once
#include "Windows.h"

struct threadInfo
{
    int *arr;
    int size;
    int idx;
    HANDLE Start;
    HANDLE Stop;
    HANDLE Terminate;
    HANDLE Continue;

    threadInfo(void);
    threadInfo(int* const &, const int&, const int&,
        const HANDLE&, const HANDLE&, const HANDLE&, const HANDLE&);
};

threadInfo::threadInfo(void)
{
    this->size = 0;
    this->arr = nullptr;
    this->idx = -1;
    this->Start = NULL;
    this->Stop = NULL;
    this->Terminate = NULL;
    this->Continue = NULL;
}

threadInfo::threadInfo(int* const & arr, const int& size, const int& idx,
        const HANDLE& Start, const HANDLE& Stop, 
        const HANDLE& Terminate, const HANDLE& Continue)
        :arr(arr), size(size), idx(idx), 
        Start(Start), Stop(Stop), Terminate(Terminate), Continue(Continue){}