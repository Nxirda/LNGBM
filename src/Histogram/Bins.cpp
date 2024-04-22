#include "Bin.hpp"
#include <iostream>

//
Bin::Bin() {
  this->count = 0;
  this->min = 0;
  this->max = 0;
  this->residual_Sum = 0;
}

//
Bin::Bin(double min, double max) {
  this->count = 0;
  this->residual_Sum = 0;
  this->min = min;
  this->max = max;
}

//
Bin::Bin(Bin &&bin) {
  this->count = bin.count;
  this->min = bin.min;
  this->max = bin.max;
  this->residual_Sum = bin.residual_Sum;
}

//
Bin::Bin(const Bin &bin) {
  this->count = bin.count;
  this->min = bin.min;
  this->max = bin.max;
  this->residual_Sum = bin.residual_Sum;
}

//
Bin &Bin::operator=(Bin &&bin) {
  this->count = bin.count;
  this->min = bin.min;
  this->max = bin.max;
  this->residual_Sum = bin.residual_Sum;
  return *this;
}

//
Bin &Bin::operator=(const Bin &bin) {
  this->count = bin.count;
  this->min = bin.min;
  this->max = bin.max;
  this->residual_Sum = bin.residual_Sum;
  return *this;
}

//
Bin::~Bin(){};

//
uint64_t Bin::get_Count() const { return this->count; }

//
double Bin::get_Min() const { return this->min; }

//
double Bin::get_Max() const { return this->max; }

//
double Bin::get_Residual_Sum() const { return this->residual_Sum; }

//
void Bin::add_Element(double residual) {
  this->residual_Sum += residual;
  this->count += 1;
}

//
void Bin::print() const {
  std::cout << "Residual Sum " << residual_Sum << " count " << count
            << std::endl;
}