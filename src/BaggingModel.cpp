#include <BaggingModel.hpp>
#include <ReductionInVar.hpp> //then we will have an enum for the operators

/**/
BaggingModel::BaggingModel(std::string split_Metric, int max_Depth) {
  // Here we just prepare the infos for the model
  this->metric = split_Metric;
  this->max_Depth = max_Depth;
}

/**/
BaggingModel::~BaggingModel() { delete (split_Metric); }

/**/
void BaggingModel::fit(const DataSet &data) {

  this->main_Tree = std::make_unique<DecisionTree>(data);

  this->split_Metric = new ReductionInVar(this->main_Tree->get_Current_Node());

  this->main_Tree->add_Operator(this->split_Metric);

  this->main_Tree->build_Splitted_Tree(this->max_Depth);
}

/**/
void BaggingModel::predict(const DataSet &data) {
  std::shared_ptr test_DataSet = std::make_shared<DataSet>(data);
  this->main_Tree->set_Test_DataSet(test_DataSet);
  this->main_Tree->predict_Test_DataSet();
}
