#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>   // std::stringstream
#include <stdexcept> // std::runtime_error
#include <string>
#include <vector>

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
Inputs : String
Ouputs : Object of DataSet Class
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
Inputs : String
Ouputs : Object of DataSet Class
*/
void DataSet::load(std::string file_Path) {
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
        // Initialize DataSet features
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
      // Ignore label column
      if (!ss.eof())
        tmp.push_back(val);

      if (ss.peek() == ',') {
        ss.ignore();
      }
    }
    // Place the row in the 2D Matrix
    this->samples.push_back(tmp);
  }

  std::cout << "DONT FORGET TO DELETE THIS IN LOAD FUNCTION\n";
  this->labels = std::vector<float>(this->samples.size(), 0);

  // Close file
  file.close();
}

/*
Explicit Constructor, takes two vectors and builds a DataSet Object
Inputs : vector<string>, vector<vector<float>>
Ouputs : Object of DataSet Class
*/
DataSet::DataSet(std::vector<std::string> features,
                 std::vector<std::vector<float>> samples,
                 std::vector<float> labels) {
  this->features = features;
  this->samples = samples;
  this->labels = labels;
}

/*
Default Constructor
Inputs :
Ouputs : Object of DataSet Class
*/
DataSet::DataSet() {
  this->labels = std::vector<float>();
  this->features = std::vector<std::string>();
  this->samples = std::vector<std::vector<float>>();
}

/* Default Destructor */
/* Inputs :           */
/* Ouputs :           */
DataSet::~DataSet() {}

/*
Naive print function of the DataSet
Inputs :
Ouputs :
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
Inputs : vector<int>
Ouputs :
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
Inputs :
Ouputs : vector<string>
*/
std::vector<std::string> DataSet::get_Features() const {
  return this->features;
}

/*
Returns the Values of the DataSet
Inputs :
Ouputs : vector<vector<float>>
*/
std::vector<std::vector<float>> DataSet::get_Samples() const {
  return this->samples;
}

/**/
std::vector<float> DataSet::get_Labels(const std::vector<int> &idx) const {
  std::vector<float> Col(0);
  for (int row : idx) {
    if (row <= this->labels_Number())
      Col.push_back(this->labels[row]);
  }
  return Col;
}

/*
Return True if there are no values in the DataSet
Inputs
Ouputs : boolean
*/
bool DataSet::empty() const { return this->samples.empty(); }

/*
Return the length (= number of features) of the DataSet
Inputs :
Ouputs : int
*/
int DataSet::features_Length() const { return this->features.size(); }

/*
Return the height (= number of samples) of the DataSet
Inputs :
Ouputs : int
*/
int DataSet::samples_Number() const { return this->samples.size(); }

/*
Return the quantity of the DataSet's labels
Inputs :
Ouputs : int
*/
int DataSet::labels_Number() const { return this->labels.size(); }

/*
Returns the specified column of the dataset
Inputs : int
Ouputs : vector<float>
*/
std::vector<float> DataSet::get_Column(int position,
                                       const std::vector<int> &idx) const {

  std::vector<float> Col(0);

  if (idx.empty()) { //(position > len) {//|| position < 0){
    return Col;
  }

  long unsigned int len = idx.size();

  for (long unsigned int i = 0; i < len; ++i) {
    if ((int)i < this->samples_Number())
      Col.push_back(this->samples[idx[i]][position]);
  }

  return Col;
}

/*
Return 2 vector which contains the index of each subtree datasets after
split, which is based on the criteria on a row at the col position
Inputs : int, float
Ouputs : vector<vector<int>>
*/
std::vector<std::vector<int>>
DataSet::split(int position, float criteria,
               const std::vector<int> &idx) const {

  std::vector<int> sub_Index_Right;
  std::vector<int> sub_Index_Left;

  for (int row : idx) {
    if (this->samples[row][position] < criteria) {
      sub_Index_Left.push_back(row);
    } else {
      sub_Index_Right.push_back(row);
    }
  }
  std::vector<std::vector<int>> res{sub_Index_Left, sub_Index_Right};
  return res;
}

/*
Computes the Mean of a given Column of the DataSet
Inputs  : int
Outputs : float
*/
float DataSet::column_Mean(int position, const std::vector<int> &idx) const {

  float mean = 0;

  if (idx.empty()) {
    return mean;
  }

  std::vector<float> current_Column = this->get_Column(position, idx);
  int len = current_Column.size();

  mean = std::reduce(current_Column.begin(), current_Column.end(), 0.0);
  mean /= len;
  return mean;
}

/*
Computes the Mean of the values of the DataSet's labels
Inputs  : const vector<int>
Outputs : float
*/
float DataSet::labels_Mean(const std::vector<int> &idx) const {
  float mean = 0;

  if (idx.empty()) {
    return mean;
  }

  std::vector<float> current_Labels = this->get_Labels(idx);
  int len = current_Labels.size();

  mean = std::reduce(current_Labels.begin(), current_Labels.end(), 0.0);
  mean /= len;
  return (mean);
}

/*
Computes the Variance of the  DataSet on the labels
idx represents the index that the object above can use
Inputs  : std:vector<int>
Outputs : float
*/
float DataSet::column_Variance(const std::vector<int> &idx) const {
  //  check if there are values in the current Column
  if (idx.size() <= 0) {
    return 0.0;
  }

  std::vector<float> current_Labels = this->get_Labels(idx);
  int len = current_Labels.size();
  float mean = this->labels_Mean(idx);
  float variance = 0.0;

  for (float i : current_Labels) {
    float difference = i - mean;
    variance += difference * difference;
  }
  variance /= len;
  return variance;
}