#!/usr/bin/env Rscript

suppressPackageStartupMessages({
  library(bipartite, quietly = TRUE, warn.conflicts = FALSE)
})

# Leer argumentos
args <- commandArgs(trailingOnly = TRUE)
if (length(args) < 3) {
  stop("Uso: metrics.R [base|low|medium|high] N_i N_j")
}

study <- args[1]  # base, low, medium, high
N_i <- as.numeric(args[2])
N_j <- as.numeric(args[3])
rep_max <- if (study == "base") 9 else 4

# Directorio del par
dir_path <- file.path(paste0("results_", study), paste0(N_i, "_", N_j))

# Vectores para almacenar métricas
vH2 <- numeric()
vmod <- numeric()
vnodf <- numeric()
vconnect <- numeric()
vA <- numeric()
vF <- numeric()

# Procesar cada repetición
for (rep in 0:rep_max) {
  rep_file <- file.path(dir_path, paste0("rep", rep, ".txt"))

  if (!file.exists(rep_file)) {
    warning(paste("Archivo no encontrado:", rep_file))
    next
  }

  lines <- suppressWarnings(readLines(rep_file))
  if (length(lines) < 2) {
    warning(paste("Archivo vacío o malformado:", rep_file))
    next
  }

  # Leer features y matriz V
  features <- as.numeric(strsplit(lines[1], "\\s+")[[1]])
  if (length(features) != 2 || any(is.na(features))) {
    warning(paste("Features inválidos en:", rep_file))
    next
  }

  rest_lines <- lines[-1]
  V_raw <- lapply(rest_lines, function(x) as.numeric(strsplit(x, "\\s+")[[1]]))
  if (any(sapply(V_raw, function(row) any(is.na(row))))) {
    warning(paste("Matriz contiene NA en:", rep_file))
    next
  }

  V_flat <- unlist(V_raw)
  if (length(V_flat) != N_i * N_j) {
    warning(paste("Dimensiones incorrectas en:", rep_file))
    next
  }

  V <- matrix(V_flat, ncol = N_j, byrow = TRUE)

  if (any(rowSums(V) == 0) || any(colSums(V) == 0)) {
    warning(paste("Matriz con filas o columnas con suma cero en:", rep_file))
    next
  }

  # Calcular métricas (resguardadas)
  vH2 <- c(vH2, tryCatch(H2fun(V, H2_integer = FALSE)[1], error = function(e) NA))
  vmod <- c(vmod, tryCatch(computeModules(V)@likelihood, error = function(e) NA))
  vnodf <- c(vnodf, tryCatch(nested(V, method = "WNODA") / 100, error = function(e) NA))
  vconnect <- c(vconnect, tryCatch(networklevel(V, index = "weighted connectance"), error = function(e) NA))
  vA <- c(vA, features[1])
  vF <- c(vF, features[2])
}

# Eliminar NAs si quedaron
vH2 <- na.omit(vH2)
vmod <- na.omit(vmod)
vnodf <- na.omit(vnodf)
vconnect <- na.omit(vconnect)
vA <- na.omit(vA)
vF <- na.omit(vF)

# Verificar si hay suficientes datos
if (length(vH2) == 0) {
  warning(paste("No se pudieron calcular métricas para:", dir_path))
  quit(status = 1)
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

# Guardar en archivo general por modelo
out_file <- paste0("results_", study, "/metrics_", study, ".txt")
write(paste(c(N_i, N_j, row_result), collapse = " "), file = out_file, append = TRUE)


