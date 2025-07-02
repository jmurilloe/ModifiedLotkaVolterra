spack env deactivate
module purge
source $HOME/Downloads/spack/share/spack/setup-env.sh
spack load r
Rscript metrics.R low 6 13