#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include "csc_matrix.h"
#include "dense_vector.h"
#include "matrix_operations.h"

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

  DenseVector<double> x2;
  x2.from_dense_vector(&b);
  cout << "loaded files" << endl;
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