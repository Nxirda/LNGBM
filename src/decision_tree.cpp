#include <fstream>
#include <iostream>
#include <iterator>
#include <limits.h> //For Max_Int value
#include <string>
#include <vector>
#include <algorithm>


#include "../include/decision_tree.h"

/********************/
/*                  */
/*    NODE PART     */
/*                  */
/********************/

/* Default Constructor                */
/* Inputs  :                          */
/* Outputs : Object of TreeNode Class */
TreeNode::TreeNode() {}

/* Default Destructor          */
/* Inputs  :                   */
/* Outputs :                   */
TreeNode::~TreeNode() {}

/* Constructor with Dataset :                       */
/* Inputs  : Object  of Dataset Class               */
/* Outputs : TreeNode Object containing the Dataset */
TreeNode::TreeNode(const Dataset &d) { this->data = d; }

/* Override "=" operator                 */
/* Inputs  : Object of TreeNode Class    */
/* Outputs : Object of TreeNode Class    */
TreeNode &TreeNode::operator=(TreeNode const &TN) {
  data = get_Dataset();
  return *this;
}

/* Returns the Node's Dataset        */
/* Inputs  :                         */
/* Outputs : Object of Dataset Class */
Dataset TreeNode::get_Dataset() { return this->data; }

/* Returns the Variance of the Node's Dataset */
/* Inputs  :                                  */
/* Outputs : float                            */
float TreeNode::NodeVariance() { return this->get_Dataset().Global_Variance(); }

/* return the Homogeneity as a boolean by comparing the variance */
/* for every Column of the given Tree Node Dataset               */
/* Inputs  :                                                     */
/* Outputs : bool                                                */
bool TreeNode::NodeHomogeneity() {
  int len = this->get_Dataset().Label_length();
  float var = this->get_Dataset().Column_Variance(0);
  for (int i = 1; i < len; ++i) {
    float tmp = this->get_Dataset().Column_Variance(i);
    //  If column variance is not uniform then return false
    if (tmp != var) {
      return false;
    }
  }
  return true;
}

/********************/
/*                  */
/*    TREE PART     */
/*                  */
/********************/

/* Base Constructor with a Dataset parameter */
/* Inputs  : Object of Dataset Class         */
/* Outputs : Object of Decision Tree Class   */
DecisionTree::DecisionTree(const Dataset &data) {
  this->Parent = nullptr;
  this->Curr_Node = std::move(new TreeNode{data});
  this->Left = nullptr;
  this->Right = nullptr;
}

/* Override "=" operator                   */
/* Inputs  : Object of Decision Tree Class */
/* Outputs : Object of Decision Tree Class */
DecisionTree &DecisionTree::operator=(const DecisionTree &DT) {
  Parent = &get_ParentTree();
  Curr_Node = &get_Current_Node();
  Left = &get_LeftTree();
  Right = &get_RightTree();

  return *this;
}

/* Default Destructor */
/* Inputs  :          */
/* Outputs :          */
DecisionTree::~DecisionTree() {
  delete this->Curr_Node;
  delete this->Left;
  delete this->Right;
}

/* Returns the Current Node of the Tree      */
/* Inputs  :                                 */
/* Outputs : pointer of Decision Tree Object */
TreeNode &DecisionTree::get_Current_Node() { return *this->Curr_Node; }

/* Returns the Parent Tree                   */
/* Inputs  :                                 */
/* Outputs : pointer of Decision Tree Object */
DecisionTree &DecisionTree::get_ParentTree() { return *this->Parent; }

/* Returns the Left Sub Tree                 */
/* Inputs  :                                 */
/* Outputs : pointer of Decision Tree Object */
DecisionTree &DecisionTree::get_LeftTree() { return *this->Left; }

/* Returns the Right Sub Tree                */
/* Inputs  :                                 */
/* Outputs : pointer of Decision Tree Object */
DecisionTree &DecisionTree::get_RightTree() { return *this->Right; }

/* Sets a new Parent for the given tree      */
/* Inputs  : pointer of Decision Tree Object */
/* Outputs :                                 */
void DecisionTree::add_Parent(DecisionTree *d) { this->Parent = std::move(d); }

/* Sets a new left Subtree          */
/* Inputs : Object of Dataset class */
/* Output :                         */
void DecisionTree::add_left(Dataset data) {
  this->Left = std::move(new DecisionTree{data});
  this->Left->add_Parent(this);
}

/* Sets a new right Subtree         */
/* Inputs : Object of Dataset class */
/* Output :                         */
void DecisionTree::add_right(Dataset data) {
  this->Right = std::move(new DecisionTree{data});
  this->Right->add_Parent(this);
}

/* Print function for Decision Trees */
/* Inputs  :                         */
/* Outputs :                         */
void DecisionTree::print_Tree() {
  this->get_Current_Node().get_Dataset().print();
  if (&this->get_LeftTree() != nullptr) {
    this->get_LeftTree().print_Tree();
  }
  if (&this->get_RightTree() != nullptr) {
    this->get_RightTree().print_Tree();
  }
}

<<<<<<< HEAD
/* Returns the variance of a split as the weighted average */
/* variance of child nodes                                 */
/* Inputs  : int                                           */
/* Outputs : float                                         */
float DecisionTree::splitting_variance(int position) {
  float split_criteria =
      this->get_Current_Node().get_Dataset().Column_Mean(position);
  vector<Dataset> Child_Nodes =
      this->get_Current_Node().get_Dataset().split(position, split_criteria);
=======
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
>>>>>>> Valentin

  float base_Population = this->get_Current_Node().get_Dataset().Entries_size();

  float Left_Variance = Child_Nodes[0].Global_Variance();
  float Left_weighted_average = Child_Nodes[0].Entries_size() / base_Population;
  float Right_Variance = Child_Nodes[1].Global_Variance();
  float Right_weighted_average =
      Child_Nodes[1].Entries_size() / base_Population;

  float weighted_average_var = Left_weighted_average * Left_Variance +
                               Right_weighted_average * Right_Variance;

  return weighted_average_var;
}

/* Search for the best attribute to split the dataset on at a given Node */
/* Inputs :                                                              */
/* Ouputs : String                                                       */
std::string DecisionTree::FindBestAttribute() {
  std::string BestAttribute = "";
  float maxReductionInVar = INT_MAX;

  vector<string> labels = this->get_Current_Node().get_Dataset().get_Labels();

  for (int i = 0; i < labels.size(); ++i) {
    float tmp_var = splitting_variance(i);
    if (tmp_var < maxReductionInVar) {
      maxReductionInVar = tmp_var;
      BestAttribute = labels[i];
    }
  }
  return BestAttribute;
}

/* Builds a Decision Tree recursively following a splitting criteria */
/* Inputs  :                                                         */
/* Outputs :                                                         */
void DecisionTree::Build_Splitted_Tree(DecisionTree *DT) {}
