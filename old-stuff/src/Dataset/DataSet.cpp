#include <algorithm>
#include <cblas.h>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>   // std::stringstream
#include <stdexcept> // std::runtime_error
#include <vector>

#include "DataSet.hpp"

/********************/
/*                  */
/*  DATASET CLASS   */
/*                  */
/********************/

//
void DataSet::transpose_Matrix(std::vector<std::vector<double>> &matrix) {

  if (matrix.empty() || matrix[0].empty()) {
    // Matrix is empty or has no elements
    return;
  }

  const size_t rows = matrix.size();
  const size_t cols = matrix[0].size();

  // Create a new matrix with dimensions swapped
  std::vector<std::vector<double>> transposed(cols, std::vector<double>(rows));

  // Copy transposed elements
  size_t i, j;
  for (i = 0; i < rows; ++i) {
    for (j = 0; j < cols; ++j) {
      transposed[j][i] = matrix[i][j];
    }
  }

  // Assign transposed matrix back to original
  matrix = std::move(transposed);
}

// https://www.gormanalysis.com/blog/reading-and-writing-csv-files-with-cpp/
DataSet::DataSet(std::string file_Path) {
  // input filestream
  std::ifstream file;
  file.open(file_Path);

  // Make sure the file is open
  if (!file.is_open()) {
    //throw std::runtime_error("Could not open given file");
    std::cerr << " < Could not open given file : " << file_Path << "\n";
    exit(1);
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

  // Transpose matrix (might change that) to have fast column access instead of
  // lines as we use columns more often (will be slow on huge matrices)

  transpose_Matrix(this->samples);

  // Close file
  file.close();
}

//
DataSet::DataSet() {
  this->labels = std::vector<double>();
  this->features = std::vector<std::string>();
  this->samples = std::vector<std::vector<double>>();
}

//
DataSet::DataSet(const std::vector<std::string> &features,
                 const std::vector<std::vector<double>> &samples,
                 const std::vector<double> &labels) {
  this->labels = labels;
  this->samples = samples;
  this->features = features;
}

//
DataSet::DataSet(const DataSet &data, const std::vector<size_t> &idx) {
  this->labels = std::move(data.get_Labels(idx));
  this->features = std::move(data.get_Features());
  this->samples = std::move(data.get_Samples(idx));
}

//
DataSet::~DataSet() {}

//
void DataSet::print() const {
  // Logical but prints the features
  for (size_t i = 0; i < this->features.size(); ++i) {
    std::cout << features[i] << "\t";
  }
  std::cout << "\n";
  // Logical but Prints the samples
  size_t i, j;
  for (i = 0; i < this->samples.size(); ++i) {
    for (j = 0; j < this->samples[0].size(); ++j) {
      std::cout << samples[i][j] << "|\t";
    }
    std::cout << labels[i] << "|\t";
    std::cout << "\n";
  }

  std::cout << "\n";
}

//
void DataSet::print_With_Index(const std::vector<size_t> &idx) const {
  // Logical but prints the features
  for (size_t i = 0; i < this->features.size(); ++i) {
    std::cout << features[i] << "\t";
  }
  std::cout << "\n";
  // Logical but Prints the samples
  size_t j;
  for (size_t i : idx) {
    for (j = 0; j < this->samples[i].size(); ++j) {
      std::cout << samples[i][j] << "|\t";
    }
    std::cout << labels[i] << "|\t";
    std::cout << "\n";
  }
  std::cout << "\n";
}

//
const std::vector<std::string> &DataSet::get_Features() const {
  return this->features;
}

//
const std::vector<std::vector<double>> &DataSet::get_Samples() const {
  return this->samples;
}

//
std::vector<std::vector<double>>
DataSet::get_Samples(const std::vector<size_t> &idx) const {
  // No idx
  if (idx.empty()) {
    return {};
  }

  // As we could have less elements due to condition
  // easier to just reserve the max size
  std::vector<std::vector<double>> samples_At_Index;
  samples_At_Index.reserve(this->samples.size());

  size_t i;
  for (i = 0; i < this->samples.size(); ++i) {
    std::vector<double> tmp;
    tmp.reserve(idx.size());
    for (const auto &j : idx) {
      if (j < this->samples[0].size())
        tmp.push_back(this->samples[i][j]);
    }
    samples_At_Index.push_back(tmp);
  }

  return samples_At_Index;
}

//
const std::vector<double> &DataSet::get_Labels() const { return this->labels; }

//
std::vector<double> DataSet::get_Labels(const std::vector<size_t> &idx) const {
  // No idx
  if (idx.empty()) {
    return {};
  }

  // As we could have less elements due to condition
  // easier to just reserve the max size
  std::vector<double> labels_At_Index;
  labels_At_Index.reserve(idx.size());

  for (const auto &row : idx) {
    // Check row in bounds
    if (row < this->labels_Number())
      labels_At_Index.push_back(this->labels[row]);
  }
  return labels_At_Index;
}

//
int DataSet::element_Size() const { return sizeof(this->samples[0][0]); }

//
bool DataSet::empty() const { return this->samples.empty(); }

//
size_t DataSet::features_Number() const { return this->features.size(); }

//
size_t DataSet::samples_Number() const {
  if (this->empty())
    return 0;

  return this->samples[0].size();
}

//
size_t DataSet::labels_Number() const { return this->labels.size(); }

//
const std::vector<double> &DataSet::get_Column(size_t position) const {
  // Check column in bounds
  if (position >= this->features_Number()) {
    std::cerr << " < Position specified is out of the matrix\n";
    exit(1);
  }
  return this->samples[position];
}