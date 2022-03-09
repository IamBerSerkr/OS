#pragma once
#include <cstdio>

struct array
{
    double* arr;
    int size;
    int min;
    int max;
    double average;
};

void replaceMinMax(array* p_arr)
{
    for (int i = 0; i < p_arr->size; i++)
    {
        if (p_arr->arr[i] == p_arr->max ||
            p_arr->arr[i] == p_arr->min)
        {
            p_arr->arr[i] = p_arr->average;
        }
    }
    return;
}

void print(array* p_arr)
{
    for (int i = 0; i < p_arr->size-1; i++)
    {
        printf("%lf\t", p_arr->arr[i]);
    }
    printf("%lf\n", p_arr->arr[p_arr->size - 1]);
}