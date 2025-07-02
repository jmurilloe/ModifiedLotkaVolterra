#!/usr/bin/env Rscript

suppressPackageStartupMessages(library(bipartite, quietly = TRUE, warn.conflicts = FALSE))

args <- commandArgs(trailingOnly = TRUE)
if (length(args) != 1) {
  stop("Debe especificarse un archivo de entrada.")
}
study <- args[1] # ex base
N_i <- args[2] # ex 18
N_j <- args[3] # ex 30
total_features

for(rep in 0:9){
  inforep <- as.matrix(read.table(file, sep = " "))
  repcharac <- as.character(rep)
  Vname <- paste("results_"+study+"/"+N_i+"_"+N_j+"/V"+repcharac+".txt")
  featuresname <- paste("results_"+study+"/"+N_i+"_"+N_j+"/features"+repcharac+".txt")

  # Leer matriz y features
  V <- as.matrix(read.table(Vname, sep = " "))
  features <- as.matrix(read.table(featuresname, sep = "\t"))

  # Calcular métricas
  H2 <- H2fun(V, H2_integer = FALSE)[1]
  mod <- computeModules(V)@likelihood
  nodf <- nested(V, method = "WNODA")
  connect <- networklevel(V, index = "weighted connectance")

}

# Extraer directorio contenedor (e.g., results/6_13)
dir <- dirname(file)
out_file <- file.path(dir, "metricas.txt")

# Guardar fila en archivo correspondiente
cat(H2, mod, nodf, connect, sep = " ", file = out_file, append = TRUE)
cat("\n", file = out_file, append = TRUE)
