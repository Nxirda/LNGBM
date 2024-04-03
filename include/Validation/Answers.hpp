#ifndef ANSWERS_H_
#define ANSWERS_H_

#include <string>
#include <vector>

#include "Tools.hpp"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

class Answers {
private:
  std::vector<std::vector<double>> numeric_Values;
  std::vector<std::vector<std::string>> values;
  std::vector<std::string> header;

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    // Unused param but necessary for boost
    static_cast<void>(version);
    ar &header;
    ar &values;
    ar &numeric_Values;
  }

public:
  Answers();
  Answers(const Answers &ans);

  ~Answers();

  Answers &operator=(const Answers &ans);
  void add_And_Mean_Values(const Answers &ans);

  void set_Values(const std::vector<std::vector<std::string>> &vals);
  std::vector<std::vector<std::string>> get_Values() const;

  void set_Numeric_Values(const std::vector<std::vector<double>> &num_Vals);
  std::vector<std::vector<double>> get_Numeric_Values() const;

  void set_Header(const std::vector<std::string> &headers);
  std::vector<std::string> get_Header() const;

  void print();
};

#endif