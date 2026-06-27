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
# 0. PREPROCESADOR DE BLOQUES asm{} TURBO C
# ---------------------------------------------------------
find_package(Python3 REQUIRED)

macro(preprocess_asm SOURCE_FILE TARGET_FILE)
    add_custom_command(
        OUTPUT ${TARGET_FILE}
        COMMAND ${Python3_EXECUTABLE}
            ${CMAKE_CURRENT_SOURCE_DIR}/asm2c.py
            ${SOURCE_FILE}
            > ${TARGET_FILE}
        DEPENDS ${SOURCE_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/asm2c.py
        COMMENT "Preprocesando bloques asm{} en ${SOURCE_FILE}..."
    )
endmacro()

# ---------------------------------------------------------
# 1. CONSTRUCCIÓN DE LA LIBRERÍA
# ---------------------------------------------------------
# Compilamos libgraph3.c y dos.c como una librería compartida (.so)
add_library(graph3 SHARED libgraph3.c dos.c)

# Vinculamos las dependencias. Usamos PUBLIC para que quien
# use nuestra librería herede automáticamente SDL3.
target_link_libraries(graph3 PUBLIC SDL3::SDL3 SDL3_ttf::SDL3_ttf m)

# El directorio actual se necesita para #include "dos.h" y "libgraph3.h"
target_include_directories(graph3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# ---------------------------------------------------------
# 2. PROGRAMAS DE EJEMPLO
# ---------------------------------------------------------

# main.c — demo original de sprites
set(MAIN_CONVERTED ${CMAKE_CURRENT_BINARY_DIR}/main_asmconv.c)
preprocess_asm(${CMAKE_CURRENT_SOURCE_DIR}/main.c ${MAIN_CONVERTED})
add_executable(programa ${MAIN_CONVERTED})
target_link_libraries(programa PRIVATE graph3)
target_include_directories(programa PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# ejemplo_hibrido.c — demo C + asm{} + libgraph3
set(EJ_HIBRIDO_CONVERTED ${CMAKE_CURRENT_BINARY_DIR}/ejemplo_hibrido_asmconv.c)
preprocess_asm(${CMAKE_CURRENT_SOURCE_DIR}/ejemplo_hibrido.c ${EJ_HIBRIDO_CONVERTED})
add_executable(ejemplo_hibrido ${EJ_HIBRIDO_CONVERTED})
target_link_libraries(ejemplo_hibrido PRIVATE graph3)
target_include_directories(ejemplo_hibrido PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# ---------------------------------------------------------
# 3. REGLAS DE INSTALACIÓN (El nuevo 'sudo make install')
# ---------------------------------------------------------
include(GNUInstallDirs)

# Copia el archivo .so a /usr/local/lib
install(TARGETS graph3 DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Copia las cabeceras a /usr/local/include
install(FILES libgraph3.h dos.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})