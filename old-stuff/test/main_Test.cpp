#include "gtest/gtest.h"

#include "Dataset/DataSet_Test.cpp"
#include "PredictionModel/Trees/DecisionTree_Test.cpp"
#include "PredictionModel/Trees/TreeNode_Test.cpp"


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}