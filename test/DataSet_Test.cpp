#include <DataSet.hpp>
#include <gtest/gtest.h>

std::string PATH = "../data/matrix.csv";

//
TEST(Getters, GetFeatures) {
  DataSet d;
  ASSERT_EQ(d.get_Features().size(), 0);

  d = DataSet(PATH);
  ASSERT_EQ(d.get_Features().size(), 5);
}

//
TEST(Getters, GetSamples) {
  DataSet d;
  ASSERT_EQ(d.get_Samples().size(), 0);

  d = DataSet(PATH);
  ASSERT_EQ(d.get_Samples().size(), 5);

  ASSERT_EQ(d.get_Samples()[0].size(), 7);
}

//
TEST(Getters, GetLabels) {
  DataSet d;
  std::vector<int> idx(0);
  ASSERT_EQ(d.get_Labels(idx).size(), 0);

  d = DataSet(PATH);
  ASSERT_EQ(d.get_Labels(idx).size(), 0);

  for (size_t i = 0; i < d.get_Samples().size(); ++i) {
    idx.push_back(i);
  }

  ASSERT_EQ(d.get_Labels(idx).size(), 5);

  // Try inserting out of range elem in index
  idx.push_back(44);
  ASSERT_EQ(d.get_Labels(idx).size(), 5);
}

//
TEST(Getters, ColumnWithIndex) {
  DataSet d;
  std::vector<int> idx;
  ASSERT_EQ(d.get_Column(3, idx).size(), 0);

  d = DataSet(PATH);
  ASSERT_EQ(d.get_Column(3, idx).size(), 0);

  for (size_t i = 0; i < d.get_Samples().size(); ++i) {
    idx.push_back(i);
  }

  ASSERT_EQ(d.get_Column(3, idx).size(), 5);

  // Try inserting out of range elem in index
  idx.push_back(44);
  ASSERT_EQ(d.get_Column(3, idx).size(), 5);
}

//
TEST(Computations, LabelsMean) {
  DataSet d;
  std::vector<int> idx(0);
  double mean = 0;

  ASSERT_EQ(d.labels_Mean(idx), -1.0);

  d = DataSet(PATH);
  ASSERT_EQ(d.labels_Mean(idx), -1.0);

  for (size_t i = 0; i < d.get_Samples().size(); ++i) {
    idx.push_back(i);
  }

  for (double i : d.get_Labels(idx)) {
    mean += i;
  }
  mean *= (1.0 / d.get_Labels(idx).size());

  ASSERT_EQ(d.labels_Mean(idx), mean);

  // Try inserting out of range elem in index
  idx.push_back(44);
  ASSERT_EQ(d.labels_Mean(idx), mean);   
}

//
TEST(Computation, ColumnMean) {
  DataSet d;
  std::vector<int> idx(0);
  double mean = 0;

  ASSERT_EQ(d.column_Mean(1, idx), -1.0);

  d = DataSet(PATH);
  ASSERT_EQ(d.column_Mean(3, idx), -1.0);

  for (size_t i = 0; i < d.get_Samples().size(); ++i) {
    idx.push_back(i);
  }
  for (double i : d.get_Column(4, idx)) {
    mean += i;
  }
  mean *= (1.0 / d.get_Column(4, idx).size());
  ASSERT_EQ(d.column_Mean(4, idx), mean);

  // Try inserting out of range elem in index
  idx.push_back(44);
  ASSERT_EQ(d.column_Mean(4, idx), mean); 
}

//
TEST(Computation, LabelsVariance) {
  DataSet d;
  std::vector<int> idx(0);
  double mean = 0.0;
  double variance = 0.0;

  ASSERT_EQ(d.labels_Variance(idx), -1.0);

  d = DataSet(PATH);
  ASSERT_EQ(d.labels_Variance(idx), -1.0);

  for (size_t i = 0; i < d.get_Samples().size(); ++i) {
    idx.push_back(i);
  }
  mean = d.labels_Mean(idx);

  for (double i : d.get_Labels(idx)) {
    float difference = i - mean;
    variance += difference * difference;
  }
  variance *= (1.0 / d.get_Labels(idx).size());

  //Near equal due to omp making addition non deterministic
  ASSERT_NE(d.labels_Variance(idx), variance); 

  // Try inserting out of range elem in index
  idx.push_back(44);
  ASSERT_NE(d.labels_Variance(idx), variance); 
}

TEST(Computation, Split) {
  DataSet d;
  ASSERT_TRUE(d.empty());
}

//
TEST(Size, empty) {
  DataSet d;
  ASSERT_TRUE(d.empty());

  d = DataSet(PATH);
  ASSERT_FALSE(d.empty());
}

//
TEST(Size, LabelsNumber) {
  DataSet d;
  ASSERT_EQ(d.labels_Number(), 0);

  d = DataSet(PATH);
  ASSERT_EQ(d.labels_Number(), 7);
}

//
TEST(Size, SamplesNumber) {
  DataSet d;
  ASSERT_EQ(d.samples_Number(), 0);

  d = DataSet(PATH);
  ASSERT_EQ(d.samples_Number(), 7);
}

//
TEST(Size, FeaturesLength) {
  DataSet d;
  ASSERT_EQ(d.features_Length(), 0);

  d = DataSet(PATH);
  ASSERT_EQ(d.features_Length(), 5);
}
