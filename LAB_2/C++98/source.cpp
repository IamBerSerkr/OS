#include <cstdio>
#include <cerrno>
#include <Windows.h>

extern int errno;

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
    for (int i = 0; i < size; i++)
    {
        printf("%d\t", arr[i]);
    }
    return 0;
}