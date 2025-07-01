// my_r_app.cpp
#include <RInside.h> // Necessary for embedding R
#include <Rcpp.h> // Necessary for embedding R
#include <iostream>  // For std::cout
#include <string>    // For std::string (good practice to include explicitly)

// You might have other functions defined here, e.g.:
// [[Rcpp::export]]
// Rcpp::NumericVector myFunction(Rcpp::NumericVector x) { ... }

int main(int argc, char *argv[]) { // This is the required main function

    // Initialize R interpreter
    RInside R(argc, argv);

    // --- Your RInside code goes here ---
    // Example:
    R.parseEvalQ("my_r_vector <- c(1.0, 2.0, 3.0, 4.0, 5.0)");

    // FIX 1: Use Rcpp::as<std::string>(...) for the string conversion
    // You need to explicitly convert the R object returned by parseEval
    // before using it with std::cout.
    std::string vector_str = Rcpp::as<std::string>(R.parseEval("my_r_vector"));
    std::cout << "Vector R created: my_r_vector = " << vector_str << std::endl;


    Rcpp::RObject result = R.parseEval("mean(my_r_vector)");

    // FIX 2: Use Rcpp::as<double>(...) for the double conversion
    double r_mean = Rcpp::as<double>(result);
    std::cout << "The mean (calculated in R) is: " << r_mean << std::endl;
    // --- End of your RInside code ---

    return 0; // Standard exit code for successful execution
}