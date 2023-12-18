#include <algorithm>
#include <stack>

#include "RandomForest.hpp"

/**************************/
/*                        */
/*  RANDOM FOREST CLASS   */
/*                        */
/**************************/

/**/
RandomForest::RandomForest() {
  this->size = 0;
  this->max_Depth = 0;
  this->dataset = DataSet{};
  this->trees = std::map<int, DecisionTree>();
}

/**/
RandomForest::RandomForest(const DataSet &dataset, IOperator *split_Operator,
                           int n, int depth) {
  this->size = n;
  this->max_Depth = depth;
  this->dataset = dataset;
  this->splitting_Operator = split_Operator;
  this->trees = std::map<int, DecisionTree>();
}

/**/
RandomForest::~RandomForest(){};

/**/
int RandomForest::get_size() { return this->size; }

/**/
void RandomForest::generate_Forest(int size) {

  for (int i = 0; i < size; ++i) {
    DecisionTree tree{};

    TrainingElement elem{};
    elem.set_Node(tree.get_Root());
    elem.train(this->dataset, this->splitting_Operator, this->max_Depth);

    tree.set_Root(std::make_unique<TreeNode>(*elem.node));
    this->trees[i] = tree;
  }
}

/**/
std::vector<float> RandomForest::predict_Results(const DataSet &data) {
  int size = data.samples_Number();
  std::vector<float> result(size, 0);

  // Computes the index
  std::vector<int> index(size);
  for (int i = 0; i < size; ++i) {
    index[i] = i;
  }

  // Iterate through the Forest
  for (unsigned long int i = 0; i < this->trees.size(); ++i) {
    std::shared_ptr<std::vector<float>> tree_Result =
        std::make_shared<std::vector<float>>(size);

    // Computes the prediction for the current tree
    tree_Prediction(data, tree_Result, index, this->trees[i].get_Root());
    /* for (unsigned long int i = 0; i < tree_Result.get()->size(); ++i) {
      std::cout << "[" << tree_Result.get()->at(i) << "]";
    }
    std::cout << "\n"; */

    // Adds two vectors
    std::transform(result.begin(), result.end(), tree_Result->begin(),
                   result.begin(), std::plus<float>());
  }
  // Divides to have the mean of the answers
  for (unsigned long int j = 0; j < result.size(); ++j) {
    result.at(j) /= this->trees.size();
  }

  return result;
}

/**/
void RandomForest::tree_Prediction(const DataSet &data,
                                   std::shared_ptr<std::vector<float>> result,
                                   std::vector<int> index, TreeNode *node) {
  // Put the correct indexes
  auto [left_Index, right_Index] =
      data.split(node->get_Split_Column(), node->get_Split_Criterion(), index);

  for (auto idx : index) {
    result.get()->at(idx) = node->get_Predicted_Value();
  }

  if (node->get_Left_Node() && left_Index) {
    tree_Prediction(data, result, *left_Index, node->get_Left_Node());
  }

  if (node->get_Right_Node() && right_Index) {
    tree_Prediction(data, result, *right_Index, node->get_Right_Node());
  }
}