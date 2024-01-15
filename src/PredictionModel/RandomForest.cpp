#include <algorithm>
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
Outputs    :
*/
RandomForest::RandomForest() {
  this->size = 0;
  this->max_Depth = 0;
  this->dataset = DataSet{};
  this->trees = std::map<int, DecisionTree>();
}

/*
Constructor with arguments
Parameters : Dataset, splitting operator, forest size, depth for the trees
Inputs     : const DataSet, IOperator*, int, int
Outputs    :
*/
RandomForest::RandomForest(const DataSet &dataset, IOperator *split_Operator,
                           int n, int depth) {
  this->size = n;
  this->max_Depth = depth;
  this->dataset = dataset;
  this->splitting_Operator = split_Operator;
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

/*
Generate the forest by training a specified number of trees
Parameters : Size (number of Trees)
Inputs     : int
Outputs    :
*/
void RandomForest::generate_Forest(int size) {
  for (int i = 0; i < size; ++i) {
    DecisionTree tree{};

    TrainingElement elem{};
    elem.set_Node(tree.get_Root());
    elem.train(this->dataset, this->splitting_Operator, this->max_Depth);

    tree.set_Root(std::make_unique<TreeNode>(*elem.node));
    this->trees.insert({i, tree});
  }
}

/*
Returns the predictions for the current dataset
Parameters : Dataset to predict on
Inputs     : const DataSet
Outputs    : vector<float>
*/
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
        std::make_shared<std::vector<float>>(size, 0);

    // Computes the prediction for the current tree
    if (this->trees.find(i) == this->trees.end()) {
      perror("Couldnt find wanted tree");
    }

    tree_Prediction(data, tree_Result, index, this->trees[i].get_Root());

    // Adds two vectors
    std::transform(result.begin(), result.end(), tree_Result->begin(),
                   result.begin(), std::plus<float>());
  }

  // Divides to have the mean of the answers
  for (int j = 0; j < size; ++j) {
    result.at(j) /= this->trees.size();
  }

  return result;
}

/*
Predict the result for the current tree
Parameters : Dataset, results, index for the current node, current node
Inputs     : const DataSet, shared_ptr<vector<float>>, vector<int>, TreeNode*
Outputs    :
*/
void RandomForest::tree_Prediction(const DataSet &data,
                                   std::shared_ptr<std::vector<float>> result,
                                   std::vector<int> index, TreeNode *node) {

  // Update the values of the result
  for (int idx : index) {
    result->at(idx) = node->get_Predicted_Value();
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