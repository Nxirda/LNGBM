#include "BaggingModel.hpp"
#include "EnumOperator.hpp"

/********************/
/*                  */
/*  BAGGING CLASS   */
/*                  */
/********************/

/*
Default Constructor
Inputs  :
Outputs :
*/
BaggingModel::BaggingModel() {
  this->forest = RandomForest();
  this->max_Depth = 0;
  this->metric = "NO METRIC";
}

/*
Constructor with operator and depth fixed
Inputs  : string, int
Outputs :
*/
BaggingModel::BaggingModel(std::string split_Metric, int max_Depth) {
  // Here we just prepare the infos for the model
  this->metric = split_Metric;
  this->max_Depth = max_Depth;
  //this->main_Tree = std::make_unique<DecisionTree>();

  std::map<std::string, OperatorType>::iterator it;

  it = operator_Dictionnary.find(this->metric);

  if (it == operator_Dictionnary.end()) {
    std::cout << "Chosen metric is invalid \n" << std::endl;
    abort();
  }

  switch (it->second) {
  case (OperatorType::MAE):
    this->split_Metric = new MAE();
    break;
  case (OperatorType::MAPE):
    this->split_Metric = new MAPE();
    break;
  case (OperatorType::RMSE):
    this->split_Metric = new RMSE();
    break;
  case (OperatorType::ReductionInVar):
    this->split_Metric =
        new ReductionInVar();
    break;
  default:
    std::cout << "Chosen metric is invalid \n" << std::endl;
    abort();
  }
}

/*
Destructor
Inputs  :
Outputs :
*/
BaggingModel::~BaggingModel() { delete (this->split_Metric); };

/*
train the model on the DataSet with the operator and depth fixed
Inputs  : const DataSet, int
Outputs :
*/
void BaggingModel::train(const DataSet &data, int n) {
  std::cout << "=== " << this->metric << " ===\n";

  /* std::vector<float> predictions;
  std::vector<float> aggregation_Of_Results; */

  std::shared_ptr<DataSet> dataset = std::make_shared<DataSet>(data);

  this->forest = RandomForest(dataset, this->split_Metric, n, this->max_Depth);

  this->forest.generate_Forest(n);
}

/*
predict the labels for the given dataset with the operator and depth fixed
Inputs  : const DataSet
Outputs :
*/
std::vector<float> BaggingModel::predict(const DataSet &data) {
  std::shared_ptr test_DataSet = std::make_shared<DataSet>(data);
  this->forest.predict_Results(test_DataSet);
  return this->forest.get_results();
}

/*
Get the prediction we made on the dataset with the operator and depth fixed
Inputs  :
Outputs : vector<float>
*/
std::vector<float> BaggingModel::get_Prediction() {
    return this->forest.get_results();
}

/*
Prints the available operator for this library
Inputs  :
Outputs :
*/
void BaggingModel::print_Available_Operators() {
  for (auto const &pair : operator_Dictionnary) {
    std::cout << "{" << pair.first << "}\n";
  }
}

/*
Computes a vector of random index in the main_Tree DataSet
Inputs  :
Outputs : 
*/
/* std::vector<int> BaggingModel::bootstrap_DataSet() {
  int len = this->main_Tree->get_Current_Node()->get_Index().size();
  std::vector<int> bootstrap_Index(len);
  for(int i = 0; i < len; i++){
    bootstrap_Index[i] = rand() % len;
  }
  return bootstrap_Index;
} */

/**/
/* void BaggingModel::fit(const DataSet &data) {
  // For now it only construct one tree
  std::cout << "=== " << this->metric << " ===\n";

  // Create the tree
  this->main_Tree = std::make_unique<DecisionTree>(data);

  // Set the right node in the metric
  this->split_Metric->set_Node(this->main_Tree->get_Current_Node());

  this->main_Tree->add_Operator(this->split_Metric);

  this->main_Tree->build_Splitted_Tree(this->max_Depth);

  // this->main_Tree->print_Tree();

  delete (this->split_Metric);
} */