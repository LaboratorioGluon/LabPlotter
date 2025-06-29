# mi_proyecto/libs/CMakeLists.txt

# Definimos la ruta base a QCustomPlot dentro de este directorio
set(QCUSTOMPLOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/qcustomplot")


# Archivos fuente y cabecera de QCustomPlot
set(QCUSTOMPLOT_SOURCES
${QCUSTOMPLOT_DIR}/qcustomplot.cpp
)

set(QCUSTOMPLOT_HEADERS
${QCUSTOMPLOT_DIR}/qcustomplot.h
)

# Agrega QCustomPlot como una librería estática
add_library(QCustomPlot STATIC
${QCUSTOMPLOT_SOURCES}
${QCUSTOMPLOT_HEADERS}
)

#target_compile_definitions(QCustomPlot PRIVATE QCUSTOMPLOT_USE_OPENGL)

# Define las propiedades de inclusión para QCustomPlot.
# Es crucial que el INCLUDE_DIRECTORIES apunte al directorio de la cabecera de QCustomPlot.
target_include_directories(QCustomPlot PUBLIC
    $<BUILD_INTERFACE:${QCUSTOMPLOT_DIR}> # Para cuando se compila
)

# Enlaza QCustomPlot con los módulos de Qt que necesita
target_link_libraries(QCustomPlot PUBLIC
    Qt6::Gui
    Qt6::Core
    Qt6::PrintSupport
#    Qt6::OpenGL
)

# Habilita el moc para QCustomPlot (si contiene Q_OBJECT)
set_target_properties(QCustomPlot PROPERTIES AUTOMOC ON)

# Si tuvieras más librerías en 'libs/', añadirías aquí su add_library, fuentes, etc.
# Por ejemplo:
# set(ANOTHERLIB_SOURCES anotherlib/anotherlib.cpp)
# add_library(AnotherLib STATIC ${ANOTHERLIB_SOURCES})
# target_include_directories(AnotherLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/anotherlib)
# target_link_libraries(AnotherLib PUBLIC Qt6::Core)