#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>   // std::stringstream
#include <stdexcept> // std::runtime_error
#include <string>
#include <vector>

#include "DataSet.hpp"

// https://www.gormanalysis.com/blog/reading-and-writing-csv-files-with-cpp/

/* 
Take the path of the file to read                         
Instanciates an object of type DataSet from the given CSV 
Inputs : String                                           
Ouputs : Object of DataSet Cass                           
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
      // Initialize DataSet Labels
      this->features.push_back(colname);
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
      tmp.push_back(val);

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
Explicit Constructor, takes two vectors and builds a DataSet Object 
Inputs : vector<string>, vector<vector<float>>                      
Ouputs : Object of DataSet Class                                    
*/
DataSet::DataSet(std::vector<std::string> features,
                 std::vector<std::vector<float>> samples) {
  this->features = features;
  this->samples = samples;
}

/* 
Default Constructor              
Inputs :                         
Ouputs : Object of DataSet Class 
*/
DataSet::DataSet() {}

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
Returns the specified column of the dataset
Inputs : int
Ouputs : vector<float>
*/
std::vector<float> DataSet::get_Column(int position,
                                       const std::vector<int> &idx) const {
  std::vector<float> Col;
  if (position > this->features_Length()) {
    return Col;
  }
  for (int row : idx) {
    Col.push_back(this->samples[row][position]);
  }
  return Col;
}

/*
Return 2 vector which contains the index of each subtree datasets after
split, which is based on the criteria on a row at the col position
Inputs : int, float
Ouputs : vector<vector<int>>*/
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
  int len = idx.size();
  // check if there are values in the current Column at the specified indexes
  if (len < 0 || position > this->features_Length() || position < 0) {
    return 0.0;
  }

  std::vector<float> current_Column = this->get_Column(position, idx);

  float mean = std::reduce(current_Column.begin(), current_Column.end(), 0l);
  mean /= len;
  return mean;
}

/*
Computes the Variance of the  DataSet on the labels
idx represents the index that the object above can use
Inputs  : std:vector<int>
Outputs : float
*/
float DataSet::column_Variance(const std::vector<int> &idx) const {
  int len = idx.size();
  //  check if there are values in the current Column
  if (len <= 0) {
    return 0.0;
  }

  std::vector<float> current_Column =
      this->get_Column(this->features_Length() - 1, idx);

  float mean = column_Mean(this->features_Length() - 1, idx);
  float variance = 0.0;

  for (int i : current_Column) {
    float difference = i - mean;
    variance += difference * difference;
  }
  variance /= len;
  return variance;
}