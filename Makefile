# Makefile

# Obtener las banderas de compilación (headers) de RInside y R/Rcpp.
# La salida de RInside::CxxFlags() DEBE contener las rutas -I para Rcpp, RInside y R base.
CPPFLAGS_RINSIDE = -I/usr/local/lib/R/site-library/RInside/include -I/usr/local/lib/R/site-library/Rcpp/include -I/usr/share/R/include

# Obtener las banderas de enlace (librerías) de RInside y R/Rcpp.
# La salida de RInside::LdFlags() DEBE contener todas las banderas -L y -l
# necesarias para RInside, Rcpp y las librerías CORE de R (como libR.so, libRmath.so, etc.).
LDFLAGS_RINSIDE = -L/usr/local/lib/R/site-library/RInside/lib -lRInside -Wl,-rpath,/usr/local/lib/R/site-library/RInside/lib -L/usr/lib/R/lib -Wl,-Bsymbolic-functions -flto=auto -ffat-lto-objects -flto=auto -Wl,-z,relro

# Tu compilador C++
CXX = g++

# CXXFLAGS: Incluye tus rutas de cabecera específicas si son necesarias,
# PERO asegúrate de que $(CPPFLAGS_RINSIDE) también esté presente para las de R/Rcpp/RInside.
# Por ejemplo, si tienes Rcpp en una ruta no estándar:
CXXFLAGS = -Wall $(CPPFLAGS_RINSIDE) 

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