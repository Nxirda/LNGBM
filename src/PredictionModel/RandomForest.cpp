#include <algorithm>
#include <stack>

#include "RandomForest.hpp"
#include "TrainingElement.hpp"

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
  this->results = std::vector<float>();
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
std::vector<float> RandomForest::get_results() { return this->results; }

/**/
int RandomForest::get_size() { return this->size; }

/**/
//std::shared_ptr<DataSet> RandomForest::get_Dataset() { return this->dataset; }

/**/
void RandomForest::generate_Forest(int size) {

  for (int i = 0; i < size; ++i) {
    std::cout << "===Iteration ["<< i << "]===\n";
    DecisionTree tree{};
    TrainingElement elem{};
    elem.set_Root(this->dataset.labels_Number(), tree.get_Root());

    elem.train(this->dataset, this->splitting_Operator, this->max_Depth);

    tree.set_Root(std::make_unique<TreeNode>(*elem.node));
    tree.get_Root()->node_Print_Criterion();

    //  Creating a Node
    /* std::shared_ptr<TreeNode> base_Node =
        make_shared<TreeNode>(this->get_Dataset()); */

    // Create a bootstrap sample
    // std::vector<int> bootstrap_Index = base_Node->bootstrap_DataSet();

    // Decision Tree Initialisation wtith bootstrapped index
    /* std::shared_ptr<DecisionTree> dt =
        std::make_shared<DecisionTree>(base_Node, bootstrap_Index); */

    // Handle random forest

    // this->splitting_Operator->set_Node(dt->get_Current_Node());

    // dt->add_Operator(this->splitting_Operator);

    // dt->build_Splitted_Tree(this->max_Depth);

    this->trees[i] = tree;
  }
}

/**/
/* void RandomForest::predict_Results(std::shared_ptr<DataSet> test_DataSet) {

  std::vector<float> accumulator;
  for (auto it = this->trees.begin(); it != this->trees.end(); ++it) {

    it->second->set_Test_DataSet(test_DataSet);

    it->second->parse_Test_DataSet();

    it->second->predict_Test_Labels();

    // Checks if we can add
    if (accumulator.size() <= 0) {
      accumulator = *it->second->get_Predicted_Labels();
    } else {
      if (accumulator.size() == it->second->get_Predicted_Labels()->size()) {

        // Adding up predictions at every index so we can compute the mean of
        // each prediction aftewards
        std::transform(accumulator.begin(), accumulator.end(),
                       it->second->get_Predicted_Labels()->begin(),
                       accumulator.begin(), std::plus<float>());
      } else {
        perror("Random Forest : one tree didnt predict on all the values");
        return;
      }
    }
  }
  if (size != 0) {
    for (unsigned long int i = 0; i < accumulator.size(); ++i) {
      accumulator[i] /= this->get_size();
    }
  }

  this->results = accumulator;
} */
