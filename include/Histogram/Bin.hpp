#ifndef BIN_H_
#define BIN_H_

#include <cstdint>
#include <vector>

class Bin {
private:
  uint64_t count;
  double min;
  double max;

public:
  Bin();
  Bin(uint64_t elems, double min, double max);
  ~Bin();

  Bin(Bin &&bin);
  Bin(const Bin &bin);
  Bin &operator=(Bin &&bin);
  Bin &operator=(const Bin &bin);

  uint64_t get_Count() const;
  double get_Min() const;
  double get_Max() const;
};

#endif