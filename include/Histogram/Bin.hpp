#ifndef BIN_H_
#define BIN_H_

#include <cstdint>
#include <vector>

class Bin {
private:
  double min;
  double max;
  double statistic;
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
  double get_Statistic() const;
  uint64_t get_Count() const;

  void set_Count(uint64_t new_Count);
  void set_Statistic(double new_Statistic);
};

#endif