suppressPackageStartupMessages(library(bipartite, quietly = TRUE, warn.conflicts = FALSE))
V <- as.matrix(read.table("data.txt", sep=" "))
H2fun(V, H2_integer = FALSE)[1]
computeModules(V)@likelihood 
nested(V, method = "WNODA")
networklevel(V,index = "weighted connectance")