#pragma once
#include "Windows.h"

struct threadInfo
{
    int *arr;
    int size;
    int idx; // index starting from 1
    HANDLE Start;
    HANDLE Stop;
    HANDLE* TerminateOrContinue;

    threadInfo(void);
    threadInfo(int* const &, const int&, const int&,
        const HANDLE&, const HANDLE&, HANDLE*&);

    threadInfo& operator =(const threadInfo&); 
};

threadInfo::threadInfo(void)
{
    this->size = 0;
    this->arr = NULL;
    this->idx = -1;
    this->Start = NULL;
    this->Stop = NULL;
    this->TerminateOrContinue = NULL;
}

threadInfo::threadInfo(int* const & arr, const int& size, const int& idx,
        const HANDLE& Start, const HANDLE& Stop, 
        HANDLE*& TerminateOrContinue)
        :arr(arr), size(size), idx(idx), 
        Start(Start), Stop(Stop), TerminateOrContinue(TerminateOrContinue){}

threadInfo& threadInfo::operator= (const threadInfo& other)
{
    this->arr = other.arr;
    this->size = other.size;
    this->idx = other.idx;
    this->Start = other.Start;
    this->Stop = other.Stop;
    this->TerminateOrContinue = other.TerminateOrContinue;
    return *this;
}
