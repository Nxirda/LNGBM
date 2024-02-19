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

void transpose_Matrix(std::vector<std::vector<double>> &matrix) {

  if (matrix.empty() || matrix[0].empty()) {
    // Matrix is empty or has no elements
    return;
  }

  const size_t rows = matrix.size();
  const size_t cols = matrix[0].size();

  // Create a new matrix with dimensions swapped
  std::vector<std::vector<double>> transposed(cols, std::vector<double>(rows));

  // Copy transposed elements
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      transposed[j][i] = matrix[i][j];
    }
  }

  // Assign transposed matrix back to original
  matrix = std::move(transposed);
}

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

  double val;
  // Read the other rows
  while (getline(file, line)) {
    // string stream of current line
    std::stringstream ss(line);

    // tmp vector : represent a row in the matrix
    std::vector<double> tmp;

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

  // Transpose matrix (might change that) to have fast column accees instead of
  // lines as we use columns more often

  transpose_Matrix(this->samples);

  // Close file
  file.close();
}

/*
Default Constructor
Parameters :
Inputs     :
Ouputs     : Object of DataSet Class
*/
DataSet::DataSet() {
  this->labels = std::vector<double>();
  this->features = std::vector<std::string>();
  this->samples = std::vector<std::vector<double>>();
}

