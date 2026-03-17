#include <Timer.hpp>
#include <ctime>
#include <iomanip>
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
std::string Timer::get_Duration() const {
  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
      this->end_time - this->start_time);

  std::ostringstream result;

  /* if (duration.count() >= 1e9) { */
  auto hours = duration_cast<std::chrono::hours>(duration);
  duration -= hours;
  auto minutes = duration_cast<std::chrono::minutes>(duration);
  duration -= minutes;
  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
  duration -= seconds;
  auto ms = duration_cast<std::chrono::milliseconds>(duration);
  result << std::setfill('0') << std::setw(2) << hours.count() << "h, "
         << std::setw(2) << minutes.count() << "m, " << std::setw(2)
         << seconds.count() << "s, " << std::setw(3) << ms.count() << "ms";
  /* } */
  /* } else if (duration.count() >= 1e6) {
    result << duration_cast<std::chrono::milliseconds>(duration).count() <<
  "ms"; } else if (duration.count() >= 1e3) { result <<
  duration_cast<std::chrono::microseconds>(duration).count() << "us"; } else {
    result << duration.count() << "ns";
  } */

  return result.str();
}

//
void Timer::print(std::string func_name) const {
  // Timer in micro-seconds
  auto micro_s_duration = std::chrono::duration_cast<std::chrono::microseconds>(
      this->end_time - this->start_time);

  // Time in mili-seconds
  auto ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      this->end_time - this->start_time);

  // Time in seconds
  auto s_duration = std::chrono::duration_cast<std::chrono::seconds>(
      this->end_time - this->start_time);

  // Time in minutes
  auto min_duration = std::chrono::duration_cast<std::chrono::minutes>(
      this->end_time - this->start_time);

  // Printing out the results
  std::cout << func_name << " : " << min_duration.count() << "min "
            << s_duration.count() - (min_duration.count() * 60) << "s "
            << (ms_duration.count()) - (s_duration.count() * 1000) << "ms "
            << (micro_s_duration.count()) - (ms_duration.count() * 1000)
            << "us \n";
}