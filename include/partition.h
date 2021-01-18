#pragma once

#include <iostream>
#include <vector>

#include "adjacencyListGraph.h"
#include "csc_matrix.h"
#include "edge.h"
#include "stlHashTable.h"

using namespace std;

class Partition {
public:
  Partition(){};
  ~Partition();
  template <typename T> void from_lower_triangular_matrix(CCSMatrix<T> *matrix);
  void clear();
  void print();

private:
  vector<vector<int>> partitioning;
};

template <typename T>
void Partition::from_lower_triangular_matrix(CCSMatrix<T> *matrix) {
  AdjacencyListGraph<int, int, int>
      dependency_graph; // node matrix col/x elt edge which col it depends on
  for (int j = 0; j < matrix->num_col_get(); j++) {
    // populate nodes
    dependency_graph.insertVertex(j);
  }
  for (int j = 0; j < matrix->num_col_get(); j++) {
    // for every column
    for (int p = matrix->column_pointer_get()[j] + 1;
         p < matrix->column_pointer_get()[j + 1]; p++) {
      // for every elt in the column
      if (matrix->values_get()[p] != 0) {
        // if elt is nonzero, the columns in the row are dependent on current
        // col
        for (int depending_col = j + 1; // next column
             depending_col <
             matrix->row_index_get()[p] + 1; // include diagonal of row
             depending_col++) {
          // until we hit the diagonal
          if (!dependency_graph.containsEdge(j, depending_col)) {
            dependency_graph.insertEdge(j, depending_col, 0, 0);
          }
        }
      }
    }
  }
  // level partitioning
  unsigned int num_partitioned =
      0; // counter to check for circular dependencies
  STLHashTable<int, int> num_neighbors_dict;
  vector<int> partition, new_partition;
  for (unsigned int i = 0; i < dependency_graph.getVertices().size(); i++) {
    // make first partition
    int v = dependency_graph.getVertices()[i];
    vector<Edge<int, int, int>> incoming_edges =
        dependency_graph.getIncomingEdges(v);
    num_neighbors_dict.insert(v, incoming_edges.size());
    if (incoming_edges.size() == 0) {
      partition.push_back(v);
      num_partitioned++;
    }
  }
  if (partition.size() > 0) {
    partitioning.push_back(partition);
  }
  while (partition.size() > 0) {
    vector<int> new_partition;
    for (unsigned int i = 0; i < partition.size(); i++) {
      int v = partition.at(i);
      vector<Edge<int, int, int>> outgoing_edges =
          dependency_graph.getOutgoingEdges(v);
      for (unsigned int j = 0; j < outgoing_edges.size(); j++) {
        int v = outgoing_edges[j].target;
        int new_num_neighbors = num_neighbors_dict.get(v) - 1;
        num_neighbors_dict.update(v, new_num_neighbors);
        if (new_num_neighbors == 0) {
          new_partition.push_back(v);
          num_partitioned++;
        }
      }
    }
    if (new_partition.size() > 0) {
      partitioning.push_back(new_partition);
    }
    partition = new_partition;
  }
  // if we reached here with num_partitioned < num_vertices, we have a circular
  // dependency
  if (num_partitioned < dependency_graph.getVertices().size()) {
    throw runtime_error("Circular dependency found during the partitioning");
  }
}

Partition::~Partition() { this->clear(); }

void Partition::clear() {}

void Partition::print() {
  for (unsigned int i = 0; i < partitioning.size(); i++) {
    cout << "partition " << i << ": ";
    for (unsigned int j = 0; j < partitioning[i].size(); j++) {
      cout << partitioning[i][j] << " ";
    }
    cout << endl;
  }
}