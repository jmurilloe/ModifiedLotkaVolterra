# Makefile

# Obtener las banderas de compilación (headers) de RInside y R/Rcpp.
# La salida de RInside::CxxFlags() DEBE contener las rutas -I para Rcpp, RInside y R base.
CPPFLAGS_RINSIDE = $(shell Rscript -e 'RInside::CxxFlags()')

# Obtener las banderas de enlace (librerías) de RInside y R/Rcpp.
# La salida de RInside::LdFlags() DEBE contener todas las banderas -L y -l
# necesarias para RInside, Rcpp y las librerías CORE de R (como libR.so, libRmath.so, etc.).
LDFLAGS_RINSIDE = $(shell Rscript -e 'RInside::LdFlags()')

# Tu compilador C++
CXX = g++

# CXXFLAGS: Incluye tus rutas de cabecera específicas si son necesarias,
# PERO asegúrate de que $(CPPFLAGS_RINSIDE) también esté presente para las de R/Rcpp/RInside.
# Por ejemplo, si tienes Rcpp en una ruta no estándar:
CXXFLAGS = -Wall $(CPPFLAGS_RINSIDE) -I/home/jmurilloe/Downloads/spack/opt/spack/linux-slackware15-skylake/gcc-14.2.0/r-rcpp-1.0.13-3gbuuueractg3y2odrnp5gv4qnapvvmf/rlib/R/library/Rcpp/include # Agrega -I para tu Rcpp.h si es necesario

# LDFLAGS: Esta línea es CRÍTICA para "undefined reference".
# Debe usar la salida COMPLETA de RInside::LdFlags().
LDFLAGS = $(LDFLAGS_RINSIDE)

TARGET = my_r_app

all: $(TARGET)

$(TARGET): my_r_app.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS) 

# Asegúrate de que esta línea empiece con un TAB

clean:
	rm -f $(TARGET)