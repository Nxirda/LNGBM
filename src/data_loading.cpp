#include <fstream>
#include <iostream>
#include <sstream>   // std::stringstream
#include <stdexcept> // std::runtime_error
#include <string>
#include <vector>

#include "../include/data_loading.h"

using namespace std;

// https://www.gormanalysis.com/blog/reading-and-writing-csv-files-with-cpp/

/* Take the path of the file to read                         */
/* Instanciates an object of type Dataset from the given CSV */
/* Inputs : String                                           */
/* Ouputs : Object of Dataset Cass                           */
Dataset::Dataset(string FilePath) {
  // input filestream
  ifstream file;
  file.open(FilePath);

  // Make sure the file is open
  if (!file.is_open()) {
    throw runtime_error("Could not open given file");
  }

  string line;
  string colname;

  // If no error state flag
  if (file.good()) {
    // Extract the first row in the file
    getline(file, line);

    // Create a stringstream from line
    stringstream ss(line);

    // Extract each column name
    while (getline(ss, colname, ',')) {
      // Initialize Dataset Labels
      this->Labels.push_back(colname);
    }
  }

  float val;
  // Read the other rows
  while (getline(file, line)) {
    // string stream of current line
    stringstream ss(line);

    // tmp vector : represent a row in the matrix
    vector<float> tmp;

    while (ss >> val) {
      tmp.push_back(val);

      if (ss.peek() == ',') {
        ss.ignore();
      }
    }
    // Place the row in the 2D Matrix
    this->Values.push_back(tmp);
  }

  // Close file
  file.close();
}

/* Explicit Constructor, takes two vectors and builds a Dataset Object */
/* Inputs : vector<string>, vector<vector<float>>                      */
/* Ouputs : Object of Dataset Class                                    */
Dataset::Dataset(vector<string> L, vector<vector<float>> V) {
  this->Labels = L;
  this->Values = V;
}

/* Default Constructor              */
/* Inputs :                         */
/* Ouputs : Object of Dataset Class */
Dataset::Dataset() {}

/* Default Destructor */
/* Inputs :           */
/* Ouputs :           */
Dataset::~Dataset() {}

/* Naive print function of the Dataset */
/* Inputs :                            */
/* Ouputs :                            */
void Dataset::print() const {
  // Logical but Prints the Labels
  for (int i = 0; i < this->Labels.size(); ++i) {
    cout << Labels[i] << "\t";
  }
  cout << "\n";
  // Logical but Prints the Values
  for (int i = 0; i < int(this->Values.size()); ++i) {
    for (int j = 0; j < this->Values[0].size(); ++j) {
      cout << Values[i][j] << "|\t";
    }
    cout << "\n";
  }
  cout << "\n";
}

/* Returns the Labels of the Dataset */
/* Inputs :                          */
/* Ouputs : vector<string>           */
vector<string> Dataset::get_Labels() const { return this->Labels; }

/* Returns the Values of the Dataset */
/* Inputs :                          */
/* Ouputs : vector<vector<float>>    */
vector<vector<float>> Dataset::get_Values() const { return this->Values; }

/* Returns the specified column of the dataset */
/* Inputs : int                                */
/* Ouputs : vector<float>                      */
vector<float> Dataset::get_Column(int position) const {
  vector<float> Col;
  if (position > this->Label_length()) {
    return Col;
  }
  for (int j = 0; j < this->Entries_size(); ++j) {
    Col.push_back(this->get_Values()[j][position]);
  }
  return Col;
}

/* Return True if there are no values in the Dataset */
/* Inputs :                                          */
/* Ouputs : boolean                                  */
bool Dataset::empty() const { return this->Values.empty(); }

/* Return the length (= number of Labels) of the Dataset */
/* Inputs :                                              */
/* Ouputs : int                                          */
int Dataset::Label_length() const { return this->get_Labels().size(); }

/* Return the height (= number of entries)  of the Dataset*/
/* Inputs :                                              */
/* Ouputs : int                                          */
int Dataset::Entries_size() const { return this->get_Values().size(); }

/* Return 2 Datasets representing the splitting of the given Dataset on the */
/* given column label via a given limit (criteria)                          */
/* Inputs : int, float                                                      */
/* Ouputs : vector<Dataset>                                                 */
vector<Dataset> Dataset::split(int position,
                               float criteria) const {

  vector<vector<float>> subValuesRight;
  vector<vector<float>> subValuesLeft;
  for (int row = 0; row < this->Entries_size(); ++row) {
    if (this->get_Values()[row][position] < criteria) {
      subValuesLeft.push_back(this->get_Values()[row]);
    } else {
      subValuesRight.push_back(this->get_Values()[row]);
    }
  }
  Dataset D_Right{this->get_Labels(), subValuesRight};
  Dataset D_Left{this->get_Labels(), subValuesLeft};
  vector<Dataset> Res{D_Left, D_Right};
  return Res;
}

/* Computes the Mean of a given Column of the Dataset */
/* Inputs  : int                                      */
/* Outputs : float                                    */
float Dataset::Column_Mean(int position) {
  vector<float> Current_Column = this->get_Column(position);
  int len = Current_Column.size();
  //  check if there are values in the current node
  if (len <= 0) {
    return 0.0;
  }
  float mean = 0.0;
  for (float elem : Current_Column) {
    mean += elem;
  }
  mean /= len;
  return mean;
}

/* Computes the Variance of a given Column of the  Dataset */
/* Inputs  : int                                           */
/* Outputs : float                                         */
float Dataset::Column_Variance(int position) {
  vector<float> Current_Column = this->get_Column(position);
  int len = Current_Column.size();
  //  check if there are values in the current Column
  if (len <= 0) {
    return 0.0;
  }
  float mean = Column_Mean(position);
  float variance = 0.0;
  for (float elem : Current_Column) {
    float difference = elem - mean;
    variance += difference * difference;
  }
  variance /= len;
  return variance;
}

/* Computes the Global Variance of the  Dataset */
/* Inputs  :                                    */
/* Outputs : float                              */
float Dataset::Global_Variance(){
  int len = this->Label_length();
  float var_res = 0;
  for (int i = 0; i < len; ++i) {
    var_res += this->Column_Variance(i);
  }
  return var_res;
}