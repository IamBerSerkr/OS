#include "array.h"
#include <cstdio>
#include <cerrno>
#include <Windows.h>

extern int errno;

DWORD WINAPI min_max(LPVOID arr);
DWORD WINAPI average(LPVOID arr);

int main(void)
{
    int size = 0;

    printf("Enter the dimentions of an array: ");
    scanf("%d", &size);
    if (size <= 0)
    {   
        errno = EDOM;
        perror("Error:\t");
        return EDOM;
    }

    int *arr = (int*)malloc(sizeof(int) * size);
    if (arr == NULL)
    {
        errno = ENOMEM;
        perror("Error:\t");
        return ENOMEM;
    }
    for (int i = 0; i < size; i++)
    {
        scanf("%d", &arr[i]);
    }

    array *my_array = (array*)malloc(sizeof(array));
    my_array->arr = arr;

    HANDLE hThread_min_max;
    HANDLE hThread_average;
    DWORD IDThread_min_max;
    DWORD IDThread_average;

    hThread_min_max = CreateThread(NULL, 0, min_max, (void*)my_array, 0, &IDThread_min_max);    
    hThread_min_max = CreateThread(NULL, 0, min_max, (void*)my_array, 0, &IDThread_min_max);
    WaitForSingleObject(hThread_min_max, INFINITE);
    WaitForSingleObject(hThread_average, INFINITE);
    
    replaceMinMax(my_array);
    print(my_array);
    
    delete[] arr;
    delete my_array;
    return 0;
}