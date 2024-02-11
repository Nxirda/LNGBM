#include "Tools.hpp"

namespace Tools {

// Print any header given as vector of string
void display_Header(const std::vector<std::string> &header) {
  for (size_t i = 0; i < header.size(); ++i) {
    std::cout << std::left << std::setw(column_Width) << header[i] + ":";
  }
  std::cout << std::endl;
}

//
std::string format_Size(double size, const std::string &unit, int precision) {
  std::ostringstream oss;
  // Truncate the size of floats to 10 digits
  oss << std::fixed << std::setprecision(precision) << size << " " << unit;
  return oss.str();
}

//
std::string matrix_Memory_Size(int length, int width, int element_Size,
                               int precision) {
  // Calculate the size in bytes
  int matrixSizeInBytes = length * width * element_Size;

  // Convert bytes
  const int kibibyte = 1024;
  const int mebibyte = 1024 * kibibyte;
  const int gibibyte = 1024 * mebibyte;

  double size;
  std::string unit;

  if (matrixSizeInBytes >= gibibyte) {
    size = static_cast<double>(matrixSizeInBytes) / gibibyte;
    unit = "GiB";
  } else if (matrixSizeInBytes >= mebibyte) {
    size = static_cast<double>(matrixSizeInBytes) / mebibyte;
    unit = "MiB";
  } else if (matrixSizeInBytes >= kibibyte) {
    size = static_cast<double>(matrixSizeInBytes) / kibibyte;
    unit = "KiB";
  } else {
    size = static_cast<double>(matrixSizeInBytes);
    unit = "Bytes";
  }

  return format_Size(size, unit, precision);
}

} // namespace Tools