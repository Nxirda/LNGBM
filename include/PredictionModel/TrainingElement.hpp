#ifndef TRAINING_ELEMENT_H_
#define TRAINING_ELEMENT_H_

#include <optional>

#include "TreeNode.hpp"
#include "IOperator.hpp"

struct TrainingElement {

  // Parameters

  TreeNode *node;
  std::vector<int> index;
  int depth;

  // Constructor

  TrainingElement();
  TrainingElement(TreeNode *node, std::vector<int> index, int depth);

  // Destructor

  ~TrainingElement();

  // Getters

  std::vector<int> get_Index();

  // Setters

  void set_Node(TreeNode *node);
  void set_Index(std::vector<int> index);
  void set_depth(int depth);

  // Methods

  std::tuple<std::optional<TrainingElement>, std::optional<TrainingElement>>
  split_Node(const DataSet &data, TrainingElement *elem,
             const IOperator *splitting_Operator,int max_Depth);

  std::tuple<int, float> find_Best_Split(const DataSet &data,
                                         TrainingElement *elem,
                                         const IOperator *splitting_Operator);

  std::tuple<std::vector<int>, std::vector<int>>
  split_Index(const DataSet &data, int criterion, int position,
              TrainingElement *elem);

  void train(const DataSet &data, IOperator *splitting_Operator,
                  int max_Depth);

  void set_Root(int dataset_Size, TreeNode *node);
};

#endif