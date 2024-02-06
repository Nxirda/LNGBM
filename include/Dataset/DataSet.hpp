#ifndef DATASET_H_
#define DATASET_H_

#include <iostream>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

/** @class DataSet
 * @brief Class representing the dataset for the program
 *
 */
class DataSet {
protected:
  std::vector<float> labels; /**< A vector containing the labels as floats */
  std::vector<std::string>
      features; /**< A vector containing the features as floats */
  std::vector<std::vector<float>>
      samples; /**< A 2D Matrix containing the sample values as floats */

public:
  /**
   * @brief Default constructor for the DataSet class.
   *
   * Initializes the class with empty parameters.
   */
  DataSet();

  /**
   * @brief Parametrize constructor for the DataSet class.
   *
   * @param string file_Path: the path of the file containing the dataset (csv)
   *
   * Initializes the class with filled parameters.
   */
  DataSet(std::string file_Path);

  /**
   * @brief Copy constructor for the DataSet class (not optimal might have to be
   * changed)
   *
   * @param vector<string> features : the features of the dataset to copy
   * @param vector<vector<float> values :  2D Matrix of the dataset elements
   * @param vector<float> labels : vector containing the "truth" values of the
   * dataset
   *
   * Initializes the class with filled parameters.
   */
  DataSet(std::vector<std::string> features,
          std::vector<std::vector<float>> values, std::vector<float> labels);

  /**
   * @brief Copy constructor for the DataSet class, needed for splitting the
   * dataset into subsets (exp: train/test)
   *
   * @param DataSet data : the dataset to copy
   * @param vector<int> idx : the indexes we want to copy
   *
   * Initializes the class with filled parameters.
   */
  DataSet(const DataSet &data, std::vector<int> idx);

  /**
   * @brief static method to load a new dataset given by the path
   *
   * @param string file_Path : the path where the dataset is stored
   *
   * Initializes a new DataSet
   */
  static DataSet load(std::string file_Path);

  /**
   * @brief Destructor for the DataSet class.
   */
  ~DataSet();

  /**
   * @brief Method to make a copy of the features vector
   *
   * @return The features of the current dataset as a vector of strings
   */
  std::vector<std::string> get_Features() const;

  /**
   * @brief Method to make a copy of the samples 2D Matrix
   *
   * @return The samples of the current dataset as a 2D vector of floats
   */
  std::vector<std::vector<float>> get_Samples() const;

  /**
   * @brief Method to copy specified samples of the 2D Matrix (DataSet)
   *
   * @return The specified samples of the current dataset as a 2D vector of
   * floats
   */
  std::vector<std::vector<float>>
  get_Samples(const std::vector<int> &idx) const;

  /**
   * @brief Method to make a copy of the labels of the DataSet
   *
   * @return The  labels of the current dataset as a vector of floats
   */
  std::vector<float> get_Labels() const;

  /**
   * @brief Method to copy specified labels of the DataSet
   *
   * @return The specified labels of the current dataset as a vector of floats
   */
  std::vector<float> get_Labels(const std::vector<int> &idx) const;

  /**
   * @brief Method to copy a column of the DataSet (samples value on one feature)
   *
   * @param int position : the feature to copy
   * @param vector<idx> : the samples values to copy in the column
   * 
   * @return The values contained at the feature position for the samples in idx
   */
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