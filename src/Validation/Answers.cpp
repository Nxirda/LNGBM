#include "Answers.hpp"
#include "Tools.hpp"

/*
 */
Answers::Answers() {
  this->header = std::vector<std::string>();
  this->values = std::vector<std::vector<std::string>>();
  this->numeric_Values = std::vector<std::vector<double>>();
}

/*
 */
Answers::Answers(const Answers &ans) {
  this->header = ans.header;
  this->values = ans.values;
  this->numeric_Values = ans.numeric_Values;
}

/*
 */
Answers::~Answers() {}

/*
 */
Answers &Answers::operator=(const Answers &ans) {
  this->header = ans.header;
  this->values = ans.values;
  this->numeric_Values = ans.numeric_Values;
  return *this;
}

/*
 */
void Answers::add_And_Mean_Values(const Answers &ans) {
  assert(this->numeric_Values.size() == ans.get_Numeric_Values().size());
  
  for (size_t i = 0; i < this->numeric_Values.size(); ++i) {
    for (size_t j = 0; j < this->numeric_Values[i].size(); ++j) {
      this->numeric_Values[i][j] += ans.get_Numeric_Values()[i][j];
      this->numeric_Values[i][j] /= 2.0;
    }
  }
}

/*
 */
void Answers::set_Header(const std::vector<std::string> &headers) {
  this->header = headers;
}

/*
 */
void Answers::set_Values(const std::vector<std::vector<std::string>> &vals) {
  this->values = vals;
}

/*
 */
void Answers::set_Numeric_Values(
    const std::vector<std::vector<double>> &num_Vals) {
  this->numeric_Values = num_Vals;
}

/*
 */
std::vector<std::vector<double>> Answers::get_Numeric_Values() const {
  return this->numeric_Values;
}

/*
 */
std::vector<std::string> Answers::get_Header() const { return this->header; }

/*
 */
std::vector<std::vector<std::string>> Answers::get_Values() const {
  return this->values;
}

/*
 */
void Answers::print() {
  assert(this->values.size() == this->numeric_Values.size());
  Tools::display_Header(this->header);

  for (size_t i = 0; i < this->values.size(); ++i) {
    // Prints the string values
    Tools::display_Values_Vector<std::string>(this->values[i]);
    // Then prints the numerical values
    Tools::display_Values_Vector<double>(this->numeric_Values[i]);
    std::cout << std::endl;
  }
}
