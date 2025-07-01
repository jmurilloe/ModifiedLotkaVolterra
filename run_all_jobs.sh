#!/bin/bash

total=2340   # 13 x 18 x 10

# Crea carpetas necesarias
mkdir -p slurm_out results

# Lanza cada trabajo con su índice manual
for id in $(seq 0 2339); do
    while [[ $(squeue -u $USER | grep lotka_manual | wc -l) -ge 96 ]]; do
        sleep 5
    done
    sbatch submit_block.sh $id
done
