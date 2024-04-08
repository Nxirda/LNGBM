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
  this->size = histo.size;
  this->histogram = std::move(histo.histogram);
}

//
Histogram2::Histogram2(const Histogram2 &histo) {
  this->size = histo.size;
  this->histogram = histo.histogram; // get_Histogram();
}

//
Histogram2 &Histogram2::operator=(Histogram2 &&histo) {
  this->size = histo.size;
  this->histogram = std::move(histo.histogram);
  return *this;
}

Histogram2 &Histogram2::operator=(const Histogram2 &histo) {
  this->size = histo.size;
  this->histogram = histo.histogram; // histo.get_Histogram();
  return *this;
}

//
Histogram2::Histogram2(size_t size, const std::vector<double> &list,
                       const ICriteria *criteria) {

  this->size = size;

  auto min_max = std::minmax_element(list.begin(), list.end());
  double min = *min_max.first;
  double max = *min_max.second;

  double numb_Of_Elems = 0;

  // NEED TO GET THE NUMBER OF ELEMENTS INSIDE THE BIN
  // NORMALLY ONLY FOR THE FIRST ONE

  // std::vector<Bin> new_Histo(size);
  this->histogram.resize(size);

  const size_t bin_size = (max - min) * (1.0 / size);

  for (size_t i = 0; i < size; ++i) {
    // Returns the values that separates two Bin
    Bin new_Bin(numb_Of_Elems, min, max);
    this->histogram[i] = std::move(new_Bin); // min + bin_size * i;
  }
}

//
Histogram2::Histogram2(size_t size, const std::vector<double> &list,
                       const ICriteria *criteria,
                       const std::vector<size_t> &idx) {

  double min = std::numeric_limits<double>::max();
  double max = 0;
  this->size = size;

  double numb_Of_Elems = 0;

  // NEED TO GET THE NUMBER OF ELEMENTS INSIDE THE BIN
  // NORMALLY ONLY FOR THE FIRST ONE

  for (const auto &i : idx) {
    const auto &elem = list[i];
    if (elem < min)
      min = elem;
    else if (elem > max)
      max = elem;
  }

  // std::vector<double> Bin(size, 0.0);
  this->histogram.resize(size);

  const size_t bin_size = (max - min) * (1.0 / size);

  for (size_t i = 0; i < size; ++i) {

    Bin new_Bin(numb_Of_Elems, min, max);
    this->histogram[i] = std::move(new_Bin); // min + bin_size * i;
    // Returns the values that separates two Bin
    // Bin[i] = min + bin_size * i;
  }

  //this->histogram = std::move(Bin);
}

//
Histogram2::~Histogram2() {}

//
size_t Histogram2::get_Number_Of_Bin() const { return this->size; }

//
std::vector<double> Histogram2::get_Histogram() const {
  // return this->histogram;
  std::cout << "JNEZCJZAECIJZABENCLZA IN GET HISTORGAM\n";
  return std::vector<double>();
}