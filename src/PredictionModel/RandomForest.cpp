#include <algorithm>
#include <stack>

#include "RandomForest.hpp"
#include "TrainingElement.hpp"

/**************************/
/*                        */
/*  RANDOM FOREST CLASS   */
/*                        */
/**************************/

//
RandomForest::RandomForest() noexcept{
  this->size = 0;
  this->max_Depth = 0;
  this->trees = std::unordered_map<uint16_t, DecisionTree>();
}

//
RandomForest::RandomForest(uint16_t n, uint16_t depth) noexcept{
  this->size = n;
  this->max_Depth = depth;
  this->trees = std::unordered_map<uint16_t, DecisionTree>(n);
}

//
RandomForest::RandomForest(RandomForest &&forest) noexcept{
  this->size = forest.size;
  this->max_Depth = forest.max_Depth;
  this->trees = std::move(forest.trees);
}

//
RandomForest &RandomForest::operator=(RandomForest &&forest) noexcept {
  this->size = forest.size;
  this->max_Depth = forest.max_Depth;
  this->trees = std::move(forest.trees);
  return *this;
}

//
RandomForest::~RandomForest(){};

//
uint16_t RandomForest::get_size() const { return this->size; }

//
const std::unordered_map<uint16_t, DecisionTree> &
RandomForest::get_Trees() const {
  return this->trees;
}

//
void RandomForest::train(const DataSet &data, ICriteria *crit, IOperator *op) {

  for (uint16_t i = 0; i < size; ++i) {

    DecisionTree tree{this->max_Depth};

    tree.train(data, crit, op);
    this->trees.emplace(i, std::move(tree));
  }
}

//
std::vector<double> RandomForest::predict(const DataSet &data) const {
  const size_t size = data.samples_Number();  
  std::vector<double> result(size, 0);

  std::vector<double> tree_Result(size, 0);

  // Iterate through the Forest
  for (size_t i = 0; i < this->trees.size(); ++i) {

    // Computes the prediction for the current tree
    if (this->trees.find(i) == this->trees.end()) {
      std::cerr << "Couldn't find wanted tree \n";
      exit(1);
    }
    //std::cout << "Random Forest Prediction is wrong with MPI\n";
    tree_Result = std::move(this->trees.at(i).predict(data));

    // Adds two vectors
    std::transform(result.begin(), result.end(), tree_Result.begin(),
                   result.begin(), std::plus<double>());
  }

  // Divides to have the mean of the answers
  for (size_t j = 0; j < size; ++j) {
    result[j] *= (1.0 / this->trees.size());
  }

  return result;
}