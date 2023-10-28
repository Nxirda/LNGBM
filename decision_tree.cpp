#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "decision_tree.h"
#include "data_loading.h"

using namespace std;
//Fonction de l'ago de splitting 
std::string Algo_Splitting(const Dataset& data, const std::vector<std::string>& labels) {
    
}
//Fonction qui divise les Data
std::vector<Dataset> Data_Splitting(const Dataset& data, const std::string& attribute){
    
}
//Fonction qui divise les labels 
std::vector<std::vector<std::string>> Labels_Splitting(const std::vector<std::string>& labels, const std::string& attribute) {
    
}



//Fonction qui construit un arbre 
DecisionTree::TreeNode* buildTree(const Dataset& data, const std::vector<std::string>& labels){

    if(data.empty() || labels.empty()){ // Si le dernier noeud est nulle 

        return nullptr;
    }
    std::string splitAttribute = Algo_Splitting(data, labels);

    DecisionTree::TreeNode* node = new DecisionTree::TreeNode; // Création d'un noeud
    node->attribute = splitAttribute;

    // Division des données en deux sous-ensembles en fonction de l'attribut de division
    std::vector<Dataset> subsets = Data_Splitting(data, splitAttribute);

    // Division des labels en deux sous-ensembles correspondants
    std::vector<std::vector<std::string>> subsetsLabels = Labels_Splitting(labels, splitAttribute); 

    // Construction des sous-arbres récursivement
    node->Left = buildTree(subsets[0], subsetsLabels[0]);
    node->Right = buildTree(subsets[1], subsetsLabels[1]);

    return node;

}



