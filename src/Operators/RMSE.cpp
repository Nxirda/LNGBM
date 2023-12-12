#include <cmath>

#include "RMSE.hpp"
#include "TreeNode.hpp"

/********************/
/*                  */
/*    RMSE CLASS    */
/*                  */
/********************/

/*
Constructor
Input  : DecisionTree*
Output :
*/
RMSE::RMSE() {}

/*
Constructor
Input  : DecisionTree*
Output :
*/
/* RMSE::RMSE(std::shared_ptr<TreeNode> tree_Node) { this->tree_Node =
 * tree_Node; }
 */
/*
Setter for the tree pointer
Input  : DecisionTree*
Output : bool
*/
/* bool RMSE::set_Node(std::shared_ptr<TreeNode> tree_Node) {
  this->tree_Node = tree_Node;
  this->split_Criteria = 0.0;
  if (this->tree_Node) {
    return true;
  }
  return false;
} */

/*
Destructor
Input  :
Output :
*/
RMSE::~RMSE() {}

/*
Print function to see the name of the operator
(For debugging mainly)
Input  :
Output :
*/
void RMSE::print() {
  std::cout << "=== Operator is : " << this->name << " ===\n";
}

/*
Returns the best splitting criteria for RIV algorithm
Input  :
Output : float
*/
/* float RMSE::get_Best_Split_Criteria() { return this->split_Criteria; }
 */
/*
Sets the split criteria as the value given
Input  : float
Output :
*/
/* void RMSE::set_Split_Criteria(float value) { this->split_Criteria = value; }
 */
/*
Computes the Root Mean Square Error of a split on a given column
Inputs  : int
Outputs : float
*/
float RMSE::splitting_RMSE(int position, const DataSet &data,
                           std::vector<int> index) const {

  // Computes the split criteria, needs to be not hardcoded in the future
  float right_RMSE = 0;
  float left_RMSE = 0;
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

  // Computes the Mean Absolute Error for left child
  float left_Prediction = data.labels_Mean(left_index);

  for (int idx : left_index) {
    if (idx < size)
      left_RMSE += pow((abs(labels[idx] - left_Prediction)), 2);
  }

  // Computes the Mean Absolute Error for left child
  float right_Prediction = data.labels_Mean(right_index);

  for (int idx : right_index) {
    if (idx < size)
      right_RMSE += pow((abs(labels[idx] - right_Prediction)), 2);
  }

  // Compute the result of MAE for the split at position
  float res = sqrt((left_RMSE + right_RMSE) / base_Population);
  return res;
}

/*
Search for the best feature to split the dataset on at a given Node
Inputs :
Ouputs : int
*/
std::tuple<int, float> RMSE::find_Best_Split(const DataSet &data,
                                         std::vector<int> index) const{
  int best_Feature = 0;
  float tmp_var = 0;
  // We try to minimize the mean absolute error for a split
  float min_RMSE = INT_MAX;

  std::vector<std::string> features =
      data.get_Features();

  for (unsigned long int i = 0; i < features.size() - 1; ++i) {
    tmp_var = splitting_RMSE(i, data, index);
    if (tmp_var < min_RMSE) {
      min_RMSE = tmp_var;
      best_Feature = i;
    }
  }
 /*  float mean = this->tree_Node->node_Column_Mean(best_Feature);
  this->set_Split_Criteria(mean);
  return best_Feature; */
  float criterion = data.column_Mean(best_Feature, index);
  return std::make_tuple(best_Feature, criterion);
}