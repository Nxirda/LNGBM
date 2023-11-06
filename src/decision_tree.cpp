#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>


#include "../include/decision_tree.h"

/********************/
/*                  */
/*    NODE PART     */
/*                  */
/********************/

/* Default Constructor */
TreeNode::TreeNode() {}

/* Constructor with Dataset :            */
/* Build a Node with Dataset infos in it */
TreeNode::TreeNode(const Dataset &d) { this->data = d; }

/* Override "=" operator */
TreeNode &TreeNode::operator=(TreeNode const &TN) {
  data = get_Dataset();
  return *this;
}

/* Returns the Node's Dataset */
Dataset TreeNode::get_Dataset() { return this->data; }

/********************/
/*                  */
/*    TREE PART     */
/*                  */
/********************/

/* Base Constructor with a Dataset parameter :          */
/* Builds a Tree with One node containing all the Datas */
DecisionTree::DecisionTree(const Dataset &data) {
  this->Parent = nullptr;
  this->Curr_Node = new TreeNode{data};
  this->Left = nullptr;
  this->Right = nullptr;
}

/* Override "=" operator */
DecisionTree &DecisionTree::operator=(const DecisionTree &DT) {
  Parent = &get_ParentTree();
  Curr_Node = &get_Current_Node();
  Left = &get_LeftTree();
  Right = &get_RightTree();

  return *this;
}

/* Returns the Current Node of the Tree */
TreeNode &DecisionTree::get_Current_Node() { return *this->Curr_Node; }

/* Returns the Parent Tree */
DecisionTree &DecisionTree::get_ParentTree() { return *this->Parent; }

/* Returns the Left Sub Tree*/
DecisionTree &DecisionTree::get_LeftTree() { return *this->Left; }

/* Returns the Right Sub Tree */
DecisionTree &DecisionTree::get_RightTree() { return *this->Right; }

/* Sets a new Parent for the given tree*/
void DecisionTree::add_Parent(DecisionTree *d) { this->Parent = d; }

/* Sets a new left Subtree */
void DecisionTree::add_left(Dataset data) {
  this->Left = new DecisionTree{data};
  this->Left->add_Parent(this);
}

/* Sets a new right Subtree */
void DecisionTree::add_right(Dataset data) {
  this->Right = new DecisionTree{data};
  this->Right->add_Parent(this);
}

/* Print function for Decision Trees */
void DecisionTree::print_Tree() {
  this->get_Current_Node().get_Dataset().print();
  if (&this->get_LeftTree() != nullptr) {
    this->get_LeftTree().print_Tree();
  }
  if (&this->get_RightTree() != nullptr) {
    this->get_RightTree().print_Tree();
  }
}

void DecisionTree::Algo_Splitting(DecisionTree* tree) {
    // Condition d'arrêt
  if (Stop_Condition(tree->get_Current_Node().get_Dataset())) {
        return;
    }

    // Choix de l'attribut de division
  std::string splitAttribute = chooseSplitAttribute(tree->get_Current_Node().get_Dataset());

    // Divisez les données en deux sous-ensembles en fonction de l'attribut de division
  std::vector<Dataset> subsets = splitByAttribute(tree->get_Current_Node().get_Dataset(), splitAttribute);

    // Créez des nœuds enfants et divisez-les récursivement    
  if (!subsets.empty()) {
     tree->add_left(subsets[0]);        
     Algo_Splitting(&tree->get_LeftTree());
     tree->add_right(subsets[1]);
     Algo_Splitting(&tree->get_RightTree());
  }
}

std::string DecisionTree::chooseSplitAttribute(const Dataset& data) {

  auto bestAttribute = std::min_element(data.get_Labels().begin(), data.get_Labels().end(), [&](const std::string& attr1, const std::string& attr2) {
            return computeGiniIndex(data, attribute1) < computeGiniIndex(data, attribute2);
        });

  return *bestAttribute;
  
}
double DecisionTree::computeGiniIndex(const Dataset& data, const std::string& attribute) {

  std::vector<std::string> labels = data.get_Labels();

    // Calculer l'indice de Gini pour l'attribut
    double giniIndex = 1.0;
    int totalEntries = labels.size();

    if (totalEntries == 0) {
        return 0.0;  // Aucune entrée, indice de Gini nul
    }

    // Fréquence de chaque étiquette
    std::map<std::string, int> labelCounts;
    for (const std::string& label : labels) {
        labelCounts[label]++;
    }

    // Calculer de l'indice de Gini
    for (const auto& labelCount : labelCounts) {
        double labelProportion = static_cast<double>(labelCount.second) / totalEntries;
        giniIndex -= labelProportion * labelProportion;
    }

    return giniIndex;
}


std::vector<Dataset> DecisionTree::splitByAttribute(const Dataset& data, const std::string& attribute) { 
  
  vector<string> labels = data.get_Labels();
  vector<vector<float>> values = data.get_Values();
  std::vector<Dataset> subsets;

    // Cherchez l'indice de l'attribut spécifié
    int attributeIndex = -1;
    for (int i = 0; i < labels.size(); ++i) {
        if (labels[i] == attribute) {
            attributeIndex = i;
            break;
        }
    }

    if (attributeIndex == -1) {
        // L'attribut spécifié n'a pas été trouvé, retournez une liste vide
        return subsets;
    }

    // Divisez les données en fonction des valeurs uniques de l'attribut
    std::set<float> uniqueValues;
    for (const vector<float>& entry : values) {
        uniqueValues.insert(entry[attributeIndex]);
    }

    // Pour chaque valeur unique, créez un sous-ensemble de données
    for (float uniqueValue : uniqueValues) {
        vector<string> subLabels = labels;
        vector<vector<float>> subValues;

        for (const vector<float>& entry : values) {
            if (entry[attributeIndex] == uniqueValue) {
                subValues.push_back(entry);
            }
        }

        Dataset subset(subLabels, subValues);
        subsets.push_back(subset);
    }

    return subsets;
}



bool DecisionTree::Stop_Condition(const Dataset& data) {

    // Stop si toutes les étiquettes sont les mêmes.
  if (data.Label_length() <= 1) {
     return true;
   }
  return false;  
}


/**********************/
/*                    */
/*    CLOBBERS        */
/*                    */
/**********************/

// Fonction qui construit un arbre
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

    // Division des données en deux sous-ensembles en fonction de l'attribut de
division
    //std::vector<Dataset> subsets = Data_Splitting(data, splitAttribute);

    // Division des labels en deux sous-ensembles correspondants
    //std::vector<std::vector<std::string>> subsetsLabels =
Labels_Splitting(labels, splitAttribute);

    std::vector<Dataset> split = Data_Splitting_in_two(data);

    // Construction des sous-arbres récursivement
    node->Left = buildTree(split[0]);
    node->Right = buildTree(split[1]);

    return node;

}*/

// Fonction de l'ago de splitting
/*
std::string Algo_Splitting(const Dataset& data)
{
    return "";
}*/

/*
// Fonction qui divise les labels
std::vector<std::vector<std::string>> Labels_Splitting(const
std::vector<std::string>& labels, const std::string& attribute)
{

}*/
