spack env deactivate
module purge
source $HOME/Downloads/spack/share/spack/setup-env.sh
spack load r
Rscript metrics.R | awk 'NR==2 {print $1}; NR==3 {print $2}; NR==5 {print $1}; NR==7 {print $1}' > data.txt