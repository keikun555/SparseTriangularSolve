/**
 * Kei Imada
 * 20210120
 * Sparse Lower Triangular Solve
 */

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include "ccs_matrix.h"
#include "dense_vector.h"
#include "matrix_operations.h"
#include "partition.h"

using namespace std;

float THRESHOLD = 1e-5;

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr,
            "Usage: %s [martix-market-L-filename] [martix-market-b-filename]\n",
            argv[0]);
    exit(1);
  }

  string L_filename(argv[1]);
  string b_filename(argv[2]);

  CCSMatrix<long double> L;
  L.from_matrix_market_filepath(L_filename);
  L.to_lower_triangular();
  // L.fill_diag(1, true); // was initially not sure if the diagonals should be
  // filled in
  DenseVector<long double> b, target_x;
  b.from_matrix_market_filepath(b_filename);
  target_x.from_dense_vector(&b);

  // original
  chrono::time_point<chrono::system_clock> start, end;
  start = chrono::system_clock::now();
  lsolve<long double>(&L, &target_x);
  end = chrono::system_clock::now();
  chrono::duration<long double> elapsed_seconds = end - start;
  cout << elapsed_seconds.count();
  // matrix multiplication to verify the original algorithm
  DenseVector<long double> y;
  y.from_num_zeros(b.dimension_get());
  spmv_ccs(&L, &target_x, &y);
  if (!y.approx_equals(&b, THRESHOLD)) {
    /**
     * even if the matrix multiplication verification fails, we are measuring
     * the optimizations made to the original algorithm, hence we only check if
     * the output of the optimizations match the output of the original
     * algorithm
     */
    cerr << "matrix multiplication verification failed\n"
            "  the original lower triangular solve algorithm failed to solve "
            "this matrix"
         << endl;
  }

  // naive parallelization
  DenseVector<long double> x;
  x.from_dense_vector(&b);
  start = chrono::system_clock::now();
  parallel_lsolve<long double>(&L, &x);
  end = chrono::system_clock::now();
  cout << ",";
  // verify
  if (x.approx_equals(&target_x, THRESHOLD)) {
    elapsed_seconds = end - start;
    cout << elapsed_seconds.count();
  } else {
    cerr << "parallel lower triangular solve verification failed, output does "
            "not match original algorithm"
         << endl;
  }

  // level set partitioning parallelization
  x.clear();
  x.from_dense_vector(&b);
  Partition partition;
  partition.from_lower_triangular_matrix<long double>(&L);
  start = chrono::system_clock::now();
  partitioned_parallel_lsolve<long double>(&L, &x, &partition);
  end = chrono::system_clock::now();
  cout << ",";
  // verify
  if (x.approx_equals(&target_x, THRESHOLD)) {
    elapsed_seconds = end - start;
    cout << elapsed_seconds.count();
  } else {
    cerr << "level partitioned parallel lower triangular solve verification "
            "failed, output does not match original algorithm"
         << endl;
  }
  cout << endl;
  return 0;
}