#include <cstdio>
#include <Windows.h> 

int* createNewArray(const int& dimentions);
void PrintArray(const int*&, const int&);

int main()
{
    int dimentions = 0; 
    do
    {
        scanf("%d", &dimentions);
    } 
    while (dimentions <= 0);
    
    int *arr = createNewArray(dimentions);
    for (int i = 0; i < dimentions; i++)
    {
        printf("%d\n", arr[i]);
    }
    
    int numberOfMakers = 0;
    do
    {
        scanf("%d", &numberOfMakers);
    } 
    while (numberOfMakers <= 0);
    
    if (!SetEvent(NULL))
    {
        return GetLastError();
    }
    

    HANDLE* halteredThreads = new HANDLE[numberOfMakers];

    int numberOfHalteredThreads = 0;
    while (numberOfHalteredThreads != numberOfMakers)
    {
        WaitForMultipleObjects(numberOfMakers, halteredThreads,
            TRUE, INFINITE);
        PrintArray(arr, dimentions);
    }
    

    delete[] arr;
    delete[] halteredThreads;

    return 0;
}

int* createNewArray(const int& dimentions)
{
    int *arr = new int[dimentions];
    for (int i = 0; i < dimentions; i++)
    {
        arr[i] = 0;
    }
    return arr;
}

void PrintArray(const int*& arr, const int& size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d\t", arr[i]);
    }
    printf("%d\n", arr[size - 1]);
    return;    
}
