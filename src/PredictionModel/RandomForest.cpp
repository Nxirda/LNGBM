#include <algorithm>
#include <execution>
#include <stack>

#include "RandomForest.hpp"

/**************************/
/*                        */
/*  RANDOM FOREST CLASS   */
/*                        */
/**************************/

/*
Default Constructor : Initialize empty Forest
Parameters :
Inputs     :
Outputs    : Object of RandomForest class
*/
RandomForest::RandomForest() {
  this->size = 0;
  this->max_Depth = 0;
  this->trees = std::map<int, DecisionTree>();
}

/*
Constructor with arguments
Parameters : Dataset, splitting operator, splitting criteria, forest size, depth
for the trees Inputs     : const DataSet, IOperator*, ICriteria*, int, int
Outputs    : Object of RandomForest class
*/
RandomForest::RandomForest(IOperator *split_Operator, ICriteria *split_Criteria,
                           int n, int depth) {
  this->size = n;
  this->max_Depth = depth;
  this->splitting_Operator = split_Operator;
  this->splitting_Criteria = split_Criteria;
  this->trees = std::map<int, DecisionTree>();
}

/*
Default Destructor
Parameters :
Inputs     :
Outputs    :
*/
RandomForest::~RandomForest(){};

/*
Returns the size of the the forest
Parameters :
Inputs     :
Outputs    : int
*/
int RandomForest::get_size() { return this->size; }

//
const std::map<int, DecisionTree> &RandomForest::get_Trees() {
  return this->trees;
}

/*
 */
void RandomForest::aggregate_Trees(const std::map<int, DecisionTree> &forest) {
  int n = this->trees.size();
  this->size += forest.size();
  for (auto &pair : forest)
    this->trees.insert({pair.first + n, pair.second});
}

/*
Generate the forest by training a specified number of trees
Parameters : Size (number of Trees)
Inputs     : int
Outputs    :
*/
void RandomForest::generate_Forest(const DataSet &data, int size) {
  long unsigned int treshold = 5;

  for (int i = 0; i < size; ++i) {

    DecisionTree tree{};
    TrainingElement elem{};

    elem.set_Node(tree.get_Root());
    elem.train(data, this->splitting_Operator, this->splitting_Criteria,
               this->max_Depth, treshold);

    tree.set_Root(std::make_unique<TreeNode>(*elem.node));
    this->trees.insert({i, tree});
  }
}

/*
Returns the predictions for the current dataset
Parameters : Dataset to predict on
Inputs     : const DataSet
Outputs    : vector<double>
*/
std::vector<double> RandomForest::predict_Results(const DataSet &data) {
  int size = data.samples_Number();
  std::vector<double> result(size, 0);

  // Computes the index
  std::vector<int> index(size);
  for (int i = 0; i < size; ++i) {
    index[i] = i;
  }

  std::vector<double> tree_Result(size, 0);
  // Iterate through the Forest
  for (size_t i = 0; i < this->trees.size(); ++i) {

    // Computes the prediction for the current tree
    if (this->trees.find(i) == this->trees.end()) {
      // perror("Couldnt find wanted tree");
      std::cerr << "Couldn't find wanted tree \n";
      exit(1);
    }

    tree_Prediction(data, tree_Result, index, this->trees[i].get_Root());

    // Adds two vectors
    std::transform(std::execution::unseq, result.begin(), result.end(),
                   tree_Result.begin(), result.begin(), std::plus<double>());
  }

  // Divides to have the mean of the answers
#pragma omp parallel for
  for (int j = 0; j < size; ++j) {
    result.at(j) *= (1.0 / this->trees.size());
  }

  return result;
}

/*
Predict the result for the current tree
Parameters : Dataset, results, index for the current node, current node
Inputs     : const DataSet, shared_ptr<vector<double>>, vector<int>, TreeNode*
Outputs    :
*/
void RandomForest::tree_Prediction(const DataSet &data,
                                   std::vector<double> &result,
                                   const std::vector<int> &index,
                                   TreeNode *node) {

  // Update the values of the result
  double pred_Val = node->get_Predicted_Value();

#pragma omp parallel for
  for (int idx : index) {
    result[idx] = pred_Val;
  }

  // Put the correct indexes
  auto [left_Index, right_Index] =
      data.split(node->get_Split_Column(), node->get_Split_Criterion(), index);

  if (node->get_Left_Node() && left_Index) {
    tree_Prediction(data, result, *left_Index, node->get_Left_Node());
  }

  if (node->get_Right_Node() && right_Index) {
    tree_Prediction(data, result, *right_Index, node->get_Right_Node());
  }
}