#include <TreeNode.hpp>

/********************/
/*                  */
/*    NODE PART     */
/*                  */
/********************/

/* Default Constructor                */
/* Inputs  :                          */
/* Outputs : Object of TreeNode Class */
TreeNode::TreeNode() {}

/* Default Destructor          */
/* Inputs  :                   */
/* Outputs :                   */
TreeNode::~TreeNode() {}

/* Constructor with DataSet and vector index:       */
/* Inputs  : Object of DataSet Class, vector        */
/* Outputs : TreeNode Object containing the DataSet */
TreeNode::TreeNode(std::shared_ptr<DataSet> data) {
  std::vector<int> idx(data->samples_Number());
  for (int i = 0; i < data->samples_Number(); ++i) {
    idx[i] = i;
  }
  this->data = data;
  this->index = idx;
}

/* Constructor with DataSet and vector index:       */
/* Inputs  : Object of DataSet Class, vector        */
/* Outputs : TreeNode Object containing the DataSet */
TreeNode::TreeNode(std::shared_ptr<DataSet> data, std::vector<int> &idx) {
  this->data = data;
  this->index = idx;
}

/* Override "=" operator                 */
/* Inputs  : Object of TreeNode Class    */
/* Outputs : Object of TreeNode Class    */
TreeNode &TreeNode::operator=(TreeNode const &tn) {
  data = tn.data;
  index = tn.index;
  return *this;
}

/* Sets the Node's Index vector      */
/* Inputs  :  vector<int>            */
/* Outputs :                         */
void TreeNode::set_Index(std::vector<int> idx) { this->index = std::move(idx); }

/* Returns the Node's DataSet        */
/* Inputs  :                         */
/* Outputs : Object of DataSet Class */
DataSet &TreeNode::get_DataSet() { return *this->data; }

/* Returns the Node's index of his elements */
/* Inputs  :                                */
/* Outputs : Object of DataSet Class        */
std::vector<int> TreeNode::get_Index() { return this->index; }

/* Returns the Variance of the Node's DataSet */
/* Inputs  : int                              */
/* Outputs : float                            */
float TreeNode::node_Variance() {
  return this->data->global_Variance(this->get_Index());
}

/* Returns the Mean of the Node's DataSet */
/* Inputs  : int                          */
/* Outputs : float                        */
float TreeNode::node_Column_Mean(int position) {
  return this->data->column_Mean(position, this->get_Index());
}

/* Return the Homogeneity as a boolean by comparing the  values */
/* of each labels of the Node                                   */
/* Inputs  :                                                    */
/* Outputs : bool                                               */
bool TreeNode::node_Homogeneity() {
  std::vector<float> labels = this->data->get_Column(this->data->features_Length());
  int base = labels[0];
  for(float curr_Label : labels){
    if(curr_Label != base){
      return false;
    }
  }
  return true;
}

/* Returns the indexes after the split of the Node's DataSet */
/* Inputs  : int, float                                      */
/* Outputs : vector<vector<int>>                             */
std::vector<std::vector<int>> TreeNode::node_Split(int position,
                                                   float split_Criteria) {
  return this->data->split(position, split_Criteria, this->get_Index());
}

/* Prints the Node */
/* Inputs :        */
/* Outputs :       */
void TreeNode::node_Print() { this->data->print_With_Index(this->get_Index()); }