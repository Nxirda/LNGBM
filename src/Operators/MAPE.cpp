#include <cmath>

#include "MAPE.hpp"
#include "TreeNode.hpp"

/*
Constructor
Input  : DecisionTree*
Output :
*/
MAPE::MAPE(std::shared_ptr<TreeNode> tree_Node) { this->tree_Node = tree_Node; }

/*
Setter for the tree pointer
Input  : DecisionTree*
Output : bool
*/
bool MAPE::set_Node(std::shared_ptr<TreeNode> tree_Node) {
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
MAPE::~MAPE() {}

/*
Print function to see the name of the operator
(For debugging mainly)
Input  :
Output :
*/
void MAPE::print() {
  std::cout << "=== Operator is : " << this->name << " ===\n";
}

/*
Returns the best splitting criteria for RIV algorithm
Input  :
Output : float
*/
float MAPE::get_Best_Split_Criteria() { return this->split_Criteria; }

/*
Sets the split criteria as the value given
Input  : float
Output :
*/
void MAPE::set_Split_Criteria(float value) { this->split_Criteria = value; }

/**/
float MAPE::splitting_MAE(int position) {

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
  std::vector<float> labels =
      this->tree_Node->get_DataSet()->get_Labels(this->tree_Node->get_Index());
      
  int size = (int)labels.size();

  // Computes the Mean Absolute Error for left child
  float left_Prediction = left_Child.compute_Predicted_Value();
  float left_MAPE = 0;
  for (int idx : child_Indexes[0]) {
    if(idx < size)
      left_MAPE += (abs(labels[idx] - left_Prediction)) / left_Prediction;
  }

  // Computes the Mean Absolute Error for left child
  float right_Prediction = right_Child.compute_Predicted_Value();
  float right_MAPE = 0;
  for (int idx : child_Indexes[1]) {
    if(idx < size)
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
int MAPE::find_Best_Split_Feature() {
  int best_Feature = 0;
  // We try to minimize the mean absolute error for a split
  float min_MAPE = INT_MAX;

  std::vector<std::string> features =
      this->tree_Node->get_DataSet()->get_Features();

  for (unsigned long int i = 0; i < features.size() ; ++i) {
    float tmp_var = splitting_MAE(i);
    if (tmp_var < min_MAPE) {
      min_MAPE = tmp_var;
      best_Feature = i;
    }
  }
  this->set_Split_Criteria(this->tree_Node->node_Column_Mean(best_Feature));

  return best_Feature;
}