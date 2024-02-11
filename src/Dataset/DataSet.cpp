#include <algorithm>
#include <cmath>     //for std lerp in percentiles function
#include <execution> // for parallel execution policies
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>   // std::stringstream
#include <stdexcept> // std::runtime_error
#include <vector>

#include <cblas.h>
#include <omp.h>

#include "DataSet.hpp"

/********************/
/*                  */
/*  DATASET CLASS   */
/*                  */
/********************/

// https://www.gormanalysis.com/blog/reading-and-writing-csv-files-with-cpp/

/*
Take the path of the file to read
Instanciates an object of type DataSet from the given CSV
Parameters : file path
Inputs     : String
Ouputs     : Object of DataSet Class
*/
DataSet::DataSet(std::string file_Path) {
  // input filestream
  std::ifstream file;
  file.open(file_Path);

  // Make sure the file is open
  if (!file.is_open()) {
    throw std::runtime_error("Could not open given file");
  }

  std::string line;
  std::string colname;

  // If no error state flag
  if (file.good()) {
    // Extract the first row in the file
    getline(file, line);

    // Create a stringstream from line
    std::stringstream ss(line);

    // Extract each column name
    while (getline(ss, colname, ',')) {
      // Skip last column because it's the target name and it's not really
      // usefull
      if (!ss.eof()) {
        // Initialize DataSet Labels
        this->features.push_back(colname);
      }
    }
  }

  float val;
  // Read the other rows
  while (getline(file, line)) {
    // string stream of current line
    std::stringstream ss(line);

    // tmp vector : represent a row in the matrix
    std::vector<float> tmp;
    while (ss >> val) {
      // Last value of CSV are labels
      if (ss.eof()) {
        this->labels.push_back(val);
      } else {
        tmp.push_back(val);
      }

      if (ss.peek() == ',') {
        ss.ignore();
      }
    }
    // Place the row in the 2D Matrix
    this->samples.push_back(tmp);
  }

  // Close file
  file.close();
}

/*
Take the path of the file to read
Instanciates an object of type DataSet from the given CSV
Parameters : file path
Inputs     : String
Ouputs     : Object of DataSet Class
*/
DataSet DataSet::load(std::string file_Path) {
  // input filestream
  std::ifstream file;
  file.open(file_Path);
  DataSet ds{};

  // Make sure the file is open
  if (!file.is_open()) {
    throw std::runtime_error("Could not open given file");
  }

  std::string line;
  std::string colname;

  // If no error state flag
  if (file.good()) {
    // Extract the first row in the file
    getline(file, line);

    // Create a stringstream from line
    std::stringstream ss(line);

    // Extract each column name
    while (getline(ss, colname, ',')) {
      // Skip last column because it's the target name and it's not really
      // usefull
      if (!ss.eof()) {
        // Initialize DataSet features
        ds.features.push_back(colname);
      }
    }
  }

  float val;
  // Read the other rows
  while (getline(file, line)) {
    // string stream of current line
    std::stringstream ss(line);

    // tmp vector : represent a row in the matrix
    std::vector<float> tmp;
    while (ss >> val) {
      // Ignore label column
      if (ss.eof()) {
        ds.labels.push_back(val);
      } else {
        tmp.push_back(val);
      }
      if (ss.peek() == ',') {
        ss.ignore();
      }
    }
    // Place the row in the 2D Matrix
    ds.samples.push_back(tmp);
  }

  // Close file
  file.close();
  return ds;
}

/*
Explicit Constructor, takes three vectors and builds a DataSet Object
Parameters : features, samples, labels
Inputs     : vector<string>, vector<vector<float>>, vector<float>
Ouputs     : Object of DataSet Class
*/
DataSet::DataSet(std::vector<std::string> features,
                 std::vector<std::vector<float>> samples,
                 std::vector<float> labels) {
  this->features = features;
  this->samples = samples;
  this->labels = labels;
}

/*
Constructor to copy partially a DataSet at the given indexes
Parameters : Dataset, index
Inputs     : const DataSet,  vector<int>
Outputs    : Object of DataSet Class
*/
DataSet::DataSet(const DataSet &data, std::vector<int> idx) {
  this->features = data.get_Features();
  this->samples = data.get_Samples(idx);
  this->labels = data.get_Labels(idx);
}

