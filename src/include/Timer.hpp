//
// Created by Mushahid.Raza on 10/13/2023
//

#ifndef SIGNALSENGINE_TIMER_H
#define SIGNALSENGINE_TIMER_H
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
/**
 * @class Timer
 * @brief A simple class for measuring the execution time of code blocks.
 *
 * The Timer class provides a convenient way to measure the execution time of
 * code blocks using high-resolution timing provided by the C++ <chrono>
 * library.
 */
using namespace std::literals;

class Timer 
{
    public:
    /**
     * @brief Constructs a Timer object and starts the timing.
     *
     * This constructor initializes the Timer object and records the starting time
     * using the high-resolution clock provided by the <chrono> library.
     */
    Timer() 
    { 
        start = std::chrono::steady_clock::now();
    }

    /**
     * @brief Destroys the Timer object and prints the elapsed time.
     *
     * The destructor calculates the elapsed time since the Timer object was
     * created, prints the elapsed time in milliseconds, and outputs it to the
     * standard output.
     */
    ~Timer() 
    {
        end = std::chrono::steady_clock::now();
        duration = end - start;
        float us = duration.count() * 1000.0f;
        //std::cout << "Time Taken: " << duration.count()*1000 << std::endl;
      

        
        //        std::cout << "Time took " << ms << "ms" << std::endl;
        printf("Time took %.4fms\n", us);
    }

private:
    std::chrono::time_point<std::chrono::steady_clock> start; /**< The start time of the Timer. */
    std::chrono::time_point<std::chrono::steady_clock> end; /**< The end time of the Timer. */
    std::chrono::duration<float> duration; /**< The duration between start and end times. */
};

#endif  // SIGNALSENGINE_TIMER_H