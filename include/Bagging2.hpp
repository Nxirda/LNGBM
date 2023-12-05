#ifndef BAGGING_MODEL_H_
#define BAGGING_MODEL_H_

#include <iostream>
#include <memory>
#include "DataSet.hpp"
#include "DecisionTree.hpp"
#include "IOperator.hpp"

class BaggingModel {
private:
    IOperator *splitMetric;
    std::unique_ptr<DecisionTree> mainTree;
    int maxDepth;
    std::string metric;

public:
    BaggingModel();
    BaggingModel(std::string splitMetric, int maxDepth);
    ~BaggingModel();

    void fit(const DataSet &data);
    void predict(const DataSet &data);
    void print_Available_Operators();

private:
    void initializeSplitMetric();
};

#endif

