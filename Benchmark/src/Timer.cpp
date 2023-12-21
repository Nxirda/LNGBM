#include <Timer.hpp>
#include <ctime>
#include <iostream>

//
Timer::Timer() {}

//
Timer::~Timer() {}

//
void Timer::start() {
  this->start_time = std::chrono::high_resolution_clock::now();
}

//
void Timer::stop() {
  this->end_time = std::chrono::high_resolution_clock::now();
}

//
void Timer::print(std::string func_name) {
  auto micro_s_duration = std::chrono::duration_cast<std::chrono::microseconds>(
      this->end_time - this->start_time);
  auto ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      this->end_time - this->start_time);
  auto s_duration = std::chrono::duration_cast<std::chrono::seconds>(
      this->end_time - this->start_time);
  auto min_duration = std::chrono::duration_cast<std::chrono::minutes>(
      this->end_time - this->start_time);
  std::cout << func_name << " : " << min_duration.count() << "min "
            << s_duration.count() - (min_duration.count() * 60) << "s "
            << (ms_duration.count()) - (s_duration.count() * 1000) << "ms "
            << (micro_s_duration.count()) - (ms_duration.count() * 1000)
            << "us \n";
}