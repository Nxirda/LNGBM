#include "include/data_loading.h"
#include "include/decision_tree.h"

using namespace std;

/*********************/
/*                   */
/*    TEMPORARY      */
/*                   */
/*********************/

/* Divides the dataset in two subsets depending on the label size */
std::vector<DataSet> Data_Splitting_in_two(DecisionTree *DT) {
  // Naive version for testing
  std::vector<std::string> subLabels1;
  std::vector<std::vector<float>> subSet1;

  std::vector<std::string> subLabels2;
  std::vector<std::vector<float>> subSet2;

  DataSet data = DT->get_Current_Node().get_DataSet();
  for (int i = 0; i < data.features_Length(); ++i) {

    /* Splitting the labels */
    if (i < data.features_Length() / 2) {
      subLabels1.push_back(data.get_Features()[i]);
    } else {
      subLabels2.push_back(data.get_Features()[i]);
    }
  }
  /* Splitting the datas accordingly */
  // We get the current line (easier to manipulate)
  for (int i = 0; i < data.samples_Number(); ++i) {
    vector<float> curr_line;
    for (int j = 0; j < data.features_Length(); ++j) {
      curr_line.push_back(data.get_Samples()[i][j]);
    }

    vector<float> left;
    vector<float> right;

    // We define an iterator that splits de columns in two parts (half)
    vector<float>::iterator middleItr(curr_line.begin() + curr_line.size() / 2);

    // Loop to put the datas in the half it belongs to
    for (auto it = curr_line.begin(); it != curr_line.end(); ++it) {
      if (std::distance(it, middleItr) > 0) {
        left.push_back(*it);
      } else {
        right.push_back(*it);
      }
    }

    //
    subSet1.push_back(left);
    subSet2.push_back(right);
  }

  // Create two "new" DataSets
  DataSet First_Half{subLabels1, subSet1};
  DataSet Second_Half{subLabels2, subSet2};

  // Cast the two half in a vector (could be an array at some point)
  std::vector<DataSet> res;
  res.push_back(First_Half);
  res.push_back(Second_Half);

  return (res);
}

/* Calls the splitting function recursively on the tree */
void rec_Naive_Splitting(DecisionTree *DT) {
  //While DataSet as more than 1 column
  if (DT->get_Current_Node().get_DataSet().features_Length() > 1) {
    vector<DataSet> Data_Splitted = Data_Splitting_in_two(DT);
    DT->add_Left(Data_Splitted[0]);
    DT->add_Right(Data_Splitted[1]);
    rec_Naive_Splitting(&DT->get_Left_Tree());
    rec_Naive_Splitting(&DT->get_Right_Tree());
  }
}


int main() {

  /*cout << "=== DataSet Loading ===\n";
  // cout << " Enter the Path of the CSV : \n";
  // cin >>;
  DataSet D{"../methode_ensemblistes_modelisation/datasets/d1.csv"};*/

  // Initialize labels
  vector<string> label;
  label.push_back("Test1");
  label.push_back("Test2");
  label.push_back("Test3");

  // Initialize the Datas
  vector<vector<float>> Datas;

  // Create a vector of size n with all values as 2nd parameter.
  vector<float> vect(3, 10);
  vector<float> vect2(3, 4);
  vector<float> vect3(3, 2);
  vector<float> vect4(3, 7);

  // Fill the Datas Vector
  Datas.push_back(vect);
  Datas.push_back(vect2);
  Datas.push_back(vect3);
  Datas.push_back(vect4);

  DataSet testing_DS{label, Datas};

  cout << "=== DataSet Loading & Tests ===\n";

  cout << "DataSet copy test \n";
  DataSet truc = testing_DS;
  truc.print();

  cout << "Node copy test \n";
  TreeNode tn{testing_DS};
  TreeNode ts = tn;
  ts.get_DataSet().print();

  cout << "Data set is :\n";
  testing_DS.print();

  cout << "Splitting Test : \n";
  vector<DataSet> div = testing_DS.split(0, 5);
  div[0].print();
  div[1].print();

  // Building a one node Tree
  DecisionTree DT{testing_DS};
  cout << DT.find_Best_Attribute() << endl;

  /*cout << "Decision Tree is :\n";
  DT.get_Current_Node().get_DataSet().print();

  cout << "Copy column test \n";
  //vector<float> column = DT.get_Current_Node().get_DataSet().get_Column(0);
  DT.FindBestAttribute();
  rec_Naive_Splitting(&DT);
  DT.print_Tree();*/

  return 0;
}