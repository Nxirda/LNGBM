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
TreeNode::TreeNode(std::shared_ptr<DataSet> data, std::vector<int> &idx) {
  this->data = data;
  this->index = std::move(idx);
}

/* Override "=" operator                 */
/* Inputs  : Object of TreeNode Class    */
/* Outputs : Object of TreeNode Class    */
TreeNode &TreeNode::operator=(TreeNode const &tn) {
  data = tn.data;
  index = tn.index;
  return *this;
}

/* Returns the Node's DataSet        */
/* Inputs  :                         */
/* Outputs : Object of DataSet Class */
DataSet &TreeNode::get_DataSet() { return *this->data; }

/* Returns the Variance of the Node's DataSet */
/* Inputs  :                                  */
/* Outputs : float                            */
float TreeNode::node_Variance() {
  return this->get_DataSet().global_Variance();
}

/* Return the Homogeneity as a boolean by comparing the variance */
/* for every Column of the given Tree Node DataSet               */
/* Inputs  :                                                     */
/* Outputs : bool                                                */
bool TreeNode::node_Homogeneity() {
int len = this->get_DataSet().features_Length();
float var = this->get_DataSet().column_Variance(0);
for (int i = 1; i < len; ++i) {
  float tmp = this->get_DataSet().column_Variance(i);
  //  If column variance is not uniform then return false
  if (tmp != var) {
    return false;
  }
}
return true;
}