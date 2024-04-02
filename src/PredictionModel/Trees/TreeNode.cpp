#include <TreeNode.hpp>

/********************/
/*                  */
/*    NODE CLASS    */
/*                  */
/********************/

//
TreeNode::TreeNode() {
  this->split_Column = 0;
  this->split_Criterion = -1;
  this->predicted_Value = -1;
  this->left = nullptr;
  this->right = nullptr;
}

//
TreeNode::TreeNode(size_t split_Column, double split_Criterion,
                   double predicted_Value) {
  this->split_Column = split_Column;
  this->split_Criterion = split_Criterion;
  this->predicted_Value = predicted_Value;
}

//
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

//
TreeNode::~TreeNode() {}

//
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

//
void TreeNode::set_Split_Column(size_t col) { this->split_Column = col; }

//
void TreeNode::set_Split_Criterion(double criterion) {
  this->split_Criterion = criterion;
}

//
void TreeNode::set_Predicted_Value(double value) {
  this->predicted_Value = value;
}

//
void TreeNode::add_Left(std::unique_ptr<TreeNode> Node) {
  this->left = std::move(Node);
}

//
void TreeNode::add_Right(std::unique_ptr<TreeNode> Node) {
  this->right = std::move(Node);
}

//
double TreeNode::get_Split_Criterion() const { return this->split_Criterion; }

//
size_t TreeNode::get_Split_Column() const { return this->split_Column; }

//
double TreeNode::get_Predicted_Value() const { return this->predicted_Value; }

//
TreeNode *TreeNode::get_Left_Node() const { return this->left.get(); }

//
TreeNode *TreeNode::get_Right_Node() const { return this->right.get(); }

//
/* void TreeNode::node_Print() {
  std::cout << "-> Split Column is : " << this->split_Column << "\n";
  std::cout << "-> Split Criterion is : " << this->split_Criterion << "\n";
  std::cout << "-> Split Prediction is : " << this->predicted_Value << "\n";
  if (this->left) {
    this->left->node_Print();
  }
  if (this->right) {
    this->right->node_Print();
  }
} */