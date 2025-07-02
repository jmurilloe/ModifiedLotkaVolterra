#!/usr/bin/env Rscript

suppressPackageStartupMessages(library(bipartite))

args <- commandArgs(trailingOnly = TRUE)
if (length(args) != 3) {
  stop("Debe especificar: modelo N_i N_j")
}

study <- args[1]  # "base", "low", "medium", "high"
N_i <- args[2]
N_j <- args[3]

rep_max <- if (study == "base") 9 else 4

# Vectores para guardar métricas por repetición
vH2 <- numeric(rep_max + 1) 
vmod <- numeric(rep_max + 1) 
vnodf <- numeric(rep_max + 1) 
vconnect <- numeric(rep_max + 1) 
vA <- numeric(rep_max + 1) 
vF <- numeric(rep_max + 1)  

# Leer archivos y calcular métricas
for (rep in 0:rep_max) {
  repcharac <- as.character(rep)
  repname <- file.path(paste0("results_", study), paste0(N_i, "_", N_j), paste0("rep", repcharac, ".txt"))
  lines <- suppressWarnings(readLines(repname))
  features <- as.numeric(strsplit(lines[1], "\\s+")[[1]])
  rest_lines <- lines[-1]
  V <- do.call(rbind, lapply(rest_lines, function(x) as.numeric(strsplit(x, "\\s+")[[1]])))
  ncol_V <- length(features)
  V <- matrix(unlist(V), ncol = ncol_V, byrow = TRUE)

  vH2[rep + 1] <- H2fun(V, H2_integer = FALSE)[1]
  vmod[rep + 1] <- computeModules(V)@likelihood
  vnodf[rep + 1] <- nested(V, method = "WNODA") / 100
  vconnect[rep + 1] <- networklevel(V, index = "weighted connectance")
  vA[rep + 1] <- features[1]
  vF[rep + 1] <- features[2]
}

# Calcular promedios y desviaciones estándar
row_result <- c(
  mean(vH2),    sd(vH2),
  mean(vmod),   sd(vmod),
  mean(vnodf),  sd(vnodf),
  mean(vconnect), sd(vconnect),
  mean(vA),     sd(vA),
  mean(vF),     sd(vF),
  as.numeric(N_i) / as.numeric(N_j)
)

# Guardar en archivo
dir_out <- file.path(paste0("results_", study), paste0(N_i, "_", N_j))
out_file <- file.path(dir_out, paste0("metrics_", N_i, "_", N_j, ".txt"))
write(paste(row_result, collapse = " "), file = out_file, append = TRUE)
