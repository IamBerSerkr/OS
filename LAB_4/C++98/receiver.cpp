#include <cstdio>
#include <Windows.h>

int main()
{
    char binFileNameBuffer[100];
    int numberOfRecords = 0;
    
    printf_s("Enter the binary file name:\t");
    scanf_s("%[^\n]%*c", binFileNameBuffer);
    
    do
    {
        printf_s("\nEnter number of records:\t");
        scanf_s("%d",  &numberOfRecords);

        if (numberOfRecords <= 0)
        {
            printf_s("Invalid input! Number of records must be greater than one!\n");
        }
    } while (numberOfRecords <= 0);


    return 0;
}
