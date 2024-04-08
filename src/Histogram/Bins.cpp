#include "Bin.hpp"

//
Bin::Bin(){
    this->count = 0;
    this->min = 0;
    this->max = 0;
}

//
Bin::Bin(uint64_t count, double min, double max) {
  this->count = count;
  this->min = min;
  this->max = max;
}

//
Bin::Bin(Bin &&bin) {
  this->count = bin.count;
  this->min = bin.min;
  this->max = bin.max;
}

//
Bin::Bin(const Bin &bin) {
  this->count = bin.count;
  this->min = bin.min;
  this->max = bin.max;
}

//
Bin &Bin::operator=(Bin &&bin) {
  this->count = bin.count;
  this->min = bin.min;
  this->max = bin.max;
  return *this;
}

//
Bin &Bin::operator=(const Bin &bin) {
  this->count = bin.count;
  this->min = bin.min;
  this->max = bin.max;
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