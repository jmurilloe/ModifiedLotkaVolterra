args <- commandArgs(trailingOnly = TRUE)

if (length(args) < 4) {
  stop("Uso: Rscript metrics.R archivo.txt N_i N_j salida.txt")
}

file_in <- args[1]
N_i <- as.integer(args[2])
N_j <- as.integer(args[3])
file_out <- args[4]

library(bipartite)

make_visitation_matrix <- function(Fi, Aj, beta_mat) {
  outer(Fi, Aj) * beta_mat
}

calculate_metrics <- function(V) {
  H2 <- tryCatch(H2fun(V, H2_integer = FALSE)[1], error = function(e) NA)
  mod <- tryCatch(computeModules(V)@likelihood, error = function(e) NA)
  nest <- tryCatch(nested(V, method = "WNODA") / 100, error = function(e) NA)
  conn <- tryCatch(networklevel(V, index = "weighted connectance"), error = function(e) NA)
  return(c(H2 = H2, mod = mod, nest = nest, conn = conn))
}

# Leer 1 de cada 10000 líneas (1 por segundo en simulación)
lines <- readLines(file_in)
header <- lines[1]
data_lines <- lines[seq(2, length(lines), by = 10000)]
data <- read.table(text = c(header, data_lines), header = TRUE)

# Columnas
t_col <- 1
P_cols <- 2:(1 + N_i)
A_cols <- (2 + N_i):(1 + N_i + N_j)
F_cols <- (2 + N_i + N_j):(1 + 2*N_i + N_j)
beta_cols <- (2 + 2*N_i + N_j):(1 + 2*N_i + N_j + N_i * N_j)

# Inicializar resultados
result <- data.frame(t = numeric(), H2 = numeric(), mod = numeric(), nest = numeric(), conn = numeric())

for (row in 1:nrow(data)) {
  t <- data[row, t_col]
  Fi <- as.numeric(data[row, F_cols])
  Aj <- as.numeric(data[row, A_cols])
  beta_vec <- as.numeric(data[row, beta_cols])
  beta_mat <- matrix(beta_vec, nrow = N_i, ncol = N_j, byrow = TRUE)
  
  V <- make_visitation_matrix(Fi, Aj, beta_mat)
  metrics <- calculate_metrics(V)
  
  result[row, ] <- c(t, metrics)
}

# Guardar en archivo .txt con tabulaciones
write.table(result, file = file_out, sep = "\t", row.names = FALSE, quote = FALSE)
