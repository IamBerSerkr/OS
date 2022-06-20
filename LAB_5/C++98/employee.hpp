#pragma once
#include <cstdio>
#include <cstdlib>


struct employee
{
    int id;
    char name[10];
    double hours;
};

employee ReadEmployeeData(const int& i)
{
    employee newEmployee;
    printf("%d:\tEnter Employee information:\n[id, name, hours]\n", i + 1);

    scanf("%d %s %lf", &newEmployee.id, newEmployee.name, &newEmployee.hours);

    return newEmployee;
}


void WriteEmployeeData(char* filename, employee* const & data, const int& size)
{
    FILE* fptr = fopen(filename, "wb");
    
    fwrite((char*)data, sizeof(employee) * size, 1, fptr);

    fclose(fptr);
}

int employeeComparator(const void* lhs, const void* rhs)
{
    return ((employee*)lhs)->id - ((employee*)rhs)->id;
}

void sortEmployees(employee* employees, size_t numberOfEmpoyees){
    qsort(employees, numberOfEmpoyees, sizeof(employee), employeeComparator);
}

employee* findEmployee(const employee* employees, size_t numberOfEmpoyees, const int& id){
    employee key;
    key.id = id;
    return (employee*)bsearch((const char*)&key, (const char*)(employees),
                              numberOfEmpoyees, sizeof(employee), employeeComparator);
}
