cmake_minimum_required(VERSION 3.16)
project(ProyectDeimos VERSION 0.1 LANGUAGES CXX)

# -------------------------------------
# Configuraciones estándar de Qt y C++
# -------------------------------------
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------------------
# Encuentra Qt (Qt6 preferido)
# -------------------------------------
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)

# -------------------------------------
# Archivos fuente del proyecto
# -------------------------------------
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    pantallainicio.cpp pantallainicio.h
    menuopciones.cpp menuopciones.h
    entidad.cpp entidad.h
    colatareas.cpp colatareas.h
    hilotrabajo.cpp hilotrabajo.h
    tranformacion.cpp tranformacion.h
    sprite.cpp sprite.h
    enemigo.cpp enemigo.h
    jugador.cpp jugador.h
    componentefisico.cpp componentefisico.h
    componentesalud.cpp componentesalud.h
    clickablelabel.cpp clickablelabel.h
    pantallacarga.cpp pantallacarga.h
    videointro.cpp videointro.h
    resources.qrc
)

# -------------------------------------
# Crea el ejecutable (.app en macOS)
# -------------------------------------
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ProyectDeimos
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(ProyectDeimos ${PROJECT_SOURCES})
endif()

# -------------------------------------
# Vincula bibliotecas Qt
# -------------------------------------
target_link_libraries(ProyectDeimos
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
)

# -------------------------------------
# Copiar carpeta Sprites/ al bundle .app
# -------------------------------------
set(RESOURCE_DEST "${CMAKE_BINARY_DIR}/ProyectDeimos.app/Contents/Resources")

add_custom_command(
    TARGET ProyectDeimos POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RESOURCE_DEST}/Sprites"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Sprites"
        "${RESOURCE_DEST}/Sprites"
)

# -------------------------------------
# Propiedades del bundle macOS
# -------------------------------------
set_target_properties(ProyectDeimos PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_BUNDLE_NAME "ProyectDeimos"
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.anavalle.ProyectDeimos"
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# -------------------------------------
# Instalación (opcional)
# -------------------------------------
include(GNUInstallDirs)
install(TARGETS ProyectDeimos
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# -------------------------------------
# Finalización para Qt6
# -------------------------------------
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ProyectDeimos)
endif()
