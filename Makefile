# Carpeta de código fuente y salida
SRC_DIR = modelos
BIN_DIR = bin

# Archivos fuente
SRC_ORIGINAL = $(SRC_DIR)/V_Lotka_Volterra.cpp
SRC_MODIFIED = $(SRC_DIR)/V_Lotka_Volterra_modified.cpp
SRC_BASE_DYNAMICS = $(SRC_DIR)/Lotka_Volterra.cpp
SRC_MOD_DYNAMICS = $(SRC_DIR)/mod_Lotka_Volterra.cpp

# Ejecutables
OUT_ORIGINAL = $(BIN_DIR)/V_Lotka_Volterra.x
OUT_MODIFIED = $(BIN_DIR)/V_Lotka_Volterra_modified.x
OUT_BASE_DYNAMICS = $(BIN_DIR)/Lotka_Volterra.x
OUT_MOD_DYNAMICS = $(BIN_DIR)/mod_Lotka_Volterra.x

# Bandera de compilación
CXX = g++
CXXFLAGS = -O3 -std=c++17 -fsanitize=address,undefined,leak

# Compilación por defecto
all: $(OUT_ORIGINAL) $(OUT_MODIFIED) $(OUT_BASE_DYNAMICS) $(OUT_MOD_DYNAMICS)

# Compilación de la dinámica

dynamics: $(OUT_BASE_DYNAMICS) $(OUT_MOD_DYNAMICS)
	mkdir -p dynamics_data
	$(OUT_BASE_DYNAMICS) 7 18 > dynamics_data/base.txt
	$(OUT_MOD_DYNAMICS) 7 18 3 > dynamics_data/low.txt
	$(OUT_MOD_DYNAMICS) 7 18 6 > dynamics_data/medium.txt
	$(OUT_MOD_DYNAMICS) 7 18 9 > dynamics_data/high.txt

plot: dynamics_data/base.txt dynamics_data/low.txt dynamics_data/medium.txt dynamics_data/high.txt
	rm -f dynamics_data/*.png
	gnuplot plot_test.gp

# Regla para el ejecutable original
$(OUT_ORIGINAL): $(SRC_ORIGINAL)
	$(CXX) $(CXXFLAGS) $< -o $@

# Regla para el ejecutable modificado
$(OUT_MODIFIED): $(SRC_MODIFIED)
	$(CXX) $(CXXFLAGS) $< -o $@

# Regla para la dinámica del ejecutable oiginal
$(OUT_BASE_DYNAMICS): $(SRC_BASE_DYNAMICS)
	$(CXX) $(CXXFLAGS) $< -o $@

# Regla para la dinámica del ejecutable modificado
$(OUT_MOD_DYNAMICS): $(SRC_MOD_DYNAMICS)
	$(CXX) $(CXXFLAGS) $< -o $@
	
# Limpiar binarios
clean:
	rm -f $(BIN_DIR)/*.x
	rm -f dynamics_data/*.png

.PHONY: all clean
