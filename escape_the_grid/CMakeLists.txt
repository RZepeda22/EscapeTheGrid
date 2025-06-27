# Define la versión mínima de CMake requerida para el proyecto.
cmake_minimum_required(VERSION 3.10)

# Define el nombre del proyecto.
project(escape_the_grid)

# Establece el estándar C++ que se usará para compilar el código fuente.
set(CMAKE_CXX_STANDARD 17)

# Esto indica a los archivos de cabecera de SFML que deben prepararse para el enlazado estático,
add_definitions(-DSFML_STATIC)

# Rutas a SFML (ajustar estas a tu entorno)
# Aquí se especifican las rutas a los directorios include y lib de SFML.
include_directories("C:/SFML-3.0.0/include")
link_directories("C:/SFML-3.0.0/lib")

# Archivo de script de CMake que contiene la lógica para convertir el recurso.
# Es mejor aislar esta lógica en su propio script.
set(EMBED_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/cmake/embed_resource.cmake")



# Embeber recursos
set(RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY ${GENERATED_DIR})

# Generar archivo de fuente embebida usando el script externo
add_custom_command(
    OUTPUT "${GENERATED_DIR}/font_data.cpp"
    COMMAND ${CMAKE_COMMAND} 
        -DRESOURCE_FILE="${RESOURCE_DIR}/fonts/JB.ttf"
        -DOUTPUT_FILE="${GENERATED_DIR}/font_data.cpp"
        -DVARIABLE_NAME="embedded_font_data"
        -P "${EMBED_SCRIPT}"
    DEPENDS "${RESOURCE_DIR}/fonts/JB.ttf" "${EMBED_SCRIPT}"
    COMMENT "Generating embedded font data using external script"
)




# Define una variable para contener todos los archivos fuente - Agrega aquí los archivos fuente de tu proyecto.
# Puedes agregar manualmente los archivos fuente aquí, pero es más conveniente usar GLOB para incluirlos archivos automáticamente.
#set(SOURCE_FILES
#    src/main.cpp
#    src/Game.cpp
#    src/DrawMap.cpp
#    src/TriangularGrid.cpp
#    ${GENERATED_DIR}/font_data.cpp  # Agregar el archivo generado
#)

# Incluir automáticamente todos los archivos .cpp y .hpp
file(GLOB_RECURSE SOURCE_FILES
    "src/*.cpp"
    "src/*.hpp"
    "include/*.hpp"
)

# Agregar el archivo generado
list(APPEND SOURCE_FILES "${GENERATED_DIR}/font_data.cpp")


# Ejecutable principal
# Define el ejecutable y el cpp principal del proyecto.
add_executable(escape_the_grid ${SOURCE_FILES})

# Definir directorios de include
set(INCLUDE_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GENERATED_DIR} # Para acceder a los recursos embebidos
)

target_include_directories(escape_the_grid PUBLIC ${INCLUDE_DIRS})

# Linkeo estático de SFML + dependencias del sistema
target_link_libraries(escape_the_grid
    # Módulos de SFML enlazados estáticamente.
    # Son las librerías principales de SFML
    sfml-graphics-s    # Módulo de gráficos
    sfml-window-s      # Módulo de ventana
    sfml-system-s      # Módulo de sistema 


    # Es utilizado internamente por varios módulos de SFML (graphics, window, audio, network)
    pthread

    # Dependencias de SFML Graphics (para fuentes y texturas)
    freetype    # Necesaria para sf::Font

    # Dependencias adicionales del sistema operativo en este caso Windows para SFML estático en MinGW. 
    opengl32    # Requerida por sfml-graphics-s, sfml-window-s 
    winmm       # Requerida por sfml-system-s
    gdi32       # Requerida por sfml-window-s
    user32      # Requerida por sfml-window-s
    advapi32    # Requerida por sfml-system-s
)
