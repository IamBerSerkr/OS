#define _CRT_SECURE_NO_WARNINGS
#include "Employee.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[])
{
	std::ifstream input;
	std::ofstream output;
	input.open(argv[1], std::ifstream::in | std::ifstream::binary);
	output.open(argv[2], std::ofstream::out);
	if (!input.is_open())
	{
		std::cerr << "Failed to open file!\n";
		return 2;
	}
	output << "FILE REPORT:" << argv[1] << std::endl;

	FILE* input_t = fopen(argv[1], "rb");

	Employee buffer;
	// while (input.read((char*)&buffer, sizeof(Employee)))
	while (fread(&buffer, sizeof(Employee), 1, input_t))
	{
		output << buffer.ID << "," << buffer.name <<
			"," << buffer.hours << ","
			<< buffer.hours * atoi(argv[3]) << std::endl;
	}

	fclose(input_t);
	input.close();
	output.close();
	return 0;
}