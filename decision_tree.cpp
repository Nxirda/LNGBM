#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "decision_tree.h"
#include "data_loading.h"

using namespace std;

//Fonction qui construit un arbre 
DecisionTree::TreeNode* buildTree(const Dataset& data, const std::vector<std::string>& labels){

    if(!data.size() || !labels.size()){ // Si l'une des valeurs est vide 

        return nullptr;
    }

    DecisionTree::TreeNode* node = new DecisionTree::TreeNode; // Création d'un noeud
    
    node ->Left = buildTree(data,labels);
    node ->Right = buildTree(data,labels);

    return node;

}



