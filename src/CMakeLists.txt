

# Los archivos fuente (.cpp)
set(APP_SOURCES
main.cpp
mainWindow.cpp
DataManager.cpp
SourceManager.cpp
dataInterface/SerialPortSource.cpp
dataInterface/CharSeparatedParser.cpp
dataInterface/IDataParser.cpp
widgets/connectionPanel.cpp
widgets/serialPortConfig.cpp
)

# Los archivos de cabecera (.h)
set(APP_HEADERS
mainWindow.h
SourceManager.h
DataManager.h
dataInterface/IDataSource.h
dataInterface/DataPoint.h
dataInterface/CharSeparatedParser.h
dataInterface/SerialPortSource.h
widgets/connectionPanel.h
widgets/serialPortConfig.h
)

set(APP_FORMS
ui/mainWindow.ui
ui/connectionPanel.ui
ui/serialPortConfig.ui
)

# Los archivos de recursos .qrc
set(APP_RESOURCES
../resources/app_resources.qrc # Ruta relativa a src/
)


qt_add_executable(${PROJECT_NAME}
${APP_SOURCES}
${APP_HEADERS}
${APP_FORMS}
${APP_RESOURCES}
)


target_link_libraries(${PROJECT_NAME} PRIVATE
Qt6::Widgets
    Qt6::Gui
    Qt6::Core
    Qt6::SerialPort
    QCustomPlot
)


target_include_directories(${PROJECT_NAME} PRIVATE
    $<TARGET_PROPERTY:${PROJECT_NAME},AUTOGEN_INCLUDE_DIR> # <--- ¡Esta es la forma más robusta!

    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src                     
    ${CMAKE_SOURCE_DIR}/src/dataInterface       
    ${CMAKE_SOURCE_DIR}/src/widgets            
)

# Opcional: Propiedades específicas para este ejecutable
# set_target_properties(${PROJECT_NAME} PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/my_app_folder"
# )