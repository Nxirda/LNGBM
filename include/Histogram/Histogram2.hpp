#ifndef HISTOGRAM2_H_
#define HISTOGRAM2_H_

#include <string>
#include <vector>

#include "Bin.hpp"
#include "ICriteria.hpp"

class Histogram2 {
private:
  std::vector<Bin> histogram;
  size_t number_Of_Bins = 256; /**< An size_t variable to store the number of
                       Bin to generate. (default : 256 Bin) */

public:
  Histogram2();

  Histogram2(size_t size, const std::vector<double> &list);

  Histogram2(size_t size, const std::vector<double> &list,
             const std::vector<size_t> &idx);

  //
  Histogram2(Histogram2 &&histo);
  Histogram2(const Histogram2 &histo);
  Histogram2 &operator=(Histogram2 &&histo);
  Histogram2 &operator=(const Histogram2 &histo);

  //
  void add_Point(double point_Value, double statistic);

  ~Histogram2();

  size_t get_Number_Of_Bins() const;
  std::vector<Bin> get_Bins() const;
};

#endif