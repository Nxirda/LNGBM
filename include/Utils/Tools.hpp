#ifndef TOOLS_H_
#define TOOLS_H_

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace Tools {

// Set width for columns
const int column_Width = 21;

// Print any header given as vector of string
void display_Header(const std::vector<std::string> &header);

// Print any list of values
template <typename... Args> void display_Values_List(const Args &...values) {
  // Using variadic template to print values
  ((std::cout << std::left << std::setw(column_Width) << values), ...);
  std::cout << std::endl;
}

// Print a vector of elements
template <typename T> void display_Values_Vector(const std::vector<T> &values) {
  for (const auto &value : values) {
    std::cout << std::left << std::setw(column_Width) << value;
  }
}

// Print any value and a vector of elements
template <typename label, typename elements>
void display_Line(const label &firstElement,
                  const std::vector<elements> &restElements) {
  // Print the header of the line
  std::cout << std::left << std::setw(column_Width) << firstElement;
  // Print the vector (actual line)
  for (const auto &element : restElements)
    std::cout << std::left << std::setw(column_Width) << element << " ";
  std::cout << std::endl;
}

//
std::string format_Size(double size, const std::string &unit, int precision);

//
std::string matrix_Memory_Size(int length, int width, int element_Size,
                               int precision);

} // namespace Tools

#endif