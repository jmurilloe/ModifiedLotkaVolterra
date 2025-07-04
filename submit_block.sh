#!/bin/bash
#SBATCH --job-name=lotka_manual
#SBATCH --output=slurm_out/out_%A_%a.txt
#SBATCH --error=slurm_out/err_%A_%a.txt
#SBATCH --time=00:30:00
#SBATCH --partition=12threads
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=1G

# Recibe el índice manualmente (0 a 2339)
ID=$1

# Parámetros del experimento
N_i_min=6
N_i_max=18
N_j_min=13
N_j_max=30
reps=10

let N_i_total=N_i_max-N_i_min+1
let N_j_total=N_j_max-N_j_min+1

let idx_pair=ID/reps
let rep=ID%reps
let i_idx=idx_pair/N_j_total
let j_idx=idx_pair%N_j_total

let N_i=N_i_min+i_idx
let N_j=N_j_min+j_idx

# Crear carpeta para la pareja (N_i, N_j)
out_dir="results_base/${N_i}_${N_j}"
mkdir -p "$out_dir"

# Ejecutar simulación y guardar features y matriz V
./bin/V_Lotka_Volterra.x $N_i $N_j > "$out_dir/rep${rep}.txt"
