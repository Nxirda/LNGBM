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
  std::vector<size_t> index;
  uint16_t depth;

public:
  // Constructor
  HistogramTrainingElement();

  HistogramTrainingElement(TreeNode *node, const std::vector<size_t> &index,
                           uint16_t depth);

  HistogramTrainingElement(const HistogramTrainingElement &TE);
  HistogramTrainingElement(HistogramTrainingElement &&TE);

  // Operator Overloading
  HistogramTrainingElement &operator=(const HistogramTrainingElement &TE);
  HistogramTrainingElement &operator=(HistogramTrainingElement &&TE);

  // Destructor

  ~HistogramTrainingElement();

  // Methods
  static void train(const DataSet &data, TreeNode *node, uint16_t max_Depth);

private:
  // Getters

  // Setters
  void set_depth(uint16_t depth);
  void set_Node(TreeNode *node);
  void set_Root(TreeNode *node);

  void set_Histogram(size_t feature, Histogram2 &&histogram);
  void set_Index(std::vector<size_t> &&new_Index);

  // void init_Histograms(const DataSet &data);
  void init_Histograms(const DataSet &data, const std::vector<size_t> &index);
  void fill_Histograms(const DataSet &data, const std::vector<size_t> &index);

  std::vector<size_t> bootstrap(size_t dataset_Size);

  /*
   */
  std::tuple<std::optional<HistogramTrainingElement>,
             std::optional<HistogramTrainingElement>>
  split_Node(const DataSet &data);

  /*
   */
  std::tuple<std::optional<Histogram2>, std::optional<Histogram2>>
  split_Histogram(size_t bin, size_t feature) const;

  /*
   */
  std::tuple<std::optional<std::vector<size_t>>,
             std::optional<std::vector<size_t>>>
  split_Index(const DataSet &data, size_t bin, size_t feature) const;

  /*
   */
  std::tuple<double, size_t, size_t> best_Histogram_Split(size_t feature) const;

  /*
   */
  std::tuple<size_t, size_t> find_Best_Split() const;

  /*
   */
  double compute_Split_Value(size_t bin_Index, size_t feature) const;

  /*
   */
  double compute_Predicted_Value(const DataSet &data) const;
};

#endif