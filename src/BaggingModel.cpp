#include "BaggingModel.hpp"
#include "EnumOperator.hpp"

/**/
BaggingModel::BaggingModel() {}

/**/
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

/**/
BaggingModel::~BaggingModel(){};

/**/
void BaggingModel::fit(const DataSet &data) {
  std::cout << "=== " << this->metric << " ===\n";

  // Create the tree
  this->main_Tree = std::make_unique<DecisionTree>(data);

  // Set the right node in the metric
  this->split_Metric->set_Node(this->main_Tree->get_Current_Node());

  this->main_Tree->add_Operator(this->split_Metric);

  this->main_Tree->build_Splitted_Tree(this->max_Depth);

  //this->main_Tree->print_Tree();

  delete (this->split_Metric);
}

/**/
void BaggingModel::predict(const DataSet &data) {

  std::shared_ptr test_DataSet = std::make_shared<DataSet>(data);

  this->main_Tree->set_Test_DataSet(test_DataSet);
  // ^_ Here we have a vector predicted value where everything is 0 in it
  this->main_Tree->parse_Test_DataSet();

  this->main_Tree->predict_Test_Labels();

}

void BaggingModel::print_Available_Operators() {
  for (auto const &pair : operator_Dictionnary) {
    std::cout << "{" << pair.first << "}\n";
  }
}