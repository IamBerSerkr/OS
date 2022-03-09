#include "array.h"
#include <cstdio>
#include <cerrno>
#include <Windows.h>

extern int errno;

DWORD WINAPI min_max(LPVOID);
DWORD WINAPI average(LPVOID);

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
    my_array->size = size;
    my_array->min_id = 0;
    my_array->max_id = 0; 

    HANDLE hThread_min_max;
    HANDLE hThread_average;
    DWORD IDThread_min_max;
    DWORD IDThread_average;

    hThread_min_max = CreateThread(NULL, 0, min_max, (void*)my_array, 0, &IDThread_min_max);    
    hThread_average = CreateThread(NULL, 0, average, (void*)my_array, 0, &IDThread_average);
    if (hThread_min_max == NULL || hThread_average == NULL) {
		return GetLastError();
	}
    WaitForSingleObject(hThread_min_max, INFINITE);
    WaitForSingleObject(hThread_average, INFINITE);
    
    CloseHandle(hThread_min_max);
    CloseHandle(hThread_average);
    
    replaceMinMax(my_array);
    print(my_array);
    
    delete[] arr;
    delete my_array;
    return 0;
}

#include <iostream>
using std::cout;

DWORD WINAPI min_max(LPVOID param)
{
    array* p_arr = (array*)param;
    int min = p_arr->arr[0];
    int max = min;
    
    for (int i = 1; i < p_arr->size; i++)
    {
        if (min > p_arr->arr[i])
        {
            min = p_arr->arr[i];
            p_arr->min_id = i;
        }
        Sleep(7);
        if (max < p_arr->arr[i])
        {
            max = p_arr->arr[i];
            p_arr->max_id = i;
        }
        Sleep(7);
    }

    printf("The maximum value in the array:\t%d\n", max);
    printf("The minimum value in the array:\t%d\n", min);
    
    return 0;
}

DWORD WINAPI average(LPVOID param)
{
    array* p_arr = (array*)param;
    int sum = p_arr->arr[0];
    for (int i = 1; i < p_arr->size; i++)
    {
        sum += p_arr->arr[i];
        Sleep(12);
    }
    p_arr->average = (double)sum / p_arr->size;
    
    printf("The average value in the array:\t%lf\n", p_arr->average);
    return 0;
}
