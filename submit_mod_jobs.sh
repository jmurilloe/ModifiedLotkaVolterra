#!/bin/bash

N_i_min=6
N_i_max=18
N_j_min=13
N_j_max=30
reps=5

mkdir -p slurm_out_mod results_low results_medium results_high

for ((N_i=N_i_min; N_i<=N_i_max; N_i++)); do
  for ((N_j=N_j_min; N_j<=N_j_max; N_j++)); do

    N_m_low=$((N_j / 6))
    N_m_med=$((N_j / 3))
    N_m_high=$((N_j / 2))

    outdir_low="results_low/${N_i}_${N_j}"
    outdir_med="results_medium/${N_i}_${N_j}"
    outdir_high="results_high/${N_i}_${N_j}"
    mkdir -p "$outdir_low" "$outdir_med" "$outdir_high"

    for ((rep=0; rep<reps; rep++)); do

      # --- LOW ---
      sbatch --job-name=mod_${N_i}${N_j}${rep}_low --partition=16threads --mem=1G --time=00:30:00 \
        --output=slurm_out_mod/out_${N_i}${N_j}${rep}_low.txt \
        --error=slurm_out_mod/err_${N_i}${N_j}${rep}_low.txt \
        --wrap="bash -c './bin/V_Lotka_Volterra_modified.x $N_i $N_j $N_m_low > $outdir_low/rep${rep}.txt'"

      # --- MEDIUM ---
      sbatch --job-name=mod_${N_i}${N_j}${rep}_med --partition=16threads --mem=1G --time=00:30:00 \
        --output=slurm_out_mod/out_${N_i}${N_j}${rep}_med.txt \
        --error=slurm_out_mod/err_${N_i}${N_j}${rep}_med.txt \
        --wrap="bash -c './bin/V_Lotka_Volterra_modified.x $N_i $N_j $N_m_med > $outdir_med/rep${rep}.txt'"

      # --- HIGH ---
      sbatch --job-name=mod_${N_i}${N_j}${rep}_high --partition=16threads --mem=1G --time=00:30:00 \
        --output=slurm_out_mod/out_${N_i}${N_j}${rep}_high.txt \
        --error=slurm_out_mod/err_${N_i}${N_j}${rep}_high.txt \
        --wrap="bash -c './bin/V_Lotka_Volterra_modified.x $N_i $N_j $N_m_high > $outdir_high/rep${rep}.txt'"

    done
  done
done
