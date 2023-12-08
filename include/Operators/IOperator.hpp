#ifndef I_OPERATOR_H_
#define I_OPERATOR_H_

#include <memory>
#include <string>

class TreeNode;

// Interface to build Operators on
class IOperator {

private:
  const std::string name;
  std::shared_ptr<TreeNode> tree_Node;
  float split_Criteria;

public:
  virtual ~IOperator(){};
  virtual bool set_Node(std::shared_ptr<TreeNode> tree_Node) {
    if (tree_Node)
      return true;
    return false;
  };

  virtual int find_Best_Split_Feature() { return 0; };
  virtual float get_Best_Split_Criteria() { return 0.0; };
  virtual void set_Split_Criteria(float value) { split_Criteria = value; }

  virtual void print(){};
};

#endif