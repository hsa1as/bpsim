#include "bpu.h"
#include <iomanip>
#include <iostream>
using ll = long long;
BPU::BPU(uint64_t _m, uint64_t _n): m(_m), n(_n) {
  // BHR is 0 initially
  this->BHR = 0;
  // Counters initialised to weakly taken
  for(ll i = 0; i < (1<<m); i++){
    this->counters.push_back(2);
  }
}

void BPU::predict_branch(uint32_t pc, char true_direction){
  // Statistics
  this->stat.incb();
  // Calculate the index into counters
  // Discard least significant 2 bits of pc
  uint32_t index = pc >> 2;
  // XOR with BHR if n != 0
  if(n != 0){
    index = index ^ ((this->BHR << (this->m - this-> n)));
  }

  // predict step
  uint8_t prediction = this->counters[index];
  
  // Predict taken
  if((prediction == 2 || prediction == 3) && true_direction != 't'){
    this->stat.incmp();
  }
  // Predict not taken
  if((prediction == 1 || prediction == 0) && true_direction != 'n'){
    this->stat.incmp();
  }

  // Update step
  // update counters
  if(true_direction == 't'){
    // 00 -> 01
    // 01 -> 10
    // 10 -> 11
    // 11 -> 11
    if(this->counters[index] != 3){
      this->counters[index] = (this->counters[index] + 1) & 0b11;
    }
  }else{
    // 00 -> 00
    // 01 -> 00
    // 10 -> 01
    // 11 -> 10
    if(this->counters[index] != 0){
      this->counters[index] = (this->counters[index] - 1) & 0b11;
    }
  }
  // update BHR
  if(true_direction == 't'){
    // +1 if not max
    if(this->BHR != ((1<<(n)) - 1)){
      this->BHR = (this->BHR + 1) & ((1<<(n)) - 1);
    }
  }else{
    // -1 if not 0
    if(this->BHR != 0){
      this->BHR = (this->BHR - 1) & ((1<<(n)) - 1);
    }

  }

}

void BPU::dump_contents(){
  std::cout<<"FINAL ";
  if(this->n != 0) std::cout<<"GSHARE CONTENTS";
  else std::cout<<"BIMODAL CONTENTS";
  std::cout<<std::endl;
  for(ll i = 0; i < (1<<this->n); i++){
    std::cout<<i<<" "<<this->counters[i]<<std::endl;
  }
}

Statistics::Statistics(){
  this->branches = 0;
  this->misprediction = 0;
  this->mp_rate = (double)(0);
}

void Statistics::incb(){
  this->branches++;
}

void Statistics::incmp(){
  this->misprediction++;
}

void Statistics::finalise_stats(){
  this->mp_rate = ((double)(this->misprediction)) / this->branches;
}

void Statistics::output_stats(){
  this->finalise_stats();
  std::cout<<std::fixed<<std::setprecision(2);
  std::cout<<"number of predictions: "<<this->branches<<std::endl;
  std::cout<<"number of mispredictions: "<<this->misprediction<<std::endl;
  std::cout<<"misprediction rate: "<<this->mp_rate<<"%"<<std::endl;
}
