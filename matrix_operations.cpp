#include <iostream>
#include <omp.h>
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
int lsolve(int n, int *Lp, int *Li, double *Lx, double *x) {
  int p, j;
  if (!Lp || !Li || !x)
    return 1;               /* check inputs */
  for (j = 0; j < n; j++) { // for every col in L, row in x
    x[j] /= Lx[Lp[j]];      // hopefully the diagonals are nonzero
    for (p = Lp[j] + 1; p < Lp[j + 1]; p++) {
      x[Li[p]] -= Lx[p] * x[Li[Lp[j]]];
    }
  }
  return 0;
}

int parallel_lsolve(int n, int *Lp, int *Li, double *Lx, double *x) {
  int p, j;
  if (!Lp || !Li || !x)
    return 1;               /* check inputs */
  for (j = 0; j < n; j++) { // for every col in L, row in x
    x[j] /= Lx[Lp[j]];      // hopefully the diagonals are nonzero
#pragma omp parallel
#pragma omp for
    for (p = Lp[j] + 1; p < Lp[j + 1]; p++) {
      x[Li[p]] -= Lx[p] * x[Li[Lp[j]]];
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
int spmv_csc(int n, int *Ap, int *Ai, double *Ax, double *x, double *y) {
  int p, j;
  if (!Ap || !x || !y)
    return 1; /* check inputs */
  for (j = 0; j < n; j++) {
    for (p = Ap[j]; p < Ap[j + 1]; p++) {
      y[Ai[p]] += Ax[p] * x[Ai[Ap[j]]];
    }
  }
  return 0;
}
int parallel_spmv_csc(int n, int *Ap, int *Ai, double *Ax, double *x,
                      double *y) {
  int p, j;
  if (!Ap || !x || !y)
    return 1; /* check inputs */
  for (j = 0; j < n; j++) {
    for (p = Ap[j]; p < Ap[j + 1]; p++) {
      y[Ai[p]] += Ax[p] * x[Ai[Ap[j]]];
    }
  }
  return 0;
}