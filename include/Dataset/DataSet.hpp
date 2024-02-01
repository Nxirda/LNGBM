#ifndef DATASET_H_
#define DATASET_H_

#include <iostream>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

class DataSet {
protected:
  // Parameters

  std::vector<float> labels;
  std::vector<std::string> features;
  std::vector<std::vector<float>> samples;

public:
  // Constructor

  DataSet();
  DataSet(std::string file_Path);
  DataSet(std::vector<std::string> features,
          std::vector<std::vector<float>> values, std::vector<float> labels);
  DataSet(const DataSet &data, std::vector<int> idx);

  static DataSet load(std::string file_Path);

  // Destructor

  ~DataSet();

  // Getters

  std::vector<std::string> get_Features() const;

  std::vector<std::vector<float>> get_Samples() const;
  std::vector<std::vector<float>>
  get_Samples(const std::vector<int> &idx) const;

  std::vector<float> get_Labels() const;
  std::vector<float> get_Labels(const std::vector<int> &idx) const;

  std::vector<float> get_Column(int position,
                                const std::vector<int> &idx) const;

  // Methods

  void print() const;
  bool empty() const;

  void print_With_Index(std::vector<int> idx) const;

  int labels_Number() const;
  int samples_Number() const;
  int features_Length() const;
  int element_Size() const;

  float whole_Labels_Mean() const;
  float labels_Mean(const std::vector<int> &idx) const;
  float labels_Variance(const std::vector<int> &idx) const;
  float column_Mean(int position, const std::vector<int> &idx) const;
  std::vector<float>
  column_Percentiles(int position, const std::vector<int> &idx,
                     const std::vector<float> &percentiles) const;

  std::tuple<std::optional<std::vector<int>>, std::optional<std::vector<int>>>
  split(int position, float criterion, const std::vector<int> &idx) const;
};

#endif