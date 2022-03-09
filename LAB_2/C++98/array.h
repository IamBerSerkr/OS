#pragma once
#include <cstdio>

struct array
{
    int* arr;
    int size;
    int min_id;
    int max_id;
    double average;
};

void replaceMinMax(array* p_arr)
{
    p_arr->arr[p_arr->min_id] = p_arr->average;
    p_arr->arr[p_arr->max_id] = p_arr->average;
    return;
}

void print(array* p_arr)
{
    for (int i = 0; i < p_arr->size-1; i++)
    {
        printf("%d\t", p_arr->arr[i]);
    }
    printf("%d\n", p_arr->arr[p_arr->size - 1]);
}