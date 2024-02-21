#ifndef TRAINING_ELEMENT_H_
#define TRAINING_ELEMENT_H_

#include <optional>

#include "ICriteria.hpp"
#include "IOperator.hpp"
#include "TreeNode.hpp"

struct TrainingElement {

  // Parameters

  TreeNode *node;
  std::vector<int> index;
  int depth;

  // Constructor

  TrainingElement();
  TrainingElement(TreeNode *node, const std::vector<int> &index, int depth);
  TrainingElement(const TrainingElement &TE);

  // Operator Overloading

  TrainingElement &operator=(const TrainingElement &TE);

  // Destructor

  ~TrainingElement();

  // Getters

  const std::vector<int> &get_Index();

  // Setters
  void set_depth(int depth);
  void set_Node(TreeNode *node);
  void set_Index(const std::vector<int> &index);

  // Methods

  std::tuple<std::optional<TrainingElement>, std::optional<TrainingElement>>
  split_Node(const DataSet &data, const IOperator *splitting_Operator,
             const ICriteria *splitting_Criteria);

  std::tuple<int, double> find_Best_Split(const DataSet &data,

                                          const IOperator *splitting_Operator,
                                          const ICriteria *splitting_Criteria);

  std::tuple<std::optional<std::vector<int>>, std::optional<std::vector<int>>>
  split_Index(const DataSet &data, int criterion, int position);

  void train(const DataSet &data, const IOperator *splitting_Operator,
             const ICriteria *splitting_Criteria, int max_Depth,
             size_t treshold);

  void set_Root(int dataset_Size, TreeNode *node/*, double value*/);
  void bootstrap_Index(int dataset_Size);
};

#endif