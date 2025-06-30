// my_matrices.cpp (or directly in R using Rcpp::cppFunction)
#include <Rcpp.h>
#include <vector> // For your std::vector<std::vector<double>>

// [[Rcpp::export]]
Rcpp::NumericMatrix createAndReturnMatrix() {
  // Example: Create a C++ matrix and convert to Rcpp::NumericMatrix
  std::vector<std::vector<double>> cpp_matrix = {
    {100, 90, 80},
    {90, 80, 70},
    {80, 70, 60}
  };

  int rows = cpp_matrix.size();
  int cols = (rows > 0) ? cpp_matrix[0].size() : 0;

  Rcpp::NumericMatrix r_matrix(rows, cols);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      r_matrix(i, j) = cpp_matrix[i][j];
    }
  }
  return r_matrix;
}

// [[Rcpp::export]]
Rcpp::List createAndReturnManyMatrices() {
  Rcpp::List matrix_list;

  for (int k = 0; k < 3; ++k) { // Example: Create 3 matrices
    int rows = 3;
    int cols = 3;
    Rcpp::NumericMatrix current_r_matrix(rows, cols);

    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        current_r_matrix(i, j) = (double)(i * 10 + j + k * 100); // Example data
      }
    }
    matrix_list.push_back(current_r_matrix, "matrix_" + std::to_string(k)); // Add to list with name
  }
  return matrix_list;
}