#include <algorithm>

#include "Histogram2.hpp"
#include "TrainingElement.hpp"

/**************************/
/*                        */
/*    HISTOGRAM CLASS     */
/*                        */
/**************************/

//
Histogram2::Histogram2() {}

Histogram2::Histogram2(Histogram2 &&histo) {
  this->number_Of_Bins = histo.number_Of_Bins;
  this->histogram = std::move(histo.histogram);
}

//
Histogram2::Histogram2(const Histogram2 &histo) {
  this->number_Of_Bins = histo.number_Of_Bins;
  this->histogram = histo.histogram;
}

//
Histogram2 &Histogram2::operator=(Histogram2 &&histo) {
  this->number_Of_Bins = histo.number_Of_Bins;
  this->histogram = std::move(histo.histogram);
  return *this;
}

Histogram2 &Histogram2::operator=(const Histogram2 &histo) {
  this->number_Of_Bins = histo.number_Of_Bins;
  this->histogram = histo.histogram;
  return *this;
}

//
Histogram2::Histogram2(size_t size, const std::vector<double> &list) {

  this->number_Of_Bins = size;

  auto min_max = std::minmax_element(list.begin(), list.end());
  double min = *min_max.first;
  double max = *min_max.second;

  size_t number_Of_Elems = list.size() / size;

  this->histogram.resize(size);

  const size_t bin_size = static_cast<size_t>((max - min) * (1.0 / size));

  double bin_Min = 0;
  double bin_Max = 0;

  for (size_t i = 0; i < size; ++i) {
    bin_Min = min + bin_size * i;
    bin_Max = bin_Min + bin_size;

    this->histogram[i] = std::move(Bin(number_Of_Elems, bin_Min, bin_Max));
  }
}

//
Histogram2::Histogram2(size_t size, const std::vector<double> &list,
                       const std::vector<size_t> &idx) {

  double min = std::numeric_limits<double>::max();
  double max = 0;
  this->number_Of_Bins = size;

  size_t number_Of_Elems = idx.size() / size;

  for (const auto &i : idx) {
    const auto &elem = list[i];
    if (elem < min)
      min = elem;
    else if (elem > max)
      max = elem;
  }

  this->histogram.resize(size);

  const size_t bin_size = static_cast<size_t>((max - min) * (1.0 / size));

  double bin_Min = 0;
  double bin_Max = 0;

  for (size_t i = 0; i < size; ++i) {

    bin_Min = min + bin_size * i;
    bin_Max = bin_Min + bin_size;

    this->histogram[i] = std::move(Bin(number_Of_Elems, bin_Min, bin_Max));
  }
}

//
Histogram2::~Histogram2() {}

//
size_t Histogram2::get_Number_Of_Bins() const { return this->number_Of_Bins; }

//
std::vector<Bin> Histogram2::get_Histogram() const { return this->histogram; }