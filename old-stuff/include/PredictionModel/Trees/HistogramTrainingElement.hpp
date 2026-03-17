#ifndef HITOGRAM_TRAINING_ELEMENT_H_
#define HITOGRAM_TRAINING_ELEMENT_H_

#include <optional>
#include <unordered_map>

#include "Histogram.hpp"
#include "TreeNode.hpp"

struct HistogramTrainingElement {

private:
  // Parameters
  TreeNode *node;

  std::unordered_map<uint16_t, Histogram> Histograms;
  std::vector<size_t> index;
  uint16_t depth;
  uint64_t bins;

public:
  // Constructor
  HistogramTrainingElement();

  HistogramTrainingElement(TreeNode *node, const std::vector<size_t> &index,
                           uint16_t depth, uint64_t bins);

  HistogramTrainingElement(const HistogramTrainingElement &TE);
  HistogramTrainingElement(HistogramTrainingElement &&TE);

  // Operator Overloading
  HistogramTrainingElement &operator=(const HistogramTrainingElement &TE);
  HistogramTrainingElement &operator=(HistogramTrainingElement &&TE);

  // Destructor

  ~HistogramTrainingElement();

  // Methods
  static void train(const DataSet &data, TreeNode *node, uint16_t max_Depth, size_t threshold, uint64_t bins);

private:
  // Getters

  // Setters
  void set_depth(uint16_t depth);
  void set_Node(TreeNode *node);
  void set_Root(TreeNode *node);
  void set_Bins(uint64_t bins);
  
  void set_Histogram(size_t feature, Histogram &&histogram);
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
  double compute_Residual(double actual, double predicted) const;

  /*
   */
  double mean_Vector_At_Index(const std::vector<double> &vector,
                              const std::vector<size_t> &index) const;

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
};

#endif