/*
Explicit Constructor, takes three vectors and builds a DataSet Object
Parameters : features, samples, labels
Inputs     : vector<string>, vector<vector<double>>, vector<double>
Ouputs     : Object of DataSet Class
*/
DataSet::DataSet(const std::vector<std::string> &features,
                 const std::vector<std::vector<double>> &samples,
                 const std::vector<double> &labels) {
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
DataSet::DataSet(const DataSet &data, const std::vector<int> &idx) {
  this->features = data.get_Features();
  this->samples = data.get_Samples(idx);
  this->labels = data.get_Labels(idx);
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
void DataSet::print_With_Index(const std::vector<int> &idx) const {
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
Ouputs     : vector<vector<double>>
*/
std::vector<std::vector<double>> DataSet::get_Samples() const {
  return this->samples;
}

/*
Returns the Samples of the DataSet at a given index
Parameters : index
Inputs     : const vector<int>
Outputs    : vector<vector<double>>
*/
std::vector<std::vector<double>>
DataSet::get_Samples(const std::vector<int> &idx) const {
  // No idx
  if (idx.empty()) {
    return {};
  }

  std::vector<std::vector<double>> res;
  // As we could have less elements due to condition
  // easier to just reserve the max size
  res.reserve(this->samples.size());
  for (size_t i = 0; i < this->samples.size(); ++i) {
    std::vector<double> tmp;
    tmp.reserve(idx.size());
    for (int j : idx) {
      if (j >= 0 && j < this->samples[0].size())
        tmp.push_back(this->samples[i][j]);
    }
    res.push_back(tmp);
  }

  return res;
}

/*
Returns all the labels of the DataSet
Parameters :
Inputs     :
Outputs    : vector<double>
*/
std::vector<double> DataSet::get_Labels() const { return this->labels; }

/*
Return the labels of the DataSet at a given index
Parameters : index
Inputs     : const vector<int>
Outputs    : vector<double>
*/
std::vector<double> DataSet::get_Labels(const std::vector<int> &idx) const {
  // No idx
  if (idx.empty()) {
    return {};
  }

  std::vector<double> column;
  // As we could have less elements due to condition
  // easier to just reserve the max size
  column.reserve(idx.size());

  for (int row : idx) {
    // Check row in bounds
    if (row < this->labels_Number() && row >= 0)
      column.push_back(this->labels[row]);
  }
  return column;
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
int DataSet::samples_Number() const {
  if (this->empty())
    return 0;

  return this->samples[0].size(); 
}

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
Ouputs     : vector<double>
*/
const std::vector<double> &DataSet::get_Column(int position) const{
                                        //const std::vector<int> &idx) const {

  // Check olumn in bounds
  if (/*idx.empty() || */position >= this->features_Length() || position < 0) {
    return {};
  }

  //std::vector<double> column;
  //column.reserve(idx.size());

//#pragma omp parallel for shared(column)
  /* for (int row : idx) {
    // Check row in bounds
    if (row < this->samples_Number() && row >= 0) {   
      column.push_back(this->samples[position][row]);
    }
  } */

  return this->samples[position];
}

/*
Return 2 vector which contains the index of each subtree datasets after
split Split is based on the criteria on a row at the column specified by
position Parameters : position, criterion, index Inputs     : int, double,
const vector<int> Ouputs     : tuple<optional<vector<int>,
optional<vector<int>>
*/
std::tuple<std::optional<std::vector<int>>, std::optional<std::vector<int>>>
DataSet::split(int position, double criterion,
               const std::vector<int> &idx) const {
  // Check column in bounds
  if (idx.empty() || position >= this->features_Length() || position < 0) {
    return {};
  }

  std::vector<int> sub_Index_Right;
  std::vector<int> sub_Index_Left;

  sub_Index_Right.reserve(idx.size() >> 1);
  sub_Index_Left.reserve(idx.size() >> 1);

#pragma omp parallel for
  for (int row : idx) {
    //  Row in bounds
    //  Push back sucks
    if (row < this->samples_Number() && row >= 0) {
      if (this->samples[position][row] < criterion) {
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
Outputs    : double
*/
double DataSet::column_Mean(int position, const std::vector<int> &idx) const {

  double mean = -1.0;

  // Check column in bounds
  if (idx.empty() || position >= this->features_Length() || position < 0) {
    return mean;
  }

  // Missing index handling
  std::vector<double> current_Column;
  current_Column.reserve(idx.size());
  //this->get_Column(position);
  for(int i : idx)
    current_Column.push_back(this->get_Column(position)[i]);

  double len = current_Column.size();

  // To prevent dividing by 0
  if (len == 0) {
    return mean;
  }

  mean = cblas_dasum(len, current_Column.data(), 1.0);
  /* mean = std::reduce(std::execution::par_unseq, current_Column.begin(),
                     current_Column.end(), 0.0f); */
  mean *= (1.0 / len);
  return mean;
}

/*
Computes the Mean of the values of the DataSet's labels
Parameters : index
Inputs     : const vector<int>
Outputs    : double
*/
double DataSet::labels_Mean(const std::vector<int> &idx) const {
  double mean = -1.0;
  // No index
  if (idx.empty()) {
    return mean;        
  }

  std::vector<double> current_Labels = this->get_Labels(idx);
  double len = current_Labels.size();

  // To prevent dividing by 0
  if (len == 0) {
    return mean;
  }

  mean = cblas_dasum(len, current_Labels.data(), 1.0);
  /* mean = std::reduce(std::execution::par_unseq, current_Labels.begin(),
                     current_Labels.end(), 0.0f) *
         (1.0 / len); */
  mean *= (1.0 / len);
  return (mean);
}

/*
Computes the mean of all the labels of the DataSet
Parameters :
Inputs     :
Outputs    : double
*/
double DataSet::whole_Labels_Mean() const {
  double mean = -1.0;

  std::vector<double> current_Labels = this->get_Labels();
  int len = current_Labels.size();

  // To prevent dividing by 0
  if (len == 0) {
    return mean;
  }

  mean = cblas_dasum(len, current_Labels.data(), 1.0);
  mean *= (1.0 / len);
  /* mean = std::reduce(std::execution::par_unseq, current_Labels.begin(),
                     current_Labels.end(), 0.0f) *
         (1.0 / len); */
  return (mean);
}

/*
Computes the Variance of the  DataSet on the labels
idx represents the index that the object above can use
Parameters : index
Inputs     : const vector<int>
Outputs    : double
*/
double DataSet::labels_Variance(const std::vector<int> &idx) const {
  // No index
  if (idx.empty()) {
    return -1.0;
  }

  std::vector<double> current_Labels = this->get_Labels(idx);

  int len = current_Labels.size();

  // To prevent dividing by 0
  if (len == 0) {
    return -1.0;
  }

  double mean = this->labels_Mean(idx);

  std::vector<double> tmp_res(len);

  double sum = 0.0;

#pragma omp parallel for reduction(+ : sum)
  for (int i = 0; i < len; ++i) {
    double difference = current_Labels[i] - mean;
    sum += difference * difference;
  }

  double variance = sum * (1.0 / len);

  return variance;
}