#include <cstdio>

int* createNewArray(const int& dimentions);

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