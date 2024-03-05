#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>   // std::stringstream
#include <stdexcept> // std::runtime_error
#include <vector>

#include <cblas.h>

#include "DataSet.hpp"

/********************/
/*                  */
/*  DATASET CLASS   */
/*                  */
/********************/

//
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
  this->features = features;
  this->samples = samples;
  this->labels = labels;
}

//
DataSet::DataSet(const DataSet &data, const std::vector<size_t> &idx) {
  this->features = std::move(data.get_Features());
  this->samples = std::move(data.get_Samples(idx));
  this->labels = std::move(data.get_Labels(idx));
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
  std::vector<std::vector<double>> res;
  res.reserve(this->samples.size());

  size_t i;
  for (i = 0; i < this->samples.size(); ++i) {
    std::vector<double> tmp;
    tmp.reserve(idx.size());
    for (const auto &j : idx) {
      if (j < this->samples[0].size())
        tmp.push_back(this->samples[i][j]);
    }
    res.push_back(tmp);
  }

  return res;
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
    std::cerr << "Position specified is out of the matrix\n";
    exit(1);
  }
  return this->samples[position];
}

//
std::tuple<std::optional<std::vector<size_t>>,
           std::optional<std::vector<size_t>>>
DataSet::split(size_t position, double criterion,
               const std::vector<size_t> &idx) const {
  // Check column in bounds
  if (idx.empty() || position >= this->features_Number()) {
    return {std::nullopt, std::nullopt};
  }

  size_t samples_Number = this->samples_Number();
  const std::vector<double> &samples = this->get_Column(position);

  std::vector<size_t> sub_Index_Right;
  std::vector<size_t> sub_Index_Left;

  sub_Index_Right.reserve(idx.size() >> 1);
  sub_Index_Left.reserve(idx.size() >> 1);

  for (const auto &row : idx) {
    //  Row in bounds (matrix is transposed)
    if (row >= samples_Number) {
      std::cerr << "row index is out of the matrix\n";
    } else {
      if (samples[row] < criterion) {
        sub_Index_Left.push_back(row);
      } else {
        sub_Index_Right.push_back(row);
      }
    }
  }

  return {std::move(sub_Index_Left), std::move(sub_Index_Right)};
}

//
double DataSet::column_Mean(size_t position,
                            const std::vector<size_t> &idx) const {

  // Check column in bounds
  if (idx.empty() || position >= this->features_Number()) {
    return 0.0;
  }

  double mean = 0.0;
  double len = 0.0;
  size_t col_Size = this->get_Column(position).size();

  for (const auto &i : idx) {
    if (i > col_Size) {
      std::cerr << "Index of column is outside matrix dimensions\n";
    } else {
      len += 1.0;
      mean += this->get_Column(position)[i];
    }
  }

  // To prevent dividing by 0
  if (len == 0.0) {
    return len;
  }

  mean *= (1.0 / len);
  return mean;
}

//
double DataSet::labels_Mean(const std::vector<size_t> &idx) const {

  // No index
  if (idx.empty()) {
    return 0.0;
  }

  double mean = 0.0;
  double len = 0.0;

  size_t labels_Size = this->labels_Number();
  const std::vector<double> &labels = this->get_Labels();

  for (const auto &i : idx) {
    if (i > labels_Size) {
      std::cerr << "Index of column is outside matrix dimensions\n";
    } else {
      len += 1.0;
      mean += labels[i];
    }
  }

  // To prevent dividing by 0
  if (len == 0.0) {
    return len;
  }

  mean *= (1.0 / len);
  return (mean);
}

//
double DataSet::whole_Labels_Mean() const {

  double mean = 0.0;
  size_t len = this->labels_Number();

  // To prevent dividing by 0
  if (len == 0) {
    return mean;
  }

  mean = cblas_dasum(len, this->get_Labels().data(), 1.0);
  mean *= (1.0 / len);

  return (mean);
}

//
double DataSet::labels_Variance(const std::vector<size_t> &idx) const {
  // No index
  if (idx.empty()) {
    return 0.0;
  }

  double len = 0.0;
  double sum = 0.0;
  double mean = this->labels_Mean(idx);

  size_t labels_Size = this->labels_Number();
  const std::vector<double> &labels = this->get_Labels();
  double difference;

  for (const auto &i : idx) {
    if (i > labels_Size) {
      std::cerr << "Index of column is outside matrix dimensions\n";
    } else {
      difference = labels[i] - mean;
      sum += difference * difference;
      len += 1.0;
    }
  }

  // To prevent dividing by 0
  if (len == 0) {
    return len;
  }

  double variance = sum * (1.0 / len);

  return variance;
}