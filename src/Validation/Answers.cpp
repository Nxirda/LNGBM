#include "Answers.hpp"
#include "Tools.hpp"

/*
 */
Answers::Answers() {
  this->header = std::vector<std::string>();
  this->values = std::vector<std::vector<std::string>>();
  this->numeric_Values = std::vector<std::vector<float>>();
}

/*
 */
Answers::~Answers() {}

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
    const std::vector<std::vector<float>> &num_Vals) {
  this->numeric_Values = num_Vals;
}

/*
 */
std::vector<std::vector<float>> Answers::get_Numeric_Values() {
  return this->numeric_Values;
}

/*
 */
std::vector<std::string> Answers::get_Header() { return this->header; }

/*
 */
std::vector<std::vector<std::string>> Answers::get_Values() {
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
    Tools::display_Values_Vector<float>(this->numeric_Values[i]);
    std::cout << std::endl;
    //Tools::display_Values_Vector(this->header);
  }
}
