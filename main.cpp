#include <stdio.h>
#include "DataSet.hpp"
#include "DecisionTree.hpp"

using namespace std;

/*********************/
/*                   */
/*    TEMPORARY      */
/*                   */
/*********************/


int main() {

  /*cout << "=== DataSet Loading ===\n";
  // cout << " Enter the Path of the CSV : \n";
  // cin >>;
  DataSet D{"../data/datasets/d1.csv"};*/

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

  cout << "= DataSet copy test = \n";
  DataSet truc = testing_DS;
  truc.print();
  std::vector<int> idx {0,1,2,3}; 
  cout << " = Node copy tests =\n";
  std::shared_ptr<DataSet> tn_test = std::make_shared<DataSet> (testing_DS);
  TreeNode tn{tn_test, idx};
  TreeNode ts;
  ts = tn;
  ts.get_DataSet().print();

  cout << " = Decision Tree Copy tests =\n";
  DecisionTree DT{testing_DS};
  DT.print_Tree();
  DecisionTree DS;
  DS = DT;
  DS.print_Tree();

  cout << "=== Decision Tree Global Tests === \n";
  cout << "= Add Left & Right =\n";
  vector<DataSet> div = DT.get_Current_Node().get_DataSet().split(0,5); 

  std::unique_ptr<DecisionTree> dt(new DecisionTree{div[0]});
  DS.add_Left(std::move(dt));
  std::unique_ptr<DecisionTree> dt2(new DecisionTree{div[1]});
  DS.add_Right(std::move(dt2));
  DS.print_Tree();

  cout << "= find best feature test =\n";
  cout << DS.find_Best_Feature() << endl;;
  
  // Building a one node Tree
  //DecisionTree DT{testing_DS};
  //cout << DT.find_Best_Feature() << endl;

  /*cout << "Decision Tree is :\n";
  DT.get_Current_Node().get_DataSet().print();

  cout << "Copy column test \n";
  //vector<float> column = DT.get_Current_Node().get_DataSet().get_Column(0);
  DT.FindBestAttribute();
  rec_Naive_Splitting(&DT);
  DT.print_Tree();*/

  return 0;
}