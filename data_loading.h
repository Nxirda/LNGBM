
#include <iostream>
#include <string>
#include <vector>

#ifndef MyClass_H_
#define MyClass_H_

using namespace std;

class Dataset 
{
    protected:
        //Parameters
        vector<string> Labels;
        vector<vector<float>> Values;

    public:
        //Constructor
        Dataset(string FilePath);

        //Getters
        vector<string> get_Labels() const;
        vector<vector<float>> get_Values() const;

        //Override operands
        //Dataset(Dataset const& a); // copy operator
        //Dataset& operator= (Dataset const& a); // copy assignment

        //Methods
        void print() const;
        // Dans votre classe Dataset

        bool empty() const {
        return Values.empty();
        }

};

#endif