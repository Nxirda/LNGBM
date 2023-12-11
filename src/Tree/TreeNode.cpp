#include <TreeNode.hpp>

/********************/
/*                  */
/*    NODE CLASS    */
/*                  */
/********************/

/*
Default Constructor
Inputs  :
Outputs : Object of TreeNode Class
*/
TreeNode::TreeNode() {}

/*
Default Destructor
Inputs  :
Outputs :
*/
TreeNode::~TreeNode() {}

/*
Constructor with DataSet and vector index:
Inputs  : Object of DataSet Class, vector
Outputs : TreeNode Object containing the DataSet
*/
TreeNode::TreeNode(std::shared_ptr<DataSet> data) {
  std::vector<int> idx(data->samples_Number());
  for (int i = 0; i < data->samples_Number(); ++i) {
    idx[i] = i;
  }
  this->data = data;
  this->index = idx;
}

/*
Constructor with DataSet and vector index:
Inputs  : Object of DataSet Class, vector
Outputs : TreeNode Object containing the DataSet
*/
TreeNode::TreeNode(std::shared_ptr<DataSet> data, std::vector<int> &idx) {
  this->data = data;
  this->index = idx;
}

/*
Override "=" operator
Inputs  : Object of TreeNode Class
Outputs : Object of TreeNode Class
*/
TreeNode &TreeNode::operator=(TreeNode const &tn) {
  data = tn.data;
  index = tn.index;
  return *this;
}

/*
Sets the column of the DataSet which this node should split
Inputs  : int
Outputs :
*/
void TreeNode::set_Split_Column(int col) { this->split_Column = col; }

/*
Sets the value in the split column that is used to split at
this Node
Inputs  : float
Outputs :
*/
void TreeNode::set_Split_Criteria(float criteria) {
  this->split_Criteria = criteria;
}

/*
Sets the value we predicted for this Node with the split
Inputs  : float
Outputs :
*/
void TreeNode::set_Predicted_Value(float value) {
  this->predicted_Value = value;
}

/*
Sets the Node's Index vector
Inputs  :  vector<int>
Outputs :
*/
void TreeNode::set_Index(std::vector<int> idx) { this->index = idx; }

/*
Sets the DataSet we are observing at this Node
Inputs  : shared_ptr<DataSet>
Outputs :
*/
void TreeNode::set_DataSet(std::shared_ptr<DataSet> data) {
  std::vector<int> idx(data->samples_Number());
  for (int i = 0; i < data->samples_Number(); ++i) {
    idx[i] = i;
  }
  this->data = data;
  this->index = idx;
}

/*
Returns the Node's DataSet
Inputs  :
Outputs : Object of DataSet Class
*/
std::shared_ptr<DataSet> TreeNode::get_DataSet() { return this->data; }

/*
Returns the Node's index of his elements
Inputs  :
Outputs : Object of DataSet Class
*/
std::vector<int> TreeNode::get_Index() { return this->index; }

/*
Get the split criteria to split on at this node
Inputs  :
Outputs : float
*/
float TreeNode::get_Split_Criteria() { return this->split_Criteria; }

/*
Get the column to split on at this node
Inputs  :
Outputs : int
*/
int TreeNode::get_Split_Column() { return this->split_Column; }

/*
Get the value we expect of the split at this node
Inputs  : 
Outputs : float
*/
float TreeNode::get_Predicted_Value() { return this->predicted_Value; }

/*
Returns the Variance of the Node's DataSet
Inputs  : int
Outputs : float
*/
float TreeNode::node_Variance() {
  return this->data->column_Variance(this->get_Index());
}

/*
Returns the Mean of the Node's DataSet
Inputs  : int
Outputs : float
*/
float TreeNode::node_Column_Mean(int position) {
  return this->data->column_Mean(position, this->get_Index());
}

/*
Return the Homogeneity as a boolean by comparing the  values
of each labels of the Node
Inputs  :
Outputs : bool
*/
bool TreeNode::node_Homogeneity() {
  if (this->get_Index().size() <= 0) {
    return false;
  }

  std::vector<float> labels = this->data->get_Labels(this->get_Index());

  int base = labels[0];
  for (float curr_Label : labels) {
    if (curr_Label != base) {
      return false;
    }
  }
  return true;
}

/*
Returns the indexes after the split of the Node's DataSet
Inputs  : int, float
Outputs : vector<vector<int>>
*/
std::vector<std::vector<int>> TreeNode::node_Split(int position,
                                                   float split_Criteria) {
  if (position < this->data->features_Length() && split_Criteria > 0) {
    return this->data->split(position, split_Criteria, this->get_Index());
  } else {
    return this->data->split(0, 0, this->get_Index());
  }
}

/*
Computes the value we expect at this node in the split
Inputs  : 
Outputs : float
*/
float TreeNode::compute_Predicted_Value() {
  if (this->get_Index().size() == 0) {
    return -1;
  }
  return this->get_DataSet()->labels_Mean(this->get_Index());
}

/**/
std::vector<int> TreeNode::bootstrap_DataSet() {
  int len = this->get_Index().size();
  std::vector<int> bootstrap_Index(len);
  for(int i = 0; i < len; i++){
    bootstrap_Index[i] = rand() % len;
  }
  return bootstrap_Index;
}

/*
Prints the Node
Inputs :
Outputs :
*/
void TreeNode::node_Print() { this->data->print_With_Index(this->get_Index()); }

/*
Print for debugging mainly
Inputs  :
Outputs :
*/
void TreeNode::node_Print_Criteria() {
  //std::cout << "-> Split Column is : " << this->split_Column << "\n";
  //std::cout << "-> Split Criteria is : " << this->split_Criteria << "\n";
  std::cout << "-> Split Prediction is : " << this->predicted_Value << "\n";
}