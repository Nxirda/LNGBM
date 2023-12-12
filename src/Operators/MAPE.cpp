#include <cmath>

#include "MAPE.hpp"
#include "TreeNode.hpp"

/********************/
/*                  */
/*    MAPE CLASS    */
/*                  */
/********************/

/*
Constructor
Inputs  : DecisionTree*
Outputs :
*/
MAPE::MAPE() {}

/*
Constructor
Inputs  : DecisionTree*
Outputs :
*/
/* MAPE::MAPE(std::shared_ptr<TreeNode> tree_Node) { this->tree_Node =
 * tree_Node; }
 */
/*
Setter for the tree pointer
Inputs  : DecisionTree*
Outputs : bool
*/
/* bool MAPE::set_Node(std::shared_ptr<TreeNode> tree_Node) {
  this->tree_Node = tree_Node;
  this->split_Criteria = 0.0;
  if (this->tree_Node) {
    return true;
  }
  return false;
} */

/*
Destructor
Inputs  :
Outputs :
*/
MAPE::~MAPE() {}

/*
Print function to see the name of the operator
(For debugging mainly)
Inputs  :
Outputs :
*/
void MAPE::print() {
  std::cout << "=== Operator is : " << this->name << " ===\n";
}

/*
Returns the best splitting criteria for RIV algorithm
Inputs  :
Outputs : float
*/
// float MAPE::get_Best_Split_Criterion() const { return this->split_Criterion;
// }

/*
Sets the split criteria as the value given
Inputs  : float
Outputs :
*/
// void MAPE::set_Split_Criterion(float value) { this->split_Criterion = value;
// }

/*
Computes the Mean Absolute Percentage Error of a split on a given column
Inputs  : int
Outputs : float
*/
float MAPE::splitting_MAPE(int position, const DataSet &data,
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
  int size = (int)labels.size();

  // Computes the Mean Absolute Percentage Error for left child
  float left_Prediction = data.labels_Mean(left_index);
  float left_MAPE = 0;
  for (int idx : left_index) {
    if (idx < size)
      left_MAPE += (abs(labels[idx] - left_Prediction)) / left_Prediction;
  }

  // Computes the Mean Absolute Percentage Error for left child
  float right_Prediction = data.labels_Mean(right_index);
  float right_MAPE = 0;
  for (int idx : right_index) {
    if (idx < size)
      right_MAPE += (abs(labels[idx] - right_Prediction)) / right_Prediction;
  }

  left_MAPE *= 100;
  right_MAPE *= 100;

  // Compute the result of MAE for the split at position
  float res = (left_MAPE + right_MAPE) / base_Population;
  return res;
}

/*
Search for the best feature to split the dataset on at a given Node
Inputs :
Ouputs : int
*/
std::tuple<int, float> MAPE::find_Best_Split(const DataSet &data,
                                             std::vector<int> index) const {
  int best_Feature = 0;
  // We try to minimize the mean absolute error for a split
  float min_MAPE = INT_MAX;

  std::vector<std::string> features = data.get_Features();

  for (unsigned long int i = 0; i < features.size(); ++i) {
    float tmp_var = splitting_MAPE(i, data, index);
    if (tmp_var < min_MAPE) {
      min_MAPE = tmp_var;
      best_Feature = i;
    }
  }
  //this->set_Split_Criteria(this->tree_Node->node_Column_Mean(best_Feature));
  float criterion = data.column_Mean(best_Feature, index);
  return std::make_tuple(best_Feature, criterion);
}