
#include <iostream>
#include <string>
#include <vector>

#ifndef DATASET_H_
#define DATASET_H_

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

  void load(std::string file_Path);

  // Destructor

  ~DataSet();

  // Getters

  std::vector<std::string> get_Features() const;
  std::vector<std::vector<float>> get_Samples() const;
  std::vector<float> get_Labels(const std::vector<int> &idx) const;
  std::vector<float> get_Column(int position,
                                const std::vector<int> &idx) const;

  // Methods

  void print() const;
  bool empty() const;
  void print_With_Index(std::vector<int> idx) const;
  void initialize_Labels(std::vector<float> column);
  void update_Label_Value(int position, float value);

  int labels_Number() const;
  int samples_Number() const;
  int features_Length() const;

  float labels_Mean(const std::vector<int> &idx) const;
  float column_Variance(const std::vector<int> &idx) const;
  float column_Mean(int position, const std::vector<int> &idx) const;

  std::vector<std::vector<int>> split(int position, float criteria,
                                      const std::vector<int> &idx) const;
};

#endif