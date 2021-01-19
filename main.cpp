#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include "csc_matrix.h"
#include "dense_vector.h"
#include "matrix_operations.h"
#include "partition.h"

using namespace std;

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
  L.from_matrix_market_filename(L_filename);
  L.to_lower_triangular();
  L.fill_diag(1);
  DenseVector<long double> b, x1;
  b.from_matrix_market_filename(b_filename);
  x1.from_dense_vector(&b);
  cout << "loaded files" << endl;
  chrono::time_point<chrono::system_clock> start, end;
  start = chrono::system_clock::now();
  lsolve<long double>(L.num_col_get(), L.column_pointer_get(), L.row_index_get(),
                 L.values_get(), x1.values_get());
  end = chrono::system_clock::now();
  chrono::duration<long double> elapsed_seconds = end - start;
  cout << "lower triangular solve completed in " << elapsed_seconds.count()
       << "s" << endl;
  // matrix mult
  DenseVector<long double> y;
  y.from_num_zeros(b.dimension_get());
  spmv_csc(L.num_col_get(), L.column_pointer_get(), L.row_index_get(),
           L.values_get(), x1.values_get(), y.values_get());
  for (int i = 0; i < b.dimension_get(); i++) {
    if (abs(y.values_get()[i] - b.values_get()[i]) > 1e-3) {
      cout << i << " " << y.values_get()[i] << " != " << b.values_get()[i]
           << endl;
    }
  }
  cout << "matrix multiplication verified" << endl;
  // return 0;

  DenseVector<long double> x2;
  x2.from_dense_vector(&b);
  start = chrono::system_clock::now();
  parallel_lsolve<long double>(L.num_col_get(), L.column_pointer_get(),
                          L.row_index_get(), L.values_get(), x2.values_get());
  end = chrono::system_clock::now();
  elapsed_seconds = end - start;
  cout << "parallel lower triangular solve completed in "
       << elapsed_seconds.count() << "s" << endl;
  // verify
  for (int i = 0; i < x1.dimension_get(); i++) {
    if (abs(x1.values_get()[i] - x2.values_get()[i]) > 1e-3) {
      cout << i << " " << x1.values_get()[i] << " != " << x2.values_get()[i]
           << endl;
    }
  }
  cout << "verified" << endl;

  x2.clear();
  x2.from_dense_vector(&b);
  Partition partition;
  partition.from_lower_triangular_matrix<long double>(&L);
  start = chrono::system_clock::now();
  partitioned_parallel_lsolve<long double>(L.column_pointer_get(), L.row_index_get(),
                                      L.values_get(), x2.values_get(),
                                      partition.partitioning_get());
  end = chrono::system_clock::now();
  elapsed_seconds = end - start;
  cout << "partitioned parallel lower triangular solve completed in "
       << elapsed_seconds.count() << "s" << endl;
  // verify
  for (int i = 0; i < x1.dimension_get(); i++) {
    if (abs(x1.values_get()[i] - x2.values_get()[i]) > 1e-3) {
      cout << i << " " << x1.values_get()[i] << " != " << x2.values_get()[i]
           << endl;
    }
  }
  cout << "verified" << endl;
  // matrix mult
  // DenseVector<long double> y;
  y.clear();
  y.from_num_zeros(b.dimension_get());
  spmv_csc<long double>(L.num_col_get(), L.column_pointer_get(), L.row_index_get(),
                   L.values_get(), x2.values_get(), y.values_get());
  for (int i = 0; i < b.dimension_get(); i++) {
    if (abs(y.values_get()[i] - b.values_get()[i]) > 1e-3) {
      cout << i << " " << y.values_get()[i] << " != " << b.values_get()[i]
           << endl;
    }
  }
  cout << "matrix multiplication verified" << endl;
  return 0;
}