/*
Default Constructor
Parameters :
Inputs     :
Ouputs     : Object of DataSet Class
*/
DataSet::DataSet() {
  this->labels = std::vector<float>();
  this->features = std::vector<std::string>();
  this->samples = std::vector<std::vector<float>>();
}

/*
Default Destructor
Parameters :
Inputs     :
Ouputs     :
*/
DataSet::~DataSet() {}

/*
Naive print function of the DataSet
Parameters :
Inputs     :
Ouputs     :
*/
void DataSet::print() const {
  // Logical but prints the features
  for (long unsigned int i = 0; i < this->features.size(); ++i) {
    std::cout << features[i] << "\t";
  }
  std::cout << "\n";
  // Logical but Prints the samples
  for (int i = 0; i < int(this->samples.size()); ++i) {
    for (long unsigned int j = 0; j < this->samples[0].size(); ++j) {
      std::cout << samples[i][j] << "|\t";
    }
    std::cout << labels[i] << "|\t";
    std::cout << "\n";
  }

  std::cout << "\n";
}

/*
Print function of the DataSet with Index
Used by TreeNode Class
Parameters : index
Inputs     : vector<int>
Ouputs     :
*/
void DataSet::print_With_Index(std::vector<int> idx) const {
  // Logical but prints the features
  for (long unsigned int i = 0; i < this->features.size(); ++i) {
    std::cout << features[i] << "\t";
  }
  std::cout << "\n";
  // Logical but Prints the samples
  for (int i : idx) {
    for (long unsigned int j = 0; j < this->samples[i].size(); ++j) {
      std::cout << samples[i][j] << "|\t";
    }
    std::cout << labels[i] << "|\t";
    std::cout << "\n";
  }
  std::cout << "\n";
}

/*
Returns the features of the DataSet
Parameters :
Inputs     :
Ouputs     : vector<string>
*/
std::vector<std::string> DataSet::get_Features() const {
  return this->features;
}

/*
Returns the Values of the DataSet
Parameters :
Inputs     :
Ouputs     : vector<vector<float>>
*/
std::vector<std::vector<float>> DataSet::get_Samples() const {
  return this->samples;
}

/*
Returns the Samples of the DataSet at a given index
Parameters : index
Inputs     : const vector<int>
Outputs    : vector<vector<float>>
*/
std::vector<std::vector<float>>
DataSet::get_Samples(const std::vector<int> &idx) const {
  std::vector<std::vector<float>> res;
  // No idx
  if (idx.empty()) {
    return {};
  }

  for (int i : idx) {
    if (i < this->samples_Number())
      res.push_back(this->samples[i]);
  }

  return res;
}

/*
Returns all the labels of the DataSet
Parameters :
Inputs     :
Outputs    : vector<float>
*/
std::vector<float> DataSet::get_Labels() const { return this->labels; }

/*
Return the labels of the DataSet at a given index
Parameters : index
Inputs     : const vector<int>
Outputs    : vector<float>
*/
std::vector<float> DataSet::get_Labels(const std::vector<int> &idx) const {
  std::vector<float> Col(0);
  // No idx
  if (idx.empty()) {
    return Col;
  }

  for (int row : idx) {
    // Check row in bounds
    if (row < this->labels_Number() && row >= 0)
      Col.push_back(this->labels[row]);
  }
  return Col;
}

/*
 */
int DataSet::element_Size() const { return sizeof(this->samples[0][0]); }

/*
Return True if there are no values in the DataSet
Parameters :
Inputs     :
Ouputs     : boolean
*/
bool DataSet::empty() const { return this->samples.empty(); }

/*
Return the length (= number of features) of the DataSet
Parameters :
Inputs     :
Ouputs     : int
*/
int DataSet::features_Length() const { return this->features.size(); }

/*
Return the height (= number of samples) of the DataSet
Parameters :
Inputs     :
Ouputs     : int
*/
int DataSet::samples_Number() const { return this->samples.size(); }

/*
Return the quantity of the DataSet's labels
Parameters :
Inputs     :
Ouputs     : int
*/
int DataSet::labels_Number() const { return this->labels.size(); }

