#include <cmath>

#include "MAE.hpp"
#include "TrainingElement.hpp"

/********************/
/*                  */
/*    MAE CLASS     */
/*                  */
/********************/

/*
Constructor
Inputs  : DecisionTree*
Outputs :
*/
MAE::MAE() {}

/*
Destructor
Inputs  :
Outputs :
*/
MAE::~MAE() {}

/*
Print function to see the name of the operator
(For debugging mainly)
Inputs  :
Outputs :
*/
void MAE::print() {
  std::cout << "=== Operator is : " << this->name << " ===\n";
}

/*
Computes the Mean Absolute Error of a split on a given column
Inputs  : int
Outputs : float
*/
float MAE::splitting_MAE(int position, const DataSet &data,
                         std::vector<int> index) const {

  // Computes the split criteria, needs to be not hardcoded in the future
  float split_Criteria = data.column_Mean(position, index);

  // Computes the DataSet Row Indexes that child nodes can access
  auto [left_index, right_index] = data.split(position, split_Criteria, index);

  float base_Population = index.size();

  // Creating a left child
  TreeNode left_Child{};

  // Creating a right child
  TreeNode right_Child{};

  // Get the labels
  std::vector<float> labels = data.get_Labels(index);

  // Computes the Mean Absolute Error for left child
  float left_Prediction = data.labels_Mean(left_index.value());
  float left_MAE = 0;
  for (int idx : left_index.value()) {
    left_MAE += abs(labels[idx] - left_Prediction);
  }

  // Computes the Mean Absolute Error for left child
  float right_Prediction = data.labels_Mean(right_index.value());
  float right_MAE = 0;
  for (int idx : right_index.value()) {
    right_MAE += abs(labels[idx] - right_Prediction);
  }

  // Compute the result of MAE for the split at position
  float res = (left_MAE + right_MAE) / base_Population;
  return res;
}

/*
Search for the best feature to split the dataset on at a given Node
Inputs :
Ouputs : int
*/
std::tuple<int, float> MAE::find_Best_Split(const DataSet &data,
                                            std::vector<int> index) const {
  int best_Feature = 0;
  // We try to minimize the mean absolute error for a split
  float min_MAE = INT_MAX;

  std::vector<std::string> features = data.get_Features();

  for (unsigned long int i = 0; i < features.size(); ++i) {
    float tmp_var = splitting_MAE(i, data, index);
    if (tmp_var < min_MAE) {
      min_MAE = tmp_var;
      best_Feature = i;
    }
  }
  float criterion = data.column_Mean(best_Feature, index);
  return std::make_tuple(best_Feature, criterion);
}