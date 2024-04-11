#ifndef BIN_H_
#define BIN_H_

#include <cstdint>
#include <vector>

class Bin {
private:
  double min;
  double max;
  double target_Sum;
  double residual_Sum;
  uint64_t count;

public:
  Bin();
  Bin(double min, double max);
  ~Bin();

  Bin(Bin &&bin);
  Bin(const Bin &bin);
  Bin &operator=(Bin &&bin);
  Bin &operator=(const Bin &bin);

  double get_Min() const;
  double get_Max() const;
  double get_Residual_Sum() const;
  double get_Target() const;
  uint64_t get_Count() const;

  void add_Element(double residual, double target);

  void print();
};

#endif