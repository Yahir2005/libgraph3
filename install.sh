#!/bin/bash
set -e

echo "===================================================="
echo "   Instalador Inteligente de libgraph3 (Bash)       "
echo "===================================================="

OS_TYPE="unknown"
if [[ "$OSTYPE" == "darwin"* ]]; then
    OS_TYPE="macos"
    echo "[INFO] Plataforma detectada: macOS (Homebrew)"
elif command -v apt >/dev/null 2>&1; then
    OS_TYPE="debian"
    echo "[INFO] Plataforma detectada: Debian/Ubuntu (apt)"
elif command -v dnf >/dev/null 2>&1; then
    OS_TYPE="fedora"
    echo "[INFO] Plataforma detectada: Fedora/RHEL (dnf)"
else
    echo "[ERROR] Gestor de paquetes no soportado."
    exit 1
fi

read -p "¿Deseas instalar las dependencias del sistema (cmake, git, gcc, x11, etc)? [s/n]: " opc
if [[ "$opc" == "s" || "$opc" == "S" ]]; then
    echo "[1/4] Instalando pre-requisitos de desarrollo..."
    if [ "$OS_TYPE" == "debian" ]; then
        sudo apt update && sudo DEBIAN_FRONTEND=noninteractive apt install -y cmake git build-essential libfreetype-dev libharfbuzz-dev libx11-dev libwayland-dev libxkbcommon-dev libxext-dev libxrender-dev libxcursor-dev libxi-dev libxfixes-dev libxrandr-dev libxss-dev libxtst-dev
    elif [ "$OS_TYPE" == "fedora" ]; then
        sudo dnf groupinstall -y "Development Tools" && sudo dnf install -y cmake git libX11-devel wayland-devel
    elif [ "$OS_TYPE" == "macos" ]; then
        brew install cmake git
    fi
else
    echo "[1/4] Saltando instalación de dependencias."
fi

echo "[2/4] Compilando libgraph3..."
mkdir -p build && cd build
cmake .. -DFETCHCONTENT_QUIET=OFF
make -j$(nproc 2>/dev/null || echo 2)

echo "[3/4] Instalando librerías globalmente en el sistema..."
EXT="so"
if [ "$OS_TYPE" == "macos" ]; then EXT="dylib"; fi

sudo cp -RP $(find _deps -name "libSDL3.${EXT}*") /usr/local/lib/ 2>/dev/null || true
sudo cp -RP $(find _deps -name "libSDL3_ttf.${EXT}*") /usr/local/lib/ 2>/dev/null || true
sudo cp -RP libgraph3.${EXT} /usr/local/lib/ 2>/dev/null || true
sudo cp ../libgraph3.h /usr/local/include/ 2>/dev/null || true

# Descarga e instalación de la fuente base portable
sudo mkdir -p /usr/local/share/libgraph3
echo "Descargando fuente OpenSans portable..."
sudo curl -sL https://raw.githubusercontent.com/google/fonts/main/ofl/opensans/OpenSans-Regular.ttf -o /usr/local/share/libgraph3/font.ttf || true

size=$(stat -c%s "/usr/local/share/libgraph3/font.ttf" 2>/dev/null || echo 0)
if [ "$size" -lt 10000 ]; then
    echo "[ADVERTENCIA] No se pudo descargar la fuente de internet. Usando fuente local del sistema..."
    sudo cp /usr/share/fonts/truetype/dejavu/DejaVuSans.ttf /usr/local/share/libgraph3/font.ttf 2>/dev/null || true
fi

if [ "$OS_TYPE" != "macos" ]; then
    echo "[4/5] Actualizando caché del enlazador global (ldconfig)..."
    sudo ldconfig
fi

echo "[5/5] Instalando atajo de compilación 'cgraph'..."
cat << 'EOF' > cgraph
#!/bin/bash
if [ -z "$1" ]; then
    echo "Uso: cgraph <archivo.c> [opciones extra]"
    exit 1
fi

FILENAME=$(basename -- "$1")
OUTFILE="${FILENAME%.*}"

echo "Compilando $1 -> $OUTFILE ..."
gcc "$@" -o "$OUTFILE" -lgraph3 -lm
EOF
chmod +x cgraph
sudo cp cgraph /usr/local/bin/cgraph
rm cgraph

echo "===================================================="
echo " ¡Instalación completada exitosamente!              "
echo " El motor ya está disponible de manera global.      "
echo "                                                    "
echo " Compila tus programas fácilmente usando:           "
echo " cgraph tu_codigo.c                                 "
echo "===================================================="
