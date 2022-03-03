#include "Employee.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

using std::cout;
using std::cin;

int main(int argc, char* argv[])
{
	/*std::ofstream output;
	output.open(argv[0], std::ofstream::out | std::ofstream::binary);*/
	// printf("number of params: %d\nFirst param: %s\n", argc, argv[1]);
	FILE* output = fopen(argv[1], "wb");
	// cout << argv[2];
	
	int number_of_records = atoi(argv[2]);
	for (int i = 0; i < number_of_records; i++)
	{
		Employee tmp;
		cout << "Enter employee's ID:\t";
		cin >> tmp.ID;
		cout << "Enter employee's name:\t";
		cin >> tmp.name;
		cout << "Enter Emplyee's working hours:\t";
		cin >> tmp.hours;
		cout << std::endl;
		fwrite(&tmp, sizeof(Employee), 1, output);
		//output.write((char*)&tmp, sizeof(tmp));
	}
	//output.close();
	fclose(output);
	return 0;
}