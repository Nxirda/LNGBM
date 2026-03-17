#include <algorithm>

#include "Histogram.hpp"
#include "TrainingElement.hpp"

/**************************/
/*                        */
/*    HISTOGRAM CLASS     */
/*                        */
/**************************/

//
Histogram::Histogram() { this->number_Of_Bins = 0; }

//
Histogram::Histogram(std::vector<Bin> &&new_Bins) {
  this->number_Of_Bins = new_Bins.size();
  this->histogram = std::move(new_Bins);
}

//
Histogram::Histogram(Histogram &&histo) {
  this->number_Of_Bins = histo.number_Of_Bins;
  this->histogram = std::move(histo.histogram);
}

//
Histogram::Histogram(const Histogram &histo) {
  this->number_Of_Bins = histo.number_Of_Bins;
  this->histogram = histo.histogram;
}

//
Histogram &Histogram::operator=(Histogram &&histo) {
  this->number_Of_Bins = histo.number_Of_Bins;
  this->histogram = std::move(histo.histogram);
  return *this;
}

//
Histogram &Histogram::operator=(const Histogram &histo) {
  this->number_Of_Bins = histo.number_Of_Bins;
  this->histogram = histo.histogram;
  return *this;
}

//
Histogram::Histogram(size_t size, const std::vector<double> &list) {

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
Histogram::Histogram(size_t size, const std::vector<double> &list,
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
void Histogram::add_Point(double point_Value, double residual) {
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
Histogram::~Histogram() {}

//
size_t Histogram::get_Number_Of_Bins() const { return this->number_Of_Bins; }

//
const std::vector<Bin> &Histogram::get_Bins() const { return this->histogram; }

//
void Histogram::print() const {
  for (size_t i = 0; i < this->number_Of_Bins; ++i) {
    std::cout << "Bin n° " << i << " "
              << " min " << this->histogram[i].get_Min() << " max "
              << this->histogram[i].get_Max() << " ";
    this->histogram[i].print();
  }
}