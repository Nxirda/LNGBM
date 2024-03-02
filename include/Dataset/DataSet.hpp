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
  std::vector<double>
      labels; /**< A vector containing the labels as doubles (f64)*/
  std::vector<std::string>
      features; /**< A vector containing the features as doubles (f64)*/
  std::vector<std::vector<double>>
      samples; /**< A 2D Matrix containing the sample values as doubles (f64)*/

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
   * @param vector<vector<double> values :  2D Matrix of the dataset elements
   * @param vector<double> labels : vector containing the "truth" values of the
   * dataset
   *
   * Initializes the class with filled parameters.
   */
  DataSet(const std::vector<std::string> &features,
          const std::vector<std::vector<double>> &values,
          const std::vector<double> &labels);

  /**
   * @brief Copy constructor for the DataSet class, needed for splitting the
   * dataset into subsets (exp: train/test)
   *
   * @param DataSet data : the dataset to copy
   * @param vector<size_t> idx : the indexes we want to copy
   *
   * Initializes the class with filled parameters.
   */
  DataSet(const DataSet &data, const std::vector<size_t> &idx);

  /**
   * @brief Destructor for the DataSet class.
   */
  ~DataSet();

  /**
   * @brief Copy of the features vector
   *
   * @return Type : vector::<string>, The features of the current dataset as a
   * vector of strings
   */
  const std::vector<std::string> &get_Features() const;

  /**
   * @brief Copy of the samples 2D Matrix
   *
   * @return Type : vector<vector<double>>, The samples of the current dataset
   * as a 2D vector of floats
   */
  const std::vector<std::vector<double>> &get_Samples() const;

  /**
   * @brief Copy specified samples of the 2D Matrix (DataSet)
   *
   * @param const vector<size_t> &index: index of the samples to get
   *
   * @return Type : vector<vector<double>>, The specified samples of the current
   * dataset as a 2D vector of floats
   */
  std::vector<std::vector<double>>
  get_Samples(const std::vector<size_t> &idx) const;

  /**
   * @brief Returns a reference to the specified labels of the DataSet
   *
   * @return Type : vector<double>, The specified labels of the current dataset
   */
  const std::vector<double> &get_Labels() const;

  /**
   * @brief Returns a copy of the specified labels of the
   * DataSet
   *
   * @param const vector<size_t> &, index of the labels to to copy
   *
   * @return Type : vector<double>, The  labels of the current dataset
   */
  std::vector<double> get_Labels(const std::vector<size_t> &idx) const;

  /**
   * @brief Returns a reference to a column of the DataSet (samples value on one
   * feature)
   *
   * @param size_t position : the column to copy
   *
   * @return The values contained for the feature at position
   */
  const std::vector<double> &get_Column(size_t position) const;

  /**
   * @brief Prints the whole dataset as a matrix
   */
  void print() const;

  /**
   * @brief Prints the dataset for the specified samples as a matrix
   *
   * @param vector<size_t>, index of the samples to copy
   */
  void print_With_Index(const std::vector<size_t> &idx) const;

  /**
   * @brief Tests whether or not the dataset is empty
   *
   * @return bool, true if the dataset is empty false if it's not
   */
  bool empty() const;

  /**
   * @brief Returns the number of labels the dataset has
   *
   * @return size_t, the number of labels
   */
  size_t labels_Number() const;

  /**
   * @brief Returns the number of samples the dataset has
   *
   * @return size_t, the number of samples
   */
  size_t samples_Number() const;

  /**
   * @brief Returns the number of features the dataset has
   *
   * @return size_t, the number of features
   */
  size_t features_Number() const;

  /**
   * @brief Returns the size of the type that represents the dataset's values
   * (ex : int, float)
   *
   * @return int, the size of the type
   */
  int element_Size() const;

  /**
   * @brief Returns the mean of the label of the dataset
   *
   * @return double, the mean
   */
  double whole_Labels_Mean() const;

  /**
   * @brief Returns the mean of the specified labels of the dataset
   *
   * @param const vector<size_t> &: index of the labels
   *
   * @return double, the mean of the specified values
   */
  double labels_Mean(const std::vector<size_t> &idx) const;

  /**
   * @brief Returns the variance of the specified labels of the dataset
   *
   * @param const vector<size_t> &: index of the labels
   *
   * @return double, the variance of the specified values
   */
  double labels_Variance(const std::vector<size_t> &idx) const;

  /**
   * @brief Returns the mean of a column of the dataset on the specified samples
   *
   * A column contains the values for a feature
   *
   * @param size_t : position of the column (feature)
   * @param const vector<size_t> & : the index of the samples
   *
   * @return double, the mean of the specified values
   */
  double column_Mean(size_t position, const std::vector<size_t> &idx) const;

  /**
   * @brief Splits the dataset in two parts depending on the criterion and the
   * position
   *
   * @param size_t : position of the column (feature) on which to split
   * @param double : value on which to split the column
   * @param const vector<size_t> &, index of the lines (samples) to consider
   *
   * @return tuple<optional<vector<size_t>>,optional<vector<size_t>>>, two
   * vectors containing the index of the samples each new dataset has
   */
  std::tuple<std::optional<std::vector<size_t>>,
             std::optional<std::vector<size_t>>>
  split(size_t position, double criterion,
        const std::vector<size_t> &idx) const;
};

#endif