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

  CCSMatrix<double> L;
  L.from_matrix_market_filename(L_filename);
  L.to_lower_triangular();
  L.fill_diag(1);
  DenseVector<double> b, x1;
  b.from_matrix_market_filename(b_filename);
  x1.from_dense_vector(&b);
  cout << "loaded files" << endl;
  chrono::time_point<chrono::system_clock> start, end;
  start = chrono::system_clock::now();
  lsolve(L.num_col_get(), L.column_pointer_get(), L.row_index_get(),
         L.values_get(), x1.values_get());
  end = chrono::system_clock::now();
  chrono::duration<double> elapsed_seconds = end - start;
  cout << "lower triangular solve completed in " << elapsed_seconds.count()
       << "s" << endl;
  // matrix mult
  // DenseVector<double> y;
  // y.from_num_zeros(b.dimension_get());
  // spmv_csc(L.num_col_get(), L.column_pointer_get(), L.row_index_get(),
  //          L.values_get(), x1.values_get(), y.values_get());
  // for (int i = 0; i < b.dimension_get(); i++) {
  //   if (abs(y.values_get()[i] - b.values_get()[i]) > 1e-10) {
  //     cout << i << " " << y.values_get()[i] << " != " << b.values_get()[i]
  //          << endl;
  //   }
  // }
  // cout << "matrix multiplication verified" << endl;

  DenseVector<double> x2;
  x2.from_dense_vector(&b);
  start = chrono::system_clock::now();
  parallel_lsolve(L.num_col_get(), L.column_pointer_get(), L.row_index_get(),
                  L.values_get(), x2.values_get());
  end = chrono::system_clock::now();
  elapsed_seconds = end - start;
  cout << "parallel lower triangular solve completed in "
       << elapsed_seconds.count() << "s" << endl;
  // verify
  for (int i = 0; i < x1.dimension_get(); i++) {
    if (x1.values_get()[i] != x2.values_get()[i]) {
      cout << i << " " << x1.values_get()[i] << " != " << x2.values_get()[i]
           << endl;
    }
  }
  cout << "verified" << endl;

  x2.clear();
  x2.from_dense_vector(&b);
  Partition partition;
  partition.from_lower_triangular_matrix<double>(&L);
  partition.print();
  return 0;
  start = chrono::system_clock::now();
  parallel_lsolve(L.num_col_get(), L.column_pointer_get(), L.row_index_get(),
                  L.values_get(), x2.values_get());
  end = chrono::system_clock::now();
  elapsed_seconds = end - start;
  cout << "parallel lower triangular solve completed in "
       << elapsed_seconds.count() << "s" << endl;
  // verify
  for (int i = 0; i < x1.dimension_get(); i++) {
    if (x1.values_get()[i] != x2.values_get()[i]) {
      cout << i << " " << x1.values_get()[i] << " != " << x2.values_get()[i]
           << endl;
    }
  }
  cout << "verified" << endl;
  return 0;
}