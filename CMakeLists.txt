cmake_minimum_required(VERSION 3.16)
project(libgraph3 C)

include(FetchContent)

# Descargamos el motor base SDL3
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG main 
)
FetchContent_MakeAvailable(SDL3)

# Descargamos la extensión de fuentes SDL3_ttf
FetchContent_Declare(
    SDL3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG main
)
FetchContent_MakeAvailable(SDL3_ttf)

# ---------------------------------------------------------
# 1. CONSTRUCCIÓN DE LA LIBRERÍA
# ---------------------------------------------------------
# Compilamos libgraph3.c como una librería compartida (.so)
add_library(graph3 SHARED libgraph3.c)

# Vinculamos las dependencias. Usamos PUBLIC para que quien
# use nuestra librería herede automáticamente SDL3.
target_link_libraries(graph3 PUBLIC SDL3::SDL3 SDL3_ttf::SDL3_ttf m)

# ---------------------------------------------------------
# 2. PROGRAMA DE PRUEBA (Opcional)
# ---------------------------------------------------------
# Mantenemos tu main.c para que puedas seguir probando
add_executable(programa main.c)
target_link_libraries(programa PRIVATE graph3)

# ---------------------------------------------------------
# 3. REGLAS DE INSTALACIÓN (El nuevo 'sudo make install')
# ---------------------------------------------------------
include(GNUInstallDirs)

# Copia el archivo .so a /usr/local/lib
install(TARGETS graph3 DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Copia la cabecera .h a /usr/local/include
install(FILES libgraph3.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})