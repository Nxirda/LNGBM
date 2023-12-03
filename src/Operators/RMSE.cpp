#include <cmath>

#include "RMSE.hpp"
#include "TreeNode.hpp"

/*
Constructor
Input  : DecisionTree*
Output :
*/
RMSE::RMSE(std::shared_ptr<TreeNode> tree_Node) { this->tree_Node = tree_Node; }

/*
Setter for the tree pointer
Input  : DecisionTree*
Output : bool
*/
bool RMSE::set_Node(std::shared_ptr<TreeNode> tree_Node) {
  this->tree_Node = tree_Node;
  this->split_Criteria = 0.0;
  if (this->tree_Node) {
    return true;
  }
  return false;
}

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
float RMSE::get_Best_Split_Criteria() { return this->split_Criteria; }

/*
Sets the split criteria as the value given
Input  : float
Output :
*/
void RMSE::set_Split_Criteria(float value) { this->split_Criteria = value; }

/**/
float RMSE::splitting_MAE(int position) {

  // Computes the split criteria, needs to be not hardcoded in the future
  float split_Criteria = this->tree_Node->node_Column_Mean(position);

  // Computes the DataSet Row Indexes that child nodes can access
  std::vector<std::vector<int>> child_Indexes =
      this->tree_Node->node_Split(position, split_Criteria);

  float base_Population = this->tree_Node->get_Index().size();

  // Creating a left child
  TreeNode left_Child{this->tree_Node->get_DataSet(), child_Indexes[0]};

  // Creating a right child
  TreeNode right_Child{this->tree_Node->get_DataSet(), child_Indexes[1]};

  // Get the labels
  std::vector<float> labels = this->tree_Node->get_DataSet()->get_Column(
      tree_Node->get_DataSet()->features_Length() - 1,
      this->tree_Node->get_Index());

  // Computes the Mean Absolute Error for left child
  float left_Prediction = left_Child.compute_Predicted_Value();
  float left_RMSE = 0;
  for (int idx : child_Indexes[0]) {
    left_RMSE += pow((abs(labels[idx] - left_Prediction)), 2);
  }

  // Computes the Mean Absolute Error for left child
  float right_Prediction = right_Child.compute_Predicted_Value();
  float right_RMSE = 0;
  for (int idx : child_Indexes[1]) {
    right_RMSE += pow((abs(labels[idx] - right_Prediction)), 2);
  }

  // Compute the result of MAE for the split at position
  float res = (left_RMSE + right_RMSE) / base_Population;
  res = sqrt(res);
  return res;
}

/*
Search for the best feature to split the dataset on at a given Node
Inputs :
Ouputs : int
*/
int RMSE::find_Best_Split_Feature() {
  int best_Feature = 0;
  // We try to minimize the mean absolute error for a split
  float min_RMSE = INT_MAX;

  std::vector<std::string> features =
      this->tree_Node->get_DataSet()->get_Features();

  //-1 here on feature size because we dont want to fit on the labels
  for (unsigned long int i = 0; i < features.size() - 1; ++i) {
    float tmp_var = splitting_MAE(i);
    if (tmp_var < min_RMSE) {
      min_RMSE = tmp_var;
      best_Feature = i;
    }
  }
  this->set_Split_Criteria(this->tree_Node->node_Column_Mean(best_Feature));

  return best_Feature;
}