#pragma once
#include <string>

using namespace std;

/**
 * A Matrix is an abstract (a.k.a. pure virtual) class specifying the interface
 * for a matrix of an arbitrary data type.
 */
template <typename T> class Matrix {
public:
  virtual ~Matrix(){/* do nothing */};
  virtual void
  from_matrix_market_filename(string const &matrix_market_filepath) = 0;
  virtual void clear() = 0;
  virtual void print() = 0;

  // You can safely ignore the following code.  This eliminates some default
  // class routines, preventing you from using them accidentally.
  // Specifically, we are disabling the use of the copy constructor and the
  // copy assignment operator.  You can read more here:
  //   http://www.cplusplus.com/articles/y8hv0pDG/
public:
  Matrix() {}

private:
  Matrix(const Matrix &other) = delete;
  Matrix &operator=(const Matrix &other) = delete;
};