/*
Returns the specified column of the dataset
Parameters : column, index
Inputs     : int, const vector<int>
Ouputs     : vector<float>
*/
std::vector<float> DataSet::get_Column(int position,
                                       const std::vector<int> &idx) const {

  std::vector<float> Col(idx.size(), 0);

  // Check olumn in bounds
  if (position >= this->features_Length() || position < 0) {
    return Col;
  }
  // No index
  if (idx.empty()) {
    return Col;
  }

  // Col.reserve(idx.size());
  int i = 0;
  for (int row : idx) {
    // Check row in bounds
    if (row < this->samples_Number() && row >= 0){
      Col[i] = this->samples[row][position];
    }
    i++;
    // Col.push_back(this->samples[row][position]);
  }

  return Col;
}

/*
Return 2 vector which contains the index of each subtree datasets after split
Split is based on the criteria on a row at the column specified by position
Parameters : position, criterion, index
Inputs     : int, float, const vector<int>
Ouputs     : tuple<optional<vector<int>, optional<vector<int>>
*/
std::tuple<std::optional<std::vector<int>>, std::optional<std::vector<int>>>
DataSet::split(int position, float criterion,
               const std::vector<int> &idx) const {
  // Check column in bounds
  if (position >= this->features_Length() || position < 0) {
    return {};
  }
  // No index
  if (idx.empty()) {
    return {};
  }
  std::vector<int> sub_Index_Right;
  std::vector<int> sub_Index_Left;

#pragma omp parallel for
  for (int row : idx) {
    // std::cout << "Thread " << omp_get_thread_num() << std::endl;
    //  Row in bounds
    //  Push back sucks
    if (row < this->samples_Number() && row >= 0) {
      if (this->samples[row][position] < criterion) {
        sub_Index_Left.push_back(row);
      } else {
        sub_Index_Right.push_back(row);
      }
    }
  }
  return std::make_tuple(sub_Index_Left, sub_Index_Right);
}

/*
Computes the Mean of a given Column of the DataSet
Parameters : position, index
Inputs     : int, const vector<int>
Outputs    : float
*/
float DataSet::column_Mean(int position, const std::vector<int> &idx) const {

  float mean = 0;

  // Check column in bounds
  if (position >= this->features_Length() || position < 0) {
    return mean;
  }
  // No index
  if (idx.empty()) {
    return mean;
  }

  std::vector<float> current_Column = this->get_Column(position, idx);
  int len = current_Column.size();

  // To prevent dividing by 0
  if (len == 0) {
    return mean;
  }

  mean = std::reduce(std::execution::par_unseq, current_Column.begin(),
                     current_Column.end(), 0.0f);
  mean /= len;
  return mean;
}

/*
Computes the Mean of the values of the DataSet's labels
Parameters : index
Inputs     : const vector<int>
Outputs    : float
*/
float DataSet::labels_Mean(const std::vector<int> &idx) const {
  float mean = -1;
  // No index
  if (idx.empty()) {
    return mean;
  }

  std::vector<float> current_Labels = this->get_Labels(idx);
  int len = current_Labels.size();

  // To prevent dividing by 0
  if (len == 0) {
    return mean;
  }

  mean = std::reduce(std::execution::par_unseq, current_Labels.begin(),
                     current_Labels.end(), 0.0f) /
         len;
  return (mean);
}

/*
Computes the mean of all the labels of the DataSet
Parameters :
Inputs     :
Outputs    : float
*/
float DataSet::whole_Labels_Mean() const {
  float mean = -1;

  std::vector<float> current_Labels = this->get_Labels();
  int len = current_Labels.size();

  // To prevent dividing by 0
  if (len == 0) {
    return mean;
  }

  mean = std::reduce(std::execution::par_unseq, current_Labels.begin(),
                     current_Labels.end(), 0.0f) /
         len;
  return (mean);
}

/*
Computes the Variance of the  DataSet on the labels
idx represents the index that the object above can use
Parameters : index
Inputs     : const vector<int>
Outputs    : float
*/
float DataSet::labels_Variance(const std::vector<int> &idx) const {
  // No index
  if (idx.empty()) {
    return 0.0;
  }

  std::vector<float> current_Labels = this->get_Labels(idx);

  int len = current_Labels.size();

  // To prevent dividing by 0
  if (len == 0) {
    return 0.0;
  }

  float mean = this->labels_Mean(idx);

  std::vector<float> tmp_res(len);

  float sum = 0.0;

#pragma omp parallel for reduction(+ : sum)
  for (int i = 0; i < len; ++i) {
    float difference = current_Labels[i] - mean;
    sum += difference * difference;
  }

  float variance = sum / len;

  return variance;
}