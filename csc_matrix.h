#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "include/matrix.h"

using namespace std;

template <typename T> class CCSMatrix : public Matrix<T> {
public:
  CCSMatrix(){};
  ~CCSMatrix();
  void from_matrix_market_filename(string const &matrix_market_filename);
  void to_lower_triangular();
  void clear();
  void print();
  int num_row_get() { return num_row; };
  int num_col_get() { return num_col; };
  int num_val_get() { return num_val; };
  int *column_pointer_get() { return column_pointer; };
  int *row_index_get() { return row_index; };
  T *values_get() { return values; };

private:
  int num_row = 0;
  int num_col = 0;
  int num_val = 0;
  int *column_pointer = nullptr;
  int *row_index = nullptr;
  T *values = nullptr;
};

template <typename T>
void CCSMatrix<T>::from_matrix_market_filename(
    string const &matrix_market_filename) {
  ifstream file(matrix_market_filename);

  // Ignore comments headers
  while (file.peek() == '%')
    file.ignore(2048, '\n');

  // Read number of rows and columns
  file >> num_row >> num_col >> num_val;

  column_pointer = new int[num_col + 1]; // the starter algorithm dictates this
  row_index = new int[num_val];
  values = new T[num_val];
  // fill the matrix with data
  int cur_col_idx = 0;
  for (int l = 0; l < num_val; l++) {
    T data;
    int row, col;
    file >> row >> col >> data;
    values[l] = data;
    row_index[l] = row - 1;
    while (cur_col_idx < num_col &&
           cur_col_idx < col) { // account for empty columns
      column_pointer[cur_col_idx++] = l;
    }
  }
  while (cur_col_idx < num_col + 1) { // account for empty columns
    column_pointer[cur_col_idx++] = num_val;
  }

  file.close();
}

template <typename T> void CCSMatrix<T>::to_lower_triangular() {
  num_val = 0;
  int val_idx = 0;
  for (int j = 0; j < num_col; j++) {
    // for every column
    while (row_index[val_idx] < j) {
      // skip non lower triangular part of column
      val_idx++;
    }
    column_pointer[j] = num_val;
    while (val_idx < column_pointer[j + 1]) {
      // for every elt in the lower triangular part of column
      values[num_val] = values[val_idx];
      row_index[num_val] = row_index[val_idx];
      val_idx++;
      num_val++;
    }
  }
  column_pointer[num_col] = num_val;
}

template <typename T> CCSMatrix<T>::~CCSMatrix() { this->clear(); }

template <typename T> void CCSMatrix<T>::clear() {
  num_row = 0;
  num_col = 0;
  num_val = 0;
  delete[] column_pointer;
  delete[] row_index;
  delete[] values;
}

template <typename T> void CCSMatrix<T>::print() {
  cout << "CSCMatrix" << endl;
  cout << "  num_row:        " << num_row << endl;
  cout << "  num_col:        " << num_col << endl;
  cout << "  num_val:        " << num_val << endl;
  cout << "  column pointer: ";
  for (int i = 0; i < num_col + 1; i++) {
    cout << column_pointer[i] << " ";
  }
  cout << endl;
  cout << "  row index:      ";
  for (int i = 0; i < num_val; i++) {
    cout << row_index[i] << " ";
  }
  cout << endl;
  cout << "  values:         ";
  for (int i = 0; i < num_val; i++) {
    if (values[i] != 0)
      cout << "(" << i << ", " << values[i] << ") ";
  }
  cout << endl;
  return;
}