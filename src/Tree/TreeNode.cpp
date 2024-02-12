#include <TreeNode.hpp>

/********************/
/*                  */
/*    NODE CLASS    */
/*                  */
/********************/

/*
Default Constructor
Parameters :
Inputs     :
Outputs    : 
*/
TreeNode::TreeNode() {
  this->split_Column = -1;
  this->split_Criterion = -1;
  this->predicted_Value = -1;
  this->left = nullptr;
  this->right = nullptr;
}

/*
Constructor of Tree Node with parameters
Parameters : column, criterion, predicted value
Inputs     : int, floatt, float
Outputs    : 
 */
TreeNode::TreeNode(int split_Column, float split_Criterion,
                   float predicted_Value) {
  this->split_Column = split_Column;
  this->split_Criterion = split_Criterion;
  this->predicted_Value = predicted_Value;
}

/*
Constructor that copies a Node
Parameters : Tree Node
Inputs     : const TreeNode
Outputs    : 
*/
TreeNode::TreeNode(const TreeNode &node) {
  this->split_Column = node.split_Column;
  this->split_Criterion = node.split_Criterion;
  this->predicted_Value = node.predicted_Value;

  if (node.left) {
    this->left = std::make_unique<TreeNode>(*node.get_Left_Node());
  } else {
    this->left.reset();
  }

  if (node.right) {
    this->right = std::make_unique<TreeNode>(*node.get_Right_Node());
  } else {
    this->right.reset();
  }
}

/*
Default Destructor
Parameters :
Inputs     :
Outputs    :
*/
TreeNode::~TreeNode() {}

/*
Override "=" operator
Parameters : TreeNode
Inputs     : const TreeNode
Outputs    :
*/
TreeNode &TreeNode::operator=(const TreeNode &tn) {

  this->predicted_Value = tn.get_Predicted_Value();
  this->split_Column = tn.get_Split_Column();
  this->split_Criterion = tn.get_Split_Criterion();

  if (tn.left) {
    this->left = std::make_unique<TreeNode>(*tn.get_Left_Node());
  } else {
    this->left.reset();
  }

  if (tn.right) {
    this->right = std::make_unique<TreeNode>(*tn.get_Right_Node());
  } else {
    this->right.reset();
  }

  return *this;
} 

/*
Sets the column of the DataSet which this node should split
Parameters : column
Inputs     : int
Outputs    :
*/
void TreeNode::set_Split_Column(int col) { this->split_Column = col; }

/*
Sets the value in the split column that is used to split at
this Node
Parameters : criterion
Inputs     : float
Outputs    :
*/
void TreeNode::set_Split_Criterion(float criterion) {
  this->split_Criterion = criterion;
}

/*
Sets the value we predicted for this Node with the split
Parameters : predicted value
Inputs     : float
Outputs    :
*/
void TreeNode::set_Predicted_Value(float value) {
  this->predicted_Value = value;
}

/*
Adds a left node to the left pointer
Parameters : Tree Node
Inputs     : unique_ptr<TreeNode>
Outputs    :
*/
void TreeNode::add_Left(std::unique_ptr<TreeNode> Node) {
  this->left = std::move(Node);
}

/*
Adds a right node to the right pointer
Parameters : Tree Node
Inputs     : unique_ptr<TreeNode>
Outputs    :
*/
void TreeNode::add_Right(std::unique_ptr<TreeNode> Node) {
  this->right = std::move(Node);
}

/*
Get the split criteria to split on at this node
Parameters : 
Inputs     :
Outputs    : float
*/
float TreeNode::get_Split_Criterion() const { return this->split_Criterion; }

/*
Get the column to split on at this node
Parameters : Tree Node
Inputs     :
Outputs    : int
*/
int TreeNode::get_Split_Column() const { return this->split_Column; }

/*
Get the value we expect of the split at this node
Parameters : 
Inputs     :
Outputs    : float
*/
float TreeNode::get_Predicted_Value() const { return this->predicted_Value; }

/*
Returns a pointer to the left node of the current node
Parameters : 
Inputs     :
Outputs    : TreeNode*
*/
TreeNode *TreeNode::get_Left_Node() const { return this->left.get(); }

/*
Returns a pointer to the right node of the current node
Parameters : 
Inputs     :
Outputs    : TreeNode
*/
TreeNode *TreeNode::get_Right_Node() const { return this->right.get(); }

/*
Print for debugging mainly
Parameters : 
Inputs     :
Outputs    :
*/
void TreeNode::node_Print_Criterion() {
  // std::cout << "-> Split Column is : " << this->split_Column << "\n";
  // std::cout << "-> Split Criterion is : " << this->split_Criterion << "\n";
  std::cout << "-> Split Prediction is : " << this->predicted_Value << "\n";
  if (this->left) {
    this->left->node_Print_Criterion();
  }
  if (this->right) {
    this->right->node_Print_Criterion();
  }
}