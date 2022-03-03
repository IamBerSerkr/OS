#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>

using std::cin;
using std::cout;

const char* CREATOR = "creator.exe";
const char* REPORTER = "reporter.exe";

void start_creator(const std::string& binary_name, const int& record_count);
void start_reported(const std::string&, const std::string&, const double&);

int main()
{
	std::string input_bin_filename;
	long long record_count = 0;

	cout << "Enter binary file name:\t";
	cin >> input_bin_filename;
	cout << "Enter record count:\t";
	cin >> record_count;

	start_creator(input_bin_filename, record_count);


	std::string report_filename;
	double hourly_pay = 0.0;
	cout << "\nEnter the report filename:\t";
	cin >> report_filename;
	cout << "Enter the hourly pay:\t";
	cin >> hourly_pay;

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
	
	if (!CreateProcess(NULL, (char*)sstream.str().c_str(),
		NULL, NULL, FALSE, CREATE_NEW_CONSOLE,
		NULL, NULL, &startup_info, &process_info))
	{
		cout << "Failed to start a process!\n";
	}
	cout << "Process started!\n";
	WaitForSingleObject(process_info.hProcess, INFINITE);
	cout << "Process stoped!\n";
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
			cout << "Failed to start a process!\n";
		}
	else
	{
		cout << "Process started!\n";
		WaitForSingleObject(process_info.hProcess, INFINITE);
		cout << "Process stoped!\n";
	}	
}
