#include "Bagging2.hpp"

BaggingModel::BaggingModel() : splitMetric(nullptr), maxDepth(0) {}

BaggingModel::BaggingModel(std::string splitMetric, int maxDepth)
    : splitMetric(nullptr), maxDepth(maxDepth), metric(splitMetric) {
    initializeSplitMetric();
}

BaggingModel::~BaggingModel() {
    delete splitMetric;
}


void BaggingModel::initializeSplitMetric() {
    auto it = operator_Dictionnary.find(metric);

    if (it == operator_Dictionnary.end()) {
        std::cout << "Chosen metric is invalid\n";
        abort();
    }

    switch (it->second) {
    case OperatorType::MAE:
        splitMetric = new MAE(mainTree->get_Current_Node());
        break;
    case OperatorType::MAPE:
        splitMetric = new MAPE(mainTree->get_Current_Node());
        break;
    case OperatorType::RMSE:
        splitMetric = new RMSE(mainTree->get_Current_Node());
        break;
    case OperatorType::ReductionInVar:
        splitMetric = new ReductionInVar(mainTree->get_Current_Node());
        break;
    default:
        std::cout << "Chosen metric is invalid\n";
        abort();
    }
}

void BaggingModel::fit(const DataSet &data) {
    std::cout << "=== " << metric << " ===\n";

    mainTree = std::make_unique<DecisionTree>(data);

    splitMetric->set_Node(mainTree->get_Current_Node());

    mainTree->add_Operator(splitMetric);

    mainTree->build_Splitted_Tree(maxDepth);

}

void BaggingModel::predict(const DataSet &data) {
    std::shared_ptr testDataSet = std::make_shared<DataSet>(data);

    mainTree->set_Test_DataSet(testDataSet);
    mainTree->parse_Test_DataSet();
    mainTree->predict_Test_Labels();
}

void BaggingModel::print_Available_Operators() {
    for (const auto &pair : operator_Dictionnary) {
        std::cout << "{" << pair.first << "}\n";
    }
}
