#include <cstdio>
#include <cstring>
#include <Windows.h>

const char* binaryFileExtention = ".bin";

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

    char *path = nullptr;
    strcpy(path, binFileNameBuffer);
    strcat(path, binaryFileExtention);
    FILE *fptr = fopen(path, "wb"); 
    fclose(fptr);

    int numberOfProcesses = 0;

    do
    {
        printf_s("\nEnter number of processes:\t");
        scanf_s("%d", &numberOfProcesses);

        if (numberOfProcesses <=  0)
        {
            printf_s("Invalid input! Number of processes must be greated than zero");
        }
    } while (numberOfProcesses <= 0);

    HANDLE* senderHandle = (HANDLE*)malloc(sizeof(HANDLE) * numberOfProcesses); 


    HANDLE mutex = CreateMutex(NULL, FALSE, L"MUTEX");
    
    free(senderHandle);
  
    return 0;
