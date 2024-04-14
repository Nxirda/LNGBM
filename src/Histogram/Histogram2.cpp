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

  // size_t number_Of_Elems = list.size() / size;

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

  // Just verify : (size == gradient_Sum.size)
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

//
void Histogram2::add_Point(double point_Value, double residual, double target) {
  for (size_t bin = 0; bin < this->histogram.size(); ++bin) {
    if (this->histogram[bin].get_Min() <= point_Value &&
        point_Value < this->histogram[bin].get_Max()) {
      this->histogram[bin].add_Element(residual, target);
      return;
    }
  }
}

//
void Histogram2::clean_Empty_Bins() {
  for (auto it = this->histogram.begin(); it != this->histogram.end();) {
    if (it->get_Count() == 0) {
      it = this->histogram.erase(it);
      number_Of_Bins -= 1;
    } else {
      ++it;
    }
  }
  this->histogram.shrink_to_fit();
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