#!/usr/bin/env Rscript

suppressPackageStartupMessages(library(bipartite, quietly = TRUE, warn.conflicts = FALSE))

args <- commandArgs(trailingOnly = TRUE)
if (length(args) != 1) {
  stop("Debe especificarse un archivo de entrada.")
}
file <- args[1]

# Leer matriz
V <- as.matrix(read.table(file, sep = " "))

# Calcular métricas
H2 <- H2fun(V, H2_integer = FALSE)[1]
mod <- computeModules(V)@likelihood
nodf <- nested(V, method = "WNODA")
connect <- networklevel(V, index = "weighted connectance")

# Extraer directorio contenedor (e.g., results/6_13)
dir <- dirname(file)
out_file <- file.path(dir, "metricas.txt")

# Guardar fila en archivo correspondiente
cat(H2, mod, nodf, connect, sep = " ", file = out_file, append = TRUE)
cat("\n", file = out_file, append = TRUE)
