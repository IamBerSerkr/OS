#include <iostream>
#include <vector>
#include <thread>
#include "threadInfo.h"

using std::cin;
using std::cout;
using std::vector;
using std::thread;

std::mutex markerMutex;
std::mutex startMutex;
std::mutex stateMutex;

std::condition_variable start;
std::condition_variable stateChanged;

bool isReady = false;
vector<bool> halteredThreads;


int generateRandomInt(const int&, const int&);

void printArray(const vector<int>&);

void markerFunction(threadInfo*);

bool isValidID(const int&);

int main(void)
{
	int sizeOfArray = generateRandomInt(5, 15);
	vector<int> arr(0, sizeOfArray);

	printArray(arr);

	int numberOfMarkers = generateRandomInt(3, 7);
	vector<thread> markers;
	vector<threadInfo*> markerInfo;

	vector<bool> terminatedThreads(false, numberOfMarkers);
	for (int i = 0; i < numberOfMarkers; i++)
	{
		markerInfo.push_back(new threadInfo(arr, sizeOfArray, i + 1));
		markers.push_back(thread(markerFunction, markerInfo.back()));
	}

	cout << "The threads are ready to start!\n";
	isReady = true;
	start.notify_all();

	int numberOfHaltredThreads = 0;
	// logic loop
	while (numberOfHaltredThreads != numberOfMarkers)
	{
		std::unique_lock<std::mutex> lock(stateMutex);
		stateChanged.wait(lock, []()-> bool {
			for (const bool& elem : halteredThreads)
			{
				if (!elem)
				{
					return false;
				}
			}
			return true;
		});
	
		printArray(arr);

		cout << "All threads are halted!\n";
		cout << "Enter the marker threads's ID to eliminate:\t";
		int userInputID = -1;
		cin >> userInputID;

		if (!isValidID(userInputID))
		{
			cout << "Invalid ID! Please enter valid id!\n";
			continue;
		}

		int arrayIndex = userInputID - 1;
		
		if (terminatedThreads[arrayIndex])
		{
			cout << "The thread has been terminated!\n";
			continue;
		}


		markerInfo[arrayIndex]->actions[1] = true;
		markerInfo[arrayIndex]->start.notify_one();
		terminatedThreads[arrayIndex] = true;
		markers[arrayIndex].join();

		numberOfHaltredThreads++;
		printArray(arr);

		for (int i = 0; i < numberOfMarkers; i++)
		{
			if (!terminatedThreads[i])
			{
				halteredThreads[i] = false;
				markerInfo[i]->actions[0] = true;
				markerInfo[i]->start.notify_one();
			}
		}
	}

	cout << "All marker threads have been eliminated!\n";
	return 0;
}