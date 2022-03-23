#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <chrono>

using std::cin;
using std::cout;
using std::thread;
using std::vector;

int main()
{
    int size = 0;
    cout << "Enter the dimentions of an array: ";
    cin >> size;
    if (size <= 0)
    {
        throw std::runtime_error("Error: Invalid paraments!\n");
        return -1;
    }

    vector<double> arr;
    int tmp = 0;
    for (int i = 0; i < size; i++)
    {
        cin >> tmp;
        arr.push_back(tmp);
    }

    int min = arr[0];
    int max = arr[0];
    thread min_max([&min, &max](vector<double> p_arr){
        // auto minimax = std::minmax_element(p_arr.begin(), p_arr.end());        
        // std::this_thread::sleep_for(
        //     std::chrono::milliseconds(7));
        // min = *minimax.first;
        // max = *minimax.second;

        for (int i = 1; i < p_arr.size(); i++)
        {
            if (min > p_arr[i])
            {
                min = p_arr[i];
            }
            std::this_thread::sleep_for(
                std::chrono::milliseconds(7));
            if (max < p_arr[i])
            {
                max = p_arr[i];
            }
            std::this_thread::sleep_for(
                std::chrono::milliseconds(7));
        }

        
    }, arr);

    double avg = 0;
    thread average([&avg](vector<double> p_arr){
        for (const double& item : p_arr)
        {
            avg += item;
            std::this_thread::sleep_for(
                std::chrono::milliseconds(12));
        }
        avg /= static_cast<double>(p_arr.size());
    }, arr); 
    min_max.join();
    average.join();
    cout << "The maximum value in the array:\t" << max << "\n";
    cout << "The minimum value in the array:\t" << min << "\n";
    cout << "The average value in the array:\t" << avg << "\n";

    for (double& item : arr)
    {
        if (item == min || item == max)
        {
            item = avg;
        }
        
        cout << item << "\t";
    }
    cout << std::endl;
    return 0;
}