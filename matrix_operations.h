#pragma once

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
int lsolve(int n, int *Lp, int *Li, double *Lx, double *x);
int parallel_lsolve(int n, int *Lp, int *Li, double *Lx, double *x);

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
int spmv_csc(int n, int *Ap, int *Ai, double *Ax, double *x, double *y);
int parallel_spmv_csc(int n, int *Ap, int *Ai, double *Ax, double *x, double *y);