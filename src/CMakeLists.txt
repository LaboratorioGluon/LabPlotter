# mi_proyecto/src/CMakeLists.txt

# No necesitas project(), cmake_minimum_required(), find_package(), etc. aquí.
# Esas configuraciones se heredan del CMakeLists.txt padre.

# Define los archivos fuente, cabecera y UI específicos de esta aplicación
set(APP_SOURCES
    main.cpp
    mainWindow.cpp
    dataInterface/SerialPortSource.cpp
    dataInterface/CharSeparatedParser.cpp
    dataInterface/IDataParser.cpp
    DataManager.cpp
)

set(APP_HEADERS
    mainWindow.h
    DataManager.h
    dataInterface/IDataSource.h
    dataInterface/DataPoint.h
    dataInterface/CharSeparatedParser.h
    dataInterface/SerialPortSource.h
)

set(APP_FORMS
    mainWindow.ui
)

# Si los recursos son específicos de esta aplicación, defínelos aquí.
# De lo contrario, déjalos en el CMakeLists.txt raíz y enlázalos.
set(APP_RESOURCES
    ../resources/app_resources.qrc # Ruta relativa al CMakeLists.txt de este directorio
)

# Crea el ejecutable para esta aplicación
add_executable(${PROJECT_NAME} # Puedes usar ${PROJECT_NAME} o un nombre específico como "MyDesktopApp"
    ${APP_SOURCES}
    ${APP_HEADERS}
    ${APP_FORMS}
    ${APP_RESOURCES}
)

# Enlaza las librerías de Qt necesarias a este ejecutable.
# Las librerías de Qt ya están disponibles porque fueron encontradas en el CMakeLists.txt padre.
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt6::Widgets
    Qt6::Gui
    Qt6::Core
    QCustomPlot
    Qt6::SerialPort)

# Opcional: Propiedades específicas para este ejecutable (si quieres algo diferente de lo global)
# set_target_properties(${PROJECT_NAME} PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/my_app_folder"
# )