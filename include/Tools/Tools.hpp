#ifndef TOOLS_H_
#define TOOLS_H_

#include <iomanip>
#include <iostream>
#include <vector>

namespace tools {

// Set width for columns
const int columnWidth = 15;

// Print any header given as vector of string
void display_Header(std::vector<std::string> header) {
  for (size_t i = 0; i < header.size(); ++i) {
    std::cout << std::left << std::setw(columnWidth) << header[i] + ":";
  }
  std::cout << std::endl;
}

// Print any values
template <typename... Args> void display_Values(const Args &...values) {
  // Using variadic template to print values
  ((std::cout << std::left << std::setw(columnWidth) << values), ...);
  std::cout << std::endl;
}
} // namespace tools


#endif