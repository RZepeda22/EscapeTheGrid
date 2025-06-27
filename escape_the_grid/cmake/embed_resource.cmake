# Script para embebir recursos - cmake/embed_resource.cmake
# Este script se ejecuta durante la construcción del proyecto

# Variables esperadas:
# - RESOURCE_FILE: Ruta del archivo a embebir
# - OUTPUT_FILE: Archivo C++ de salida
# - VARIABLE_NAME: Nombre de la variable en C++

# Script para embeber recursos como arrays de bytes en C++
# Convierte cualquier archivo binario en un array de bytes utilizable desde C++

# Verificar que todas las variables necesarias están definidas
if(NOT DEFINED RESOURCE_FILE)
    message(FATAL_ERROR "RESOURCE_FILE no está definido")
endif()

if(NOT DEFINED OUTPUT_FILE)
    message(FATAL_ERROR "OUTPUT_FILE no está definido")
endif()

if(NOT DEFINED VARIABLE_NAME)
    message(FATAL_ERROR "VARIABLE_NAME no está definido")
endif()

# Verificar que el archivo de recurso existe
if(NOT EXISTS "${RESOURCE_FILE}")
    message(FATAL_ERROR "El archivo de recurso no existe: ${RESOURCE_FILE}")
endif()

# Leer el archivo de recurso como datos binarios (hexadecimal)
file(READ "${RESOURCE_FILE}" file_content HEX)

# Obtener el tamaño del archivo en bytes
file(SIZE "${RESOURCE_FILE}" file_size)

# Convertir datos hexadecimales a formato de array C++
# Cada par de caracteres hex se convierte en 0xXX,
string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," array_values "${file_content}")

# Remover la última coma
string(REGEX REPLACE ",$" "" array_values "${array_values}")

# Crear el contenido del archivo C++ generado
set(output_content
"// Archivo generado automáticamente por embed_resource.cmake
// NO EDITAR MANUALMENTE - Los cambios se perderán al regenerar

#include <cstddef>

// Array de bytes que contiene los datos del recurso embebido
extern const unsigned char ${VARIABLE_NAME}[] = {
    ${array_values}
};

// Tamaño del recurso en bytes
extern const size_t ${VARIABLE_NAME}_size = ${file_size}U;
")

# Escribir el archivo de salida
file(WRITE "${OUTPUT_FILE}" "${output_content}")

# Mensaje informativo
message(STATUS "Recurso embebido generado:")
message(STATUS "  Archivo origen: ${RESOURCE_FILE}")
message(STATUS "  Archivo destino: ${OUTPUT_FILE}")
message(STATUS "  Variable: ${VARIABLE_NAME}")
message(STATUS "  Tamaño: ${file_size} bytes")