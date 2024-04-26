#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>

class Timer
{
//
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time;

//
public:

    Timer();
    ~Timer();

    void start();
    void stop();
    std::string get_Duration() const;
    void print(std::string func_name) const;
};

#endif