#!/bin/bash

# Crear estructura
mkdir -p modelos bin data/original data/modified_low data/modified_medium data/modified_high scripts results

# Mover archivos fuente
mv V_Lotka_Volterra.cpp modelos/
mv V_Lotka_Volterra_modified.cpp modelos/

# Compilar con banderas de sanitización y extensiones .x
g++ -O3 -fsanitize=address,undefined,leak modelos/V_Lotka_Volterra.cpp -o bin/a_original.x
g++ -O3 -fsanitize=address,undefined,leak modelos/V_Lotka_Volterra_modified.cpp -o bin/a_modificado.x

# Mover scripts .sh y .R al directorio correspondiente
find . -maxdepth 1 -type f \( -name "*.sh" -o -name "*.R" \) -exec mv {} scripts/ \;

# Crear .gitignore sin excluir data/
cat > .gitignore << EOF
/bin/
/*.o
*.swp
*.tmp
EOF

# README básico
echo "# Proyecto Lotka-Volterra con modificación parcial" > README.md

echo "✅ Proyecto organizado y ejecutables compilados con sanitizadores."
