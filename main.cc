#include <iostream>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include "bpu.h"
using namespace std;
using ll = long long;

void start_sim(string filename, ll n, ll m){
  ifstream instream(filename);
  string direction;
  string pc_s;
  instream >> pc_s >> direction;
  uint32_t pc = strtol(pc_s.c_str(), NULL, 16);
  char d = direction[0];
}

int main(int argc, char **argv){
 
  ll m,n;
  string filename;
  // Check predictor type:
  // Bimodal type: argc == 4, gshare: argc == 5
  if(argc == 4){
    m = stoi(argv[2]);
    n = 0;
    filename = stoi(argv[3]);
  }else if (argc == 5){
    m = stoi(argv[2]);
    n = stoi(argv[3]);
    filename = stoi(argv[4]);
  }else{
    cout<<"Wrong number of arguments specified";
    exit(1);
  }

  start_sim(filename, m, n);
  return 0; 
  
}
