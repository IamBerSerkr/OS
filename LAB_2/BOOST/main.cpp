#include <iostream>
#include <vector>
#include <exception>
#include <iomanip>
#include <boost/thread.hpp>
#include <boost/random.hpp>
#include <boost/chrono.hpp>

using std::cin;
using std::cout;
using std::vector;
using boost::thread;

typedef boost::mt19937 RNGType;

const int kMimMaxThreadSleepTime = 7;
const int kAvgThreadSleeptime = 12;

vector<double> generateRandomArray(void);

int main()
{   
    vector<double> arr = generateRandomArray();
    
    for (const double& item : arr)
    {
        std::setprecision(0);
        cout << item << "\t";
    }
    cout << std::endl;

    double min = arr[0];
    double max = arr[0];
    thread min_max([&min, &max](vector<double> p_arr) {
            for (int i = 1; i < p_arr.size(); i++)
            {
                if (min > p_arr[i])
                {
                    min = p_arr[i];
                }
                boost::this_thread::sleep_for(
                    boost::chrono::microseconds(kMimMaxThreadSleepTime)
                );
                if (max < p_arr[i])
                {
                    max = p_arr[i];
                }
                boost::this_thread::sleep_for(
                    boost::chrono::microseconds(kMimMaxThreadSleepTime)
                );
            }
        }, arr);

    double avg = 0;
    thread average([&avg](vector<double> p_arr) {
        for (const double& item : p_arr)
        {
            avg += item;
            boost::this_thread::sleep_for(
                boost::chrono::microseconds(kAvgThreadSleeptime)
            );
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

vector<double> generateRandomArray(void)
{
    vector<double> arr;

    // set up random generator
    RNGType rng;
    boost::uniform_int<> size(5, 15);
    boost::variate_generator<RNGType, boost::uniform_int<>> dice(rng, size);

    int arraySize = dice();
    boost::uniform_int<> element(-1000, 1000);
    dice.distribution() = element;
    int tmp = 0;
    for (int i = 0; i < arraySize; i++)
    {
        tmp = dice();
        arr.push_back(tmp);
    }
    return arr;
}