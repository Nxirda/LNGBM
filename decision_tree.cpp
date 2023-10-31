#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>

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

/* Returns the Parent Tree */
DecisionTree *DecisionTree::get_ParentTree()
{
    return this->Parent;
}

/* Returns the Left Sub Tree*/
DecisionTree *DecisionTree::get_LeftTree()
{
    return this->Left;
}

/* Returns the Right Sub Tree */
DecisionTree *DecisionTree::get_RightTree()
{
    return this->Right;
}

/* Sets a new left Subtree */
void DecisionTree::add_left(Dataset data)
{
    DecisionTree left{data};
    this->Left = &left;
}

/* Sets a new right Subtree */
void DecisionTree::add_right(Dataset data)
{
    DecisionTree right{data};
    this->Right = &right;
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

    for(int i = 0; i < data.Label_length(); ++i)
    {

        /* Splitting the labels */
        if(i < data.Label_length()/2)
        {
            subLabels1.push_back(data.get_Labels()[i]);
        }
        else
        {
            subLabels2.push_back(data.get_Labels()[i]);
        }
        
        /* Splitting the datas accordingly */
        //We get the current line (easier to manipulate)
        vector<float> curr_line = data.get_Values()[i];

        vector<float> left;
        vector<float> right;

        //We define an iterator that splits de columns in two parts (half)
        vector<float>::iterator middleItr(curr_line.begin() + curr_line.size() / 2);

        //Loop to put the datas in the half it belongs to 
        for (auto it = curr_line.begin(); it != curr_line.end(); ++it)
        {
            if (std::distance(it, middleItr) > 0) 
            {
                left.push_back(*it);
            }
            else 
            {
                right.push_back(*it);
            }
        }
        
        //
        subSet1.push_back(left);
        subSet2.push_back(right);

    }

    //Create two "new" Datasets
    Dataset First_Half  {subLabels1, subSet1};
    Dataset Second_Half {subLabels2, subSet2};

    //Cast the two half in a vector (could be an array at some point)
    std::vector<Dataset> res;
    res.push_back(First_Half);
    res.push_back(Second_Half);

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