#include "Bin.hpp"

//
Bin::Bin() {
  this->count = 0;
  this->min = 0;
  this->max = 0;
  this->statistic = 0;
}

//
Bin::Bin(double min, double max) {
  this->count = count;
  this->statistic = statistic;
  this->min = min;
  this->max = max;
}

//
Bin::Bin(Bin &&bin) {
  this->count = bin.count;
  this->min = bin.min;
  this->max = bin.max;
  this->statistic = bin.statistic;
}

//
Bin::Bin(const Bin &bin) {
  this->count = bin.count;
  this->min = bin.min;
  this->max = bin.max;
  this->statistic = bin.statistic;
}

//
Bin &Bin::operator=(Bin &&bin) {
  this->count = bin.count;
  this->min = bin.min;
  this->max = bin.max;
  this->statistic = bin.statistic;
  return *this;
}

//
Bin &Bin::operator=(const Bin &bin) {
  this->count = bin.count;
  this->min = bin.min;
  this->max = bin.max;
  this->statistic = bin.statistic;
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
double Bin::get_Statistic() const { return this->statistic; }

//
void Bin::set_Count(uint64_t new_Count) { this->count = new_Count; }

//
void Bin::set_Statistic(double new_Statistic) {
  this->statistic = new_Statistic;
}