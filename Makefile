# Carpeta de código fuente y salida
SRC_DIR = modelos
BIN_DIR = bin

# Archivos fuente
SRC_ORIGINAL = $(SRC_DIR)/V_Lotka_Volterra.cpp
SRC_MODIFIED = $(SRC_DIR)/V_Lotka_Volterra_modified.cpp

# Ejecutables
OUT_ORIGINAL = $(BIN_DIR)/V_Lotka_Volterra.x
OUT_MODIFIED = $(BIN_DIR)/V_Lotka_Volterra_modified.x

# Bandera de compilación
CXX = g++
CXXFLAGS = -O3 -std=c++17 -fsanitize=address,undefined,leak

# Compilación por defecto
all: $(OUT_ORIGINAL) $(OUT_MODIFIED)

# Regla para el ejecutable original
$(OUT_ORIGINAL): $(SRC_ORIGINAL)
	$(CXX) $(CXXFLAGS) $< -o $@

# Regla para el ejecutable modificado
$(OUT_MODIFIED): $(SRC_MODIFIED)
	$(CXX) $(CXXFLAGS) $< -o $@

# Limpiar binarios
clean:
	rm -f $(BIN_DIR)/*.x

.PHONY: all clean
