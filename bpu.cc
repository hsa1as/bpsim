#include "bpu.h"
using ll = long long;
BPU::BPU(uint64_t _m, uint64_t _n): m(_m), n(_n) {
  // BHR is 0 initially
  this->BHR = 0;
  // Counters initialised to weakly taken
  for(ll i = 0; i < (1<<m); i++){
    this->counters.push_back(2);
  }
}
