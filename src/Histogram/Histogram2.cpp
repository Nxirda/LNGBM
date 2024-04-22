#include <algorithm>

#include "Histogram2.hpp"
#include "TrainingElement.hpp"

/**************************/
/*                        */
/*    HISTOGRAM CLASS     */
/*                        */
/**************************/

//
Histogram2::Histogram2() { this->number_Of_Bins = 0; }

//
Histogram2::Histogram2(std::vector<Bin> &&new_Bins) {
  this->number_Of_Bins = new_Bins.size();
  this->histogram = std::move(new_Bins);
}

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

//
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

  this->histogram.resize(size);

  const double bin_size = ((max - min) * (1.0 / size));

  double bin_Min = 0;
  double bin_Max = 0;

  for (size_t i = 0; i < size; ++i) {
    bin_Min = min + bin_size * i;
    bin_Max = bin_Min + bin_size;

    this->histogram[i] = std::move(Bin(bin_Min, bin_Max));
  }
}

//
Histogram2::Histogram2(size_t size, const std::vector<double> &list,
                       const std::vector<size_t> &idx) {

  double min = std::numeric_limits<double>::max();
  double max = 0;
  this->number_Of_Bins = size;

  for (const auto &i : idx) {
    const auto &elem = list[i];
    if (elem < min)
      min = elem;
    else if (elem > max)
      max = elem;
  }

  this->histogram.resize(size);

  const double bin_size = ((max - min) * (1.0 / size));

  double bin_Min = 0;
  double bin_Max = 0;

  for (size_t i = 0; i < size; ++i) {

    bin_Min = min + bin_size * i;
    bin_Max = bin_Min + bin_size;

    this->histogram[i] = std::move(Bin(bin_Min, bin_Max));
  }
}

// Binary search because it's fast 
void Histogram2::add_Point(double point_Value, double residual) {
  size_t left = 0;
  size_t right = this->histogram.size();

  while (left < right) {
    size_t mid = left + (right - left) / 2;
    auto &bin = this->histogram[mid];

    if (point_Value < bin.get_Min()) {
      right = mid;
    } else if (point_Value >= bin.get_Max()) {
      left = mid + 1;
    } else {
      bin.add_Element(residual);
      return;
    }
  }
}

//
Histogram2::~Histogram2() {}

//
size_t Histogram2::get_Number_Of_Bins() const { return this->number_Of_Bins; }

//
const std::vector<Bin> &Histogram2::get_Bins() const { return this->histogram; }

//
void Histogram2::print() const {
  for (size_t i = 0; i < this->number_Of_Bins; ++i) {
    std::cout << "Bin n° " << i << " "
              << " min " << this->histogram[i].get_Min() << " max "
              << this->histogram[i].get_Max() << " ";
    this->histogram[i].print();
  }
}