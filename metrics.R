suppressPackageStartupMessages(library(bipartite, quietly = TRUE, warn.conflicts = FALSE))
V <- matrix(c(
    100, 90, 80,
    90, 80, 70,
    80, 70, 60,
    70, 60, 50,
    60, 50, 40,
    50, 40, 0,
    40, 0, 0,
    0, 1, 1,
    0, 0, 1
), nrow = 9, byrow = TRUE)
H2fun(V, H2_integer = FALSE)[1]
computeModules(V)@likelihood 
nested(V, method = "WNODA")
networklevel(V,index = "weighted connectance")