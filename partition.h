#pragma once

#include <iostream>
#include <vector>
#include "boost/libs/"

using namespace std;

class Partition {
public:
  Partition(){};
  ~Partition();
  void from_lower_triangular_matrix();
  void clear();
  void print();

private:
  vector<int> *partition;
};

void from_lower_triangular_matrix(){

}

Partition::~Partition() { this->clear(); }

void Partition::clear() {
  delete[] partition;
}

void Partition::print() {}