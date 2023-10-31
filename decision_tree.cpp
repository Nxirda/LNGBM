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

/* Override "=" operator */
TreeNode &TreeNode::operator= (TreeNode const& TN)
{
    data = get_Dataset(); 
    return *this;
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
    this->Curr_Node = new TreeNode{data};
    this->Left = nullptr;
    this->Right = nullptr;
}

/* Override "=" operator */
DecisionTree &DecisionTree::operator= (const DecisionTree & DT)
{
    Parent = &get_ParentTree();
    Curr_Node = &get_Current_Node();
    Left = &get_LeftTree();
    Right = &get_RightTree(); 

    return *this;
}

/* Returns the Current Node of the Tree */
TreeNode &DecisionTree::get_Current_Node()
{
    return *this->Curr_Node;
}

/* Returns the Parent Tree */
DecisionTree &DecisionTree::get_ParentTree()
{
    return *this->Parent;
}

/* Returns the Left Sub Tree*/
DecisionTree &DecisionTree::get_LeftTree()
{
    return *this->Left;
}

/* Returns the Right Sub Tree */
DecisionTree &DecisionTree::get_RightTree()
{
    return *this->Right;
}

/* Sets a new Parent for the given tree*/
void DecisionTree::add_Parent(DecisionTree *d)
{
    this->Parent = d;
}

/* Sets a new left Subtree */
void DecisionTree::add_left(Dataset data)
{
    this->Left = new DecisionTree{data};
    this->Left->add_Parent(this);
}

/* Sets a new right Subtree */
void DecisionTree::add_right(Dataset data)
{
    this->Right = new DecisionTree{data};
    this->Right->add_Parent(this);
}

/* Print function for Decision Trees */
void DecisionTree::print_Tree()
{
    this->get_Current_Node().get_Dataset().print();
    if(&this->get_LeftTree() != nullptr )
    {
        this->get_LeftTree().print_Tree();
    }
    if(&this->get_RightTree() != nullptr )
    {
        this->get_RightTree().print_Tree();
    }
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
std::vector<Dataset> Data_Splitting_in_two(DecisionTree *DT)
{
    //Naive version for testing
    std::vector<std::string> subLabels1;
    std::vector<std::vector<float>> subSet1;

    std::vector<std::string> subLabels2;
    std::vector<std::vector<float>> subSet2;

    Dataset data = DT->get_Current_Node().get_Dataset();
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
    }  
        /* Splitting the datas accordingly */
        //We get the current line (easier to manipulate)
    for(int i = 0; i < data.Entries_size(); ++i)
    {
        vector<float> curr_line;
        for(int j = 0; j < data.Label_length(); ++j) 
        {
            curr_line.push_back(data.get_Values()[i][j]);
        }

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

/* Calls the splitting function recursively on the tree */
void rec_Naive_Splitting(DecisionTree *DT)
{
    if(DT->get_Current_Node().get_Dataset().Label_length() > 1)
    {
        vector<Dataset> Data_Splitted = Data_Splitting_in_two(DT);
        DT->add_left (Data_Splitted[0]);
        DT->add_right(Data_Splitted[1]);

        /*
        cout << "Left Sub Tree :\n";
        DT->get_LeftTree().get_Current_Node().get_Dataset().print();
        cout << "Right Sub Tree :\n";
        DT->get_RightTree().get_Current_Node().get_Dataset().print(); 
        */       

        rec_Naive_Splitting (&DT->get_LeftTree());
        rec_Naive_Splitting(&DT->get_RightTree());
    }
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

    cout << "=== Dataset Loading & Tests ===\n";

    cout << "Dataset copy test \n";
    Dataset truc = testing_DS;
    truc.print();

    cout << "Node copy test \n";
    TreeNode tn{testing_DS};
    TreeNode ts = tn;
    ts.get_Dataset().print();

    cout << "Data set is :\n";
    testing_DS.print();

    // Building a one node Tree
    DecisionTree DT{testing_DS};

    cout << "Decision Tree is :\n";
    //DT.get_Current_Node()->get_Dataset().print();
    DT.get_Current_Node().get_Dataset().print();

    rec_Naive_Splitting(&DT);
    DT.print_Tree();
    /* 
    vector<Dataset> split_Test = Data_Splitting_in_two(DT.get_Current_Node().get_Dataset());

    DT.add_left (split_Test[0]);
    DT.add_right(split_Test[1]);

    cout << "Sub Tree left is :\n";
    DecisionTree left = DT.get_LeftTree();
    left.get_Current_Node().get_Dataset().print();   
    cout << "Sub Tree left Parent is :\n";
    left.get_ParentTree().get_Current_Node().get_Dataset().print();

    cout << "Sub Tree right is :\n";
    DecisionTree right = DT.get_RightTree();
    right.get_Current_Node().get_Dataset().print();    
    cout << "Sub Tree right Parent is :\n";
    right.get_ParentTree().get_Current_Node().get_Dataset().print();
    */

    return 0;
}