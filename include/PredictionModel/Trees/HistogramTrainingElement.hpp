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

  // std::vector<double> labels;
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
  void set_Root(/* const DataSet &data,  */ TreeNode *node);

  void init_Histograms(const DataSet &data);
  void fill_Histograms(const DataSet &data, const std::vector<size_t> &index);

  std::vector<size_t> bootstrap(size_t dataset_Size);

  /*
   */
  std::tuple<std::optional<HistogramTrainingElement>,
             std::optional<HistogramTrainingElement>>
  split_Node(const DataSet &data, const IOperator *splitting_Operator
             /*,const ICriteria *splitting_Criteria*/);

  /*
   */
  std::tuple<Histogram2, Histogram2> split_Histogram(size_t feature) const;

  /*
   */
  std::tuple<double, double, size_t> best_Histogram_Split(size_t feature) const;

  /*
   */
  std::tuple<size_t, double>
  find_Best_Split(/* const DataSet &data, */ const IOperator *splitting_Operator
                  /*,const ICriteria *splitting_Criteria*/) const;
  /*
   */
  double compute_Split_Value(size_t bin_Index, size_t feature,
                             const IOperator *op) const;
};

#endif