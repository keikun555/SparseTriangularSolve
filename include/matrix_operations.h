#pragma once

#include <iostream>
#include <omp.h>
#include <vector>

/*
 * Lower triangular solver Lx=b
 * L is stored in the compressed column storage format
 * Inputs are:
 * n : the matrix dimension
 * Lp : the column pointer of L
 * Li : the row index of L
 * Lx : the values of L
 * In/Out:
 * x : the right hand-side b at start and the solution x at the end. */
template <typename T> int lsolve(int n, int *Lp, int *Li, T *Lx, T *x);
template <typename T> int parallel_lsolve(int n, int *Lp, int *Li, T *Lx, T *x);
template <typename T>
int partitioned_parallel_lsolve(int *Lp, int *Li, T *Lx, T *x,
                                vector<vector<int>> partition);

/*
 * Sparse matrix-vector multiply: y = A*x
 * A is stored in the compressed column storage format
 * Inputs:
 * Ap : the column pointer of A
 * Ai : the row index of A
 * Ax : the values of A
 * x : is a dense vector
 * Output:
 * y : is a dense vector that stores the result of multiplication */
template <typename T> int spmv_csc(int n, int *Ap, int *Ai, T *Ax, T *x, T *y);
template <typename T>
int parallel_spmv_csc(int n, int *Ap, int *Ai, T *Ax, T *x, T *y);

using namespace std;
/*
 * Lower triangular solver Lx=b
 * L is stored in the compressed column storage format
 * Inputs are:
 * n : the matrix dimension (number of columns)
 * Lp : the column pointer of L
 * Li : the row index of L
 * Lx : the values of L
 * In/Out:
 * x : the right hand-side b at start and the solution x at the end. */
template <typename T> int lsolve(int n, int *Lp, int *Li, T *Lx, T *x) {
  int p, j;
  if (!Lp || !Li || !x)
    return 1;               /* check inputs */
  for (j = 0; j < n; j++) { // for every col in L, row in x
    x[j] /= Lx[Lp[j]];      // hopefully the diagonals are nonzero
    for (p = Lp[j] + 1; p < Lp[j + 1]; p++) {
      x[Li[p]] -= Lx[p] * x[j];
    }
  }
  return 0;
}

template <typename T>
int parallel_lsolve(int n, int *Lp, int *Li, T *Lx, T *x) {
  int p, j;
  if (!Lp || !Li || !x)
    return 1;               /* check inputs */
  for (j = 0; j < n; j++) { // for every col in L, row in x
    x[j] /= Lx[Lp[j]];      // hopefully the diagonals are nonzero
#pragma omp parallel shared(Lp, Li, Lx, x, j) private(p)
#pragma omp for
    for (p = Lp[j] + 1; p < Lp[j + 1]; p++) {
      x[Li[p]] -= Lx[p] * x[j];
    }
  }
  return 0;
}

template <typename T>
int partitioned_parallel_lsolve(int *Lp, int *Li, T *Lx, T *x,
                                vector<vector<int>> partitioning) {
  int p, j;
  if (!Lp || !Li || !x)
    return 1; /* check inputs */
  for (unsigned int i = 0; i < partitioning.size(); i++) {
    vector<int> partition = partitioning[i];
#pragma omp parallel shared(Lp, Li, Lx, x, i) private(p, j)
#pragma omp for
    for (unsigned int part_idx = 0; part_idx < partition.size(); part_idx++) {
      j = partition[part_idx];
      x[j] /= Lx[Lp[j]]; // hopefully the diagonals are nonzero
      for (p = Lp[j] + 1; p < Lp[j + 1]; p++) {
        x[Li[p]] -= Lx[p] * x[j];
      }
    }
  }
  return 0;
}

/*
 * Sparse matrix-vector multiply: y = A*x
 * A is stored in the compressed column storage format
 * Inputs:
 * Ap : the column pointer of A
 * Ai : the row index of A
 * Ax : the values of A
 * x : is a dense vector
 * Output:
 * y : is a dense vector that stores the result of multiplication */
template <typename T> int spmv_csc(int n, int *Ap, int *Ai, T *Ax, T *x, T *y) {
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
int parallel_spmv_csc(int n, int *Ap, int *Ai, T *Ax, T *x, T *y) {
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