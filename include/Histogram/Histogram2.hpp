#ifndef HISTOGRAM2_H_
#define HISTOGRAM2_H_

#include <string>
#include <vector>

#include "Bin.hpp"
#include "ICriteria.hpp"

class Histogram2 {
private:
  size_t size = 32; /**< An size_t variable to store the number of Bin to
                       generate. (default : 32 Bin) */

  std::vector<Bin> histogram;

public:
  Histogram2();

  Histogram2(size_t size, const std::vector<double> &list,
             const ICriteria *criteria);

  Histogram2(size_t size, const std::vector<double> &list,
             const ICriteria *criteria, const std::vector<size_t> &idx);

  //
  Histogram2(Histogram2 &&histo);
  Histogram2(const Histogram2 &histo);
  Histogram2 &operator=(Histogram2 &&histo);
  Histogram2 &operator=(const Histogram2 &histo);

  ~Histogram2();

  void print() const;

  size_t get_Number_Of_Bin() const;
  std::vector<double> get_Histogram() const;
};

#endif