#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>

const char* CREATOR = "creator.exe";
const char* REPORTER = "reporter.exe";

void start_creator(const std::string& binary_name, const int& record_count);
void start_reported(const std::string&, const std::string&, const double&);

int main()
{
	std::string input_bin_filename;
	long long record_count = 0;

	std::cout << "Enter binary file name:\t";
	std::cin >> input_bin_filename;
	std::cout << "Enter record count:\t";
	std::cin >> record_count;

	start_creator(input_bin_filename, record_count);


	std::string report_filename;
	double hourly_pay = 0.0;
	std::cout << "\nEnter the report filename:\t";
	std::cin >> report_filename;
	std::cout << "Enter the hourly pay:\t";
	std::cin >> hourly_pay;

	start_reported(input_bin_filename, report_filename, hourly_pay);

	return 0;
}

void start_creator(const std::string& binary_name, const int& record_count)
{
	STARTUPINFO startup_info;
	PROCESS_INFORMATION process_info;

	ZeroMemory(&startup_info, sizeof(startup_info));
	startup_info.cb = sizeof(startup_info);

	std::stringstream sstream;
	sstream << CREATOR  << " " << binary_name << " " << record_count;
	std::cout << sstream.str() << "\n";

	if (!CreateProcess(NULL, (char*)sstream.str().c_str(),
		NULL, NULL, FALSE, CREATE_NEW_CONSOLE,
		NULL, NULL, &startup_info, &process_info))
	{
		std::cout << "Failed to start a process!\n";
	}
	std::cout << "Process started!\n";
	WaitForSingleObject(process_info.hProcess, INFINITE);
	std::cout << "Process stoped!\n";
}


void start_reported(const std::string& input_bin_filename, 
	const std::string& report_filename, const double& hourly_pay)
{
	STARTUPINFO startup_info;
	PROCESS_INFORMATION process_info;

	ZeroMemory(&startup_info, sizeof(startup_info));
	startup_info.cb = sizeof(startup_info);

	std::stringstream sstream;
	sstream << REPORTER << " " << input_bin_filename << " " << report_filename <<
		" " << hourly_pay;

	if (!CreateProcess(NULL, (char*)sstream.str().c_str(),
			NULL, NULL, FALSE, CREATE_NEW_CONSOLE,
			NULL, NULL, &startup_info, &process_info))
		{
			std::cout << "Failed to start a process!\n";
		}
	else
	{
		std::cout << "Process started!\n";
		WaitForSingleObject(process_info.hProcess, INFINITE);
		std::cout << "Process stoped!\n";
	}	
}
