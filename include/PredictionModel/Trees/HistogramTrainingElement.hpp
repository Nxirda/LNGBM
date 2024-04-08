#ifndef HITOGRAM_TRAINING_ELEMENT_H_
#define HITOGRAM_TRAINING_ELEMENT_H_

#include <optional>
#include <unordered_map>

#include "Histogram2.hpp"
#include "IOperator.hpp"
#include "TreeNode.hpp"

struct HistogramTrainingElement {

private:
  // Parameters
  TreeNode *node;
  // std::vector<size_t> index;
  // Might need to be re factored to an independnt class
  std::vector<double> labels;
  std::unordered_map<uint16_t, Histogram2> Histograms;
  uint16_t depth;

public:
  // Constructor
  HistogramTrainingElement();

  HistogramTrainingElement(TreeNode *node, uint16_t depth);

  HistogramTrainingElement(const HistogramTrainingElement &TE);
  HistogramTrainingElement(HistogramTrainingElement &&TE);

  // Operator Overloading
  HistogramTrainingElement &operator=(const HistogramTrainingElement &TE);
  HistogramTrainingElement &operator=(HistogramTrainingElement &&TE);

  // Destructor

  ~HistogramTrainingElement();

  // Methods
  static void train(const DataSet &data, TreeNode *node,
                    const IOperator *splitting_Operator,
                    /*const ICriteria *splitting_Criteria,*/ uint16_t max_Depth,
                    size_t treshold);

  /*
   */
  static std::tuple<std::optional<std::vector<size_t>>,
                    std::optional<std::vector<size_t>>>
  split_Index(const std::vector<double> &column,
              const std::vector<size_t> &index, double criterion);

private:
  // Getters

  // Setters
  void set_depth(uint16_t depth);
  void set_Node(TreeNode *node);
  // void set_Index(const std::vector<size_t> &index);
  void set_Root(const DataSet &data, TreeNode *node, const ICriteria *criteria);
  std::vector<size_t> bootstrap_Index(size_t dataset_Size);

  /*
   */
  std::tuple<std::optional<HistogramTrainingElement>,
             std::optional<HistogramTrainingElement>>
  split_Node(const DataSet &data, const IOperator *splitting_Operator
             /*,const ICriteria *splitting_Criteria*/);

  /*
   */
  std::tuple<std::optional<std::vector<double>>,
             std::optional<std::vector<double>>>
  split_Bin(size_t feature, double criterion);

  /*
   */
  std::tuple<size_t, double>
  find_Best_Split(/* const DataSet &data, */ const IOperator *splitting_Operator
                  /*,const ICriteria *splitting_Criteria*/) const;
  /*
   */
  double compute_Split_Value(const std::vector<size_t> &index,
                             const DataSet &data, size_t feature,
                             /*double criteria,*/ const IOperator *op) const;
};

#endif