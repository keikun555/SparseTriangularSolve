#pragma once

#include <iostream>
#include <omp.h>
#include <vector>

#include "ccs_matrix.h"
#include "dense_vector.h"
#include "partition.h"

/*
 * Lower triangular solver Lx=b
 * partitioning : the partitioning for parallel computation, if any
 * input_vector : the right hand-side b at start and the solution x at the end.
 */
template <typename T>
int lsolve(CCSMatrix<T> *matrix, DenseVector<T> *input_vector);
template <typename T>
int parallel_lsolve(CCSMatrix<T> *matrix, DenseVector<T> *input_vector);
template <typename T>
int partitioned_parallel_lsolve(CCSMatrix<T> *matrix, T *x,
                                Partition *partition);

/*
 * Sparse matrix-vector multiply: y = A*x */
template <typename T>
int spmv_ccs(CCSMatrix<T> *matrix, DenseVector<T> *input_vector,
             DenseVector<T> *output_vector);
template <typename T>
int parallel_spmv_ccs(CCSMatrix<T> *matrix, DenseVector<T> *input_vector,
                      DenseVector<T> *output_vector);

/* implementations */
using namespace std;

template <typename T>
int lsolve(CCSMatrix<T> *matrix, DenseVector<T> *input_vector) {
  if (!matrix || !input_vector) {
    return 1;
  }
  int n = matrix->num_col_get();
  int *Lp = matrix->column_pointer_get();
  int *Li = matrix->row_index_get();
  T *Lx = matrix->values_get();
  T *x = input_vector->values_get();
  int p, j;
  if (!Lp || !Li || !x) {
    return 1;               /* check inputs */
  }
  for (j = 0; j < n; j++) { // for every col in L, row in x
    x[j] /= Lx[Lp[j]];      // hopefully the diagonals are nonzero
    for (p = Lp[j] + 1; p < Lp[j + 1]; p++) {
      x[Li[p]] -= Lx[p] * x[j];
    }
  }
  return 0;
}

template <typename T>
int parallel_lsolve(CCSMatrix<T> *matrix, DenseVector<T> *input_vector) {
  if (!matrix || !input_vector) {
    return 1;
  }
  int n = matrix->num_col_get();
  int *Lp = matrix->column_pointer_get();
  int *Li = matrix->row_index_get();
  T *Lx = matrix->values_get();
  T *x = input_vector->values_get();
  int p, j;
  if (!Lp || !Li || !x){
    return 1;               /* check inputs */
  }
  for (j = 0; j < n; j++) { // for every col in L, row in x
    x[j] /= Lx[Lp[j]];      // hopefully the diagonals are nonzero
#pragma omp parallel default(shared) private(p)
#pragma omp for
    for (p = Lp[j] + 1; p < Lp[j + 1]; p++) {
      x[Li[p]] -= Lx[p] * x[j];
    }
  }
  return 0;
}

template <typename T>
int partitioned_parallel_lsolve(CCSMatrix<T> *matrix,
                                DenseVector<T> *input_vector,
                                Partition *partition) {
  if (!matrix || !input_vector || !partition) {
    return 1;
  }
  int *Lp = matrix->column_pointer_get();
  int *Li = matrix->row_index_get();
  T *Lx = matrix->values_get();
  T *x = input_vector->values_get();
  vector<vector<int>> partitioning = partition->partitioning_get();
  if (!Lp || !Li || !x){
    return 1; /* check inputs */
  }
  for (unsigned int i = 0; i < partitioning.size(); i++) {
    vector<int> partition = partitioning[i];
#pragma omp parallel default(shared)
    {
#pragma omp for
      for (unsigned int part_idx = 0; part_idx < partition.size(); part_idx++) {
        int j = partition[part_idx];
        x[j] /= Lx[Lp[j]]; // hopefully the diagonals are nonzero
        for (int p = Lp[j] + 1; p < Lp[j + 1]; p++) {
          double tmp = Lx[p] * x[j];
          int x_idx = Li[p];
#pragma omp atomic
          x[x_idx] -= tmp;
        }
      }
    }
  }
  return 0;
}

template <typename T>
int spmv_ccs(CCSMatrix<T> *matrix, DenseVector<T> *input_vector,
             DenseVector<T> *output_vector) {
  if (!matrix || !input_vector || !output_vector) {
    return 1;
  }
  int n = matrix->num_col_get();
  int *Ap = matrix->column_pointer_get();
  int *Ai = matrix->row_index_get();
  T *Ax = matrix->values_get();
  T *x = input_vector->values_get();
  T *y = output_vector->values_get();
  int p, j;
  if (!Ap || !x || !y)
    return 1; /* check inputs */
  for (j = 0; j < n; j++) {
    for (p = Ap[j]; p < Ap[j + 1]; p++) {
      y[Ai[p]] += Ax[p] * x[j];
    }
  }
  return 0;
}

template <typename T>
int parallel_spmv_ccs(CCSMatrix<T> *matrix, DenseVector<T> *input_vector,
                      DenseVector<T> *output_vector) {
  if (!matrix || !input_vector || !output_vector) {
    return 1;
  }
  int n = matrix->num_col_get();
  int *Ap = matrix->column_pointer_get();
  int *Ai = matrix->row_index_get();
  T *Ax = matrix->values_get();
  T *x = input_vector->values_get();
  T *y = output_vector->values_get();
  int p, j;
  if (!Ap || !x || !y) {
    return 1; /* check inputs */
  }
  for (j = 0; j < n; j++) {
    for (p = Ap[j]; p < Ap[j + 1]; p++) {
      y[Ai[p]] += Ax[p] * x[j];
    }
  }
  return 0;
}