#include <iostream>
#include <string>
#include <vector>

#include "data_loading.h"

#ifndef DecisionTree_H_
#define DecisionTree_H_

// Node Class
class TreeNode{ 
	protected:
		//Parameters
		Dataset data;
		//Division Function to be added
		
	public:
		//Constructor
		TreeNode();
		TreeNode(const Dataset& d);

		//Getters
		Dataset get_Dataset();

		//Methods
				
};

// Binary Search Tree Class
class DecisionTree { 
	protected:
		//Parameters
		DecisionTree* Parent;
		TreeNode   Curr_Node; // Actual Node with properties
		DecisionTree*  Right; // Right Node pointer
		DecisionTree*   Left; // Left  Node pointer
	
	public :
		//Constructor
		DecisionTree(const Dataset& data);

		//Getters 
		TreeNode get_Current_Node   ();
		DecisionTree *get_ParentTree();
		DecisionTree *get_RightTree ();
		DecisionTree *get_LeftTree  ();

		//Setters
		void add_right(Dataset data);
		void add_left (Dataset data);

		//Methods
		DecisionTree *buildTree();
		

};

#endif
