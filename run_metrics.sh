#!/bin/bash

spack env deactivate
module purge
source $HOME/Downloads/spack/share/spack/setup-env.sh
spack load r

# Crear encabezado si no existe
for model in base low medium high; do
  metrics_file="results_${model}/metrics_${model}.txt"
  if [ ! -f "$metrics_file" ]; then
    echo "N_i N_j H2_mean H2_sd mod_mean mod_sd nodf_mean nodf_sd Connect_mean Connect_sd A_mean A_sd F_mean F_sd ratio" > "$metrics_file"
  fi
done

parallel -j 16 'Rscript metrics_all_reps.R {1} {2} {3}' ::: base low medium high ::: {6..18} ::: {13..30}
