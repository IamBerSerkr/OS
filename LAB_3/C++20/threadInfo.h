#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>


struct threadInfo
{
	std::vector<int> arr;
	int size;
	int idx;
	bool actions[2];
	std::condition_variable start;
	std::mutex actionMutex;

	threadInfo(void);
	threadInfo(const std::vector<int>&, const int&, const int&);
};

threadInfo::threadInfo(void)
{
	this->size = 0;
	this->idx = -1;
	this->actions[0] = false;
	this->actions[1] = false;
}

threadInfo::threadInfo(const std::vector<int>& arr,
	const int& size, const int& idx)
	: arr(arr), size(size), idx(idx)
{
	this->actions[0] = false;
	this->actions[1] = false;
}