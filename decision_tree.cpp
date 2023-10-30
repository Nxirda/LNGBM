#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "decision_tree.h"

//using namespace std;

/********************/
/*                  */
/*    NODE PART     */
/*                  */
/********************/

/* Default Constructor */
TreeNode::TreeNode(){}

/* Constructor with Dataset :            */
/* Build a Node with Dataset infos in it */
TreeNode::TreeNode(const Dataset& d)
{
    this->data = d;
}

/* Returns the Node's Dataset */
Dataset TreeNode::get_Dataset()
{
    return this->data;
}


/********************/
/*                  */
/*    TREE PART     */
/*                  */
/********************/

/* Base Constructor with a Dataset parameter :          */
/* Builds a Tree with One node containing all the Datas */
DecisionTree::DecisionTree(const Dataset& data)
{
    this->Parent = nullptr;
    this->Curr_Node = TreeNode{data};
    this->Left = nullptr;
    this->Right = nullptr;
}

/* Returns the Current Node of the Tree */
TreeNode DecisionTree::get_Current_Node()
{
    return this->Curr_Node;
}


/**********************/
/*                    */
/*    CLOBBERS        */
/*                    */
/**********************/

//Fonction qui construit un arbre 
/*
DecisionTree *buildTree(const Dataset& data)
{

    if(data.empty())
    { // Si le dernier noeud est nulle 

        return nullptr;
    }
    
    //std::string splitAttribute = Algo_Splitting(data);

    TreeNode* node = new TreeNode{}; // Création d'un noeud
    //node->attribute = splitAttribute;

    // Division des données en deux sous-ensembles en fonction de l'attribut de division
    //std::vector<Dataset> subsets = Data_Splitting(data, splitAttribute);

    // Division des labels en deux sous-ensembles correspondants
    //std::vector<std::vector<std::string>> subsetsLabels = Labels_Splitting(labels, splitAttribute); 

    std::vector<Dataset> split = Data_Splitting_in_two(data);

    // Construction des sous-arbres récursivement
    node->Left = buildTree(split[0]);
    node->Right = buildTree(split[1]);
    
    return node;

}*/

//Fonction de l'ago de splitting 
/*
std::string Algo_Splitting(const Dataset& data) 
{
    return "";
}*/

/*
// Fonction qui divise les labels 
std::vector<std::vector<std::string>> Labels_Splitting(const std::vector<std::string>& labels, const std::string& attribute) 
{
    
}*/

/* Divides the dataset in two subsets depending on the label size */
std::vector<Dataset> Data_Splitting_in_two(const Dataset& data)
{
    //Naive version for testing
    std::vector<std::string> subLabels1;
    std::vector<std::vector<float>> subSet1;

    std::vector<std::string> subLabels2;
    std::vector<std::vector<float>> subSet2;

    for(int i = 0; i<data.Label_length(); ++i)
    {
        if (i < data.Entries_size() / 2)
        {
            subLabels1.push_back(data.get_Labels()[i]);
            subSet1.push_back(data.get_Values()[i]);
        }

        else
        {
            subLabels2.push_back(data.get_Labels()[i]);
            subSet2.push_back(data.get_Values()[i]);
        }
    }

    Dataset subset_1 {subLabels1, subSet1};
    Dataset subset_2 {subLabels2, subSet2};

    std::vector<Dataset> res;
    res.push_back(subset_1);
    res.push_back(subset_2);

    return (res);
}

/**********************/
/*                    */
/*    TESTING         */
/*                    */
/**********************/

int main()
{

    cout << "=== Dataset Loading ===\n";
    //cout << " Enter the Path of the CSV : \n";
    //cin >>;
    Dataset D{"../methode_ensemblistes_modelisation/datasets/d1.csv"};
    //D.print();

    //Initialize labels
    vector<string> label;
    label.push_back("Test1");
    label.push_back("Test2");
    label.push_back("Test3");

    //Initialize the Datas
    vector<vector<float>> Datas;

    // Create a vector of size n with all values as 2nd parameter.
    vector<float> vect(3, 10);
    vector<float> vect2(3, 4);
    vector<float> vect3(3, 2);

    // Fill the Datas Vector
    Datas.push_back (vect);
    Datas.push_back(vect2);
    Datas.push_back(vect3);

    Dataset testing_DS{label, Datas};
    testing_DS.print();

    // Building a one node Tree
    DecisionTree DT{testing_DS};
    DT.get_Current_Node().get_Dataset().print();

    vector<Dataset> split_Test = Data_Splitting_in_two(DT.get_Current_Node().get_Dataset());

    split_Test[0].print();
    split_Test[1].print();

    return 0;
}