# Define la versión mínima de CMake requerida
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# Define el nombre de tu proyecto
project(LabPlotter VERSION 1.0.0 LANGUAGES CXX)


# Configuración de C++
# Establece el estándar C++ (por ejemplo, C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON) 
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

# Habilita los módulos de Qt necesarios
# Usa find_package para buscar los módulos de Qt.
# REQUIRED significa que la compilación fallará si no se encuentran.
# COMPONENTS especifica los módulos que usarás (Ej: Widgets, Core, Gui, Network)
find_package(Qt6 COMPONENTS 
    Widgets 
    Core  
    Gui 
    PrintSupport 
    SerialPort
    REQUIRED)

qt_standard_project_setup()


add_subdirectory(src)
add_subdirectory(libs)

# Enlaza las librerías de Qt necesarias a tu ejecutable
# target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)
# Una forma más sencilla y recomendada con Qt6:
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

# Opcional: Define un prefijo de instalación
# install(TARGETS ${PROJECT_NAME} DESTINATION bin)