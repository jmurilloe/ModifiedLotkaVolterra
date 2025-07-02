spack env deactivate
module purge
source $HOME/Downloads/spack/share/spack/setup-env.sh
spack load r
Rscript metrics.R | awk 'NR==2 {print $1}; NR==4 {print $1}; NR==6 {print $1}; NR==8 {print $1}' > data_metrics.txt