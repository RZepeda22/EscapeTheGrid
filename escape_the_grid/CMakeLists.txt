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

# Define una variable para contener todos los archivos fuente
set(SOURCE_FILES
    src/main.cpp
)
# Ejecutable principal
# Define el ejecutable y el cpp principal del proyecto.
add_executable(escape_the_grid ${SOURCE_FILES})

# Linkeo estático de SFML + dependencias del sistema
target_link_libraries(escape_the_grid
    # Módulos de SFML enlazados estáticamente.
    # Son las librerías principales de SFML
    sfml-graphics-s    # Módulo de gráficos
    sfml-window-s      # Módulo de ventana
    sfml-system-s      # Módulo de sistema 


    # Es utilizado internamente por varios módulos de SFML (graphics, window, audio, network)
    pthread

    # Dependencias adicionales del sistema operativo en este caso Windows para SFML estático en MinGW. 
    opengl32    # Requerida por sfml-graphics-s, sfml-window-s 
    winmm       # Requerida por sfml-system-s
    gdi32       # Requerida por sfml-window-s
    user32      # Requerida por sfml-window-s
    advapi32    # Requerida por sfml-system-s
)
