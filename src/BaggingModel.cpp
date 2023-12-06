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
  this->main_Tree = nullptr;
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
  this->main_Tree = std::make_unique<DecisionTree>();

  std::map<std::string, OperatorType>::iterator it;

  it = operator_Dictionnary.find(this->metric);

  if (it == operator_Dictionnary.end()) {
    std::cout << "Chosen metric is invalid \n" << std::endl;
    abort();
  }

  switch (it->second) {
  case (OperatorType::MAE):
    this->split_Metric = new MAE(this->main_Tree->get_Current_Node());
    break;
  case (OperatorType::MAPE):
    this->split_Metric = new MAPE(this->main_Tree->get_Current_Node());
    break;
  case (OperatorType::RMSE):
    this->split_Metric = new RMSE(this->main_Tree->get_Current_Node());
    break;
  case (OperatorType::ReductionInVar):
    this->split_Metric =
        new ReductionInVar(this->main_Tree->get_Current_Node());
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

  std::vector<float> predictions;
  std::vector<float> aggregation_Of_Results;

  // std::shared_ptr test_DataSet =
  //     std::make_shared<DataSet>(test_Data); // Temporary ofc

  // Initialise the Main Tree
  this->main_Tree = std::make_unique<DecisionTree>(data);

  this->split_Metric->set_Node(this->main_Tree->get_Current_Node());

  this->main_Tree->add_Operator(this->split_Metric);

  this->main_Tree->build_Splitted_Tree(this->max_Depth);

  for (int i = 1; i < n; ++i) {

    // Create bootstrap sample
    std::vector<int> new_Index = this->bootstrap_DataSet();

    // Handle random forest

    // train_DataSet = ...
    // test_DataSet = ...

    // Decision Tree Initialisation :
    DecisionTree dt = DecisionTree(data);
    dt.get_Current_Node()->set_Index(new_Index);

    this->split_Metric->set_Node(dt.get_Current_Node());

    dt.add_Operator(this->split_Metric);

    dt.build_Splitted_Tree(this->max_Depth);

    // Partial sum stored in main tree at each iter
    // Test for aggregating, ppbly wrong
    this->main_Tree->sum_Predicted_Labels(&dt);
  }
  // Other part of aggregation test
  this->main_Tree->divide_Predicted_Labels(n);
  this->main_Tree->print_Tree();
}

/*
predict the labels for the given dataset with the operator and depth fixed
Inputs  : const DataSet
Outputs :
*/
void BaggingModel::predict(const DataSet &data) {
  // This shall stay the same, we dont want to parse our dataset to predict
  // in the forest of tree but only in the aggregation tree after bagging
  std::shared_ptr test_DataSet = std::make_shared<DataSet>(data);

  this->main_Tree->set_Test_DataSet(test_DataSet);
  // ^_ Here we have a vector "predicted value" where everything is 0 in it
  this->main_Tree->parse_Test_DataSet();

  this->main_Tree->predict_Test_Labels();
}

/*
Get the prediction we made on the dataset with the operator and depth fixed
Inputs  :
Outputs : vector<float>
*/
std::vector<float> BaggingModel::get_Prediction() {
  return *this->main_Tree->get_Predicted_Labels();
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
std::vector<int> BaggingModel::bootstrap_DataSet() {
  int len = this->main_Tree->get_Current_Node()->get_Index().size();
  std::vector<int> bootstrap_Index(len);
  for(int i = 0; i < len; i++){
    bootstrap_Index[i] = rand() % len;
  }
  return bootstrap_Index;
}

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