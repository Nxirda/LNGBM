#ifndef TRAINING_ELEMENT_H_
#define TRAINING_ELEMENT_H_

#include <optional>

#include "ICriteria.hpp"
#include "IOperator.hpp"
#include "TreeNode.hpp"

struct TrainingElement {

private:
  // Parameters

  uint16_t depth;
  TreeNode *node;
  std::vector<size_t> index;

public:
  // Constructor

  TrainingElement();
  TrainingElement(TreeNode *node, const std::vector<size_t> &index,
                  uint16_t depth);
  TrainingElement(const TrainingElement &TE);

  // Operator Overloading

  TrainingElement &operator=(const TrainingElement &TE);

  // Destructor

  ~TrainingElement();

  // Getters

  const std::vector<size_t> &get_Index() const;

  // Setters
  void set_depth(uint16_t depth);
  void set_Node(TreeNode *node);
  void set_Index(const std::vector<size_t> &index);

  // Methods

  static void train(const DataSet &data, TreeNode *node,
                    const IOperator *splitting_Operator,
                    const ICriteria *splitting_Criteria, uint16_t max_Depth,
                    size_t treshold);

  /*
   */
  static std::tuple<std::optional<std::vector<size_t>>,
                    std::optional<std::vector<size_t>>>
  split_Index(const std::vector<double> &column,
              const std::vector<size_t> &index, double criterion);

private:
  void set_Root(size_t dataset_Size, TreeNode *node);
  void bootstrap_Index(size_t dataset_Size);

  /*
   */
  std::tuple<std::optional<TrainingElement>, std::optional<TrainingElement>>
  split_Node(const DataSet &data, const IOperator *splitting_Operator,
             const ICriteria *splitting_Criteria);

  /*
   */
  std::tuple<size_t, double>
  find_Best_Split_Parallel_2(const DataSet &data,
                             const IOperator *splitting_Operator,
                             const ICriteria *splitting_Criteria) const;
  /*
   */
  double compute_Split_Value(const std::vector<size_t> &index,
                             const DataSet &data, size_t feature,
                             double criteria, const IOperator *op) const;

  /*
   */
  std::tuple<std::optional<std::vector<double>>,
             std::optional<std::vector<double>>>
  split_Labels(const std::vector<double> &column,
               const std::vector<double> &labels, double criterion,
               const std::vector<size_t> &idx) const;
  /*
   */
  double mean_Vector_At_Index(const std::vector<double> &vector,
                              const std::vector<size_t> &index) const;
};

#endif