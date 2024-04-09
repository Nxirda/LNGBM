#ifndef BIN_H_
#define BIN_H_

#include <cstdint>
#include <vector>

class Bin {
private:
  double min;
  double max;
  uint64_t count;

public:
  Bin();
  Bin(uint64_t elems, double min, double max);
  ~Bin();

  Bin(Bin &&bin);
  Bin(const Bin &bin);
  Bin &operator=(Bin &&bin);
  Bin &operator=(const Bin &bin);

  double get_Min() const;
  double get_Max() const;
  uint64_t get_Count() const;
};

#endif