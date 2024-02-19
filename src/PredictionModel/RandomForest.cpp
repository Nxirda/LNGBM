#include <algorithm>
#include <execution>
#include <stack>

#include "RandomForest.hpp"
#include "TrainingElement.hpp"

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
  this->trees = std::unordered_map<int, DecisionTree>();
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
  this->trees = std::unordered_map<int, DecisionTree>();
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
const std::unordered_map<int, DecisionTree> &RandomForest::get_Trees() {
  return this->trees;
}


//
void RandomForest::train(const DataSet &data) {

  for (int i = 0; i < size; ++i) {

    DecisionTree tree{this->max_Depth, this->splitting_Criteria,
                      this->splitting_Operator};
    tree.train(data);
    this->trees.insert({i, tree});
  }
}

//
std::vector<double> RandomForest::predict(const DataSet &data) const {
  int size = data.samples_Number();
  std::vector<double> result(size, 0);

  std::vector<double> tree_Result(size, 0);
  // Iterate through the Forest
  for (size_t i = 0; i < this->trees.size(); ++i) {

    // Computes the prediction for the current tree
    if (this->trees.find(i) == this->trees.end()) {
      std::cerr << "Couldn't find wanted tree \n";
      exit(1);
    }

    tree_Result = this->trees.at(i).predict(data);

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
