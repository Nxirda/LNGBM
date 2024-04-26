#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include <string>
#include <vector>

#include "Bin.hpp"
#include "ICriteria.hpp"

class Histogram {
private:
  std::vector<Bin> histogram;
  size_t number_Of_Bins = 256; /**< An size_t variable to store the number of
                       Bin to generate. (default : 256 Bin) */

public:
  Histogram();

  Histogram(std::vector<Bin> &&new_Bins);

  Histogram(size_t size, const std::vector<double> &list);

  Histogram(size_t size, const std::vector<double> &list,
             const std::vector<size_t> &idx);

  //
  Histogram(Histogram &&histo);
  Histogram(const Histogram &histo);
  Histogram &operator=(Histogram &&histo);
  Histogram &operator=(const Histogram &histo);

  //
  void add_Point(double point_Value, double residual);

  ~Histogram();

  size_t get_Number_Of_Bins() const;
  const std::vector<Bin> &get_Bins() const;

  void print() const;
};

#endif