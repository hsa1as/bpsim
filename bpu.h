#ifndef LIB_BPU
#define LIB_BPU
#include <cstdint>
#include <vector>

class Statistics{
  uint64_t branches;
  uint64_t misprediction;
  long double mp_rate;
public:
  Statistics();
  void output_stats();
  void finalise_stats();
  void incb();
  void incmp();
};

class BPU{
  // Assume m,n <= 64 ?
  uint64_t m;
  uint64_t n;
  std::vector<uint8_t> counters;
  uint64_t BHR;

public:
  Statistics stat;
  BPU(uint64_t _m, uint64_t _n);
  void predict_branch(uint32_t address, char true_direction);
  void dump_contents();
};
#endif
