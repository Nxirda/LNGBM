#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

#include <memory>
#include <string>
#include <vector>

#include "IModel.hpp"
#include "TreeNode.hpp"

// Binary Search Tree Class
class DecisionTree : IModel {
private:
  // Parameters
  uint16_t max_Depth;
  std::unique_ptr<TreeNode> root;

  // Boost part to serialize the trees so we can send them to other MPIs Process
  // Might need to build some sort of verif for MPI/Boost install at some point
  // so we can use it with or without them
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    // Unused param but necessary for boost
    static_cast<void>(version);
    ar &root;
  }

public:
  // Constructor

  DecisionTree() noexcept;
  DecisionTree(uint16_t max_Depth) noexcept;
  DecisionTree(const DecisionTree &dt) noexcept;
  DecisionTree &operator=(const DecisionTree &tree) noexcept;

  DecisionTree(DecisionTree &&tree) noexcept;
  DecisionTree &operator=(DecisionTree &&tree) noexcept;

  // Destructor

  ~DecisionTree();

  // Getters

  TreeNode *get_Root() const;

  // Setters
  void set_Root(std::unique_ptr<TreeNode> node);

  // Methods

  void train(const DataSet &data, ICriteria *crit, IOperator *op) override;
  std::vector<double> predict(const DataSet &data) const override;
};

#endif