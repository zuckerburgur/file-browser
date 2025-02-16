cmake_minimum_required(VERSION 3.5)

project(DSProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Try to find Qt6 first, then fallback to Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Try to find WebEngineWidgets component, but don't make it required initially
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS WebEngineWidgets)

set(PROJECT_SOURCES
    main.cpp
    gui1.cpp
    gui1.h
    gui1.ui
    webpagedisplay.h
    webpagedisplay.cpp
    webpagedisplay.ui
    querywindow.h
    querywindow.cpp
    querywindow.ui
    errorwindow.h
    errorwindow.cpp
    errorwindow.ui
    searchengine.h
    searchengine.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(DSProject
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        graphwindow.h graphwindow.cpp graphwindow.ui
    )
else()
    if(ANDROID)
        add_library(DSProject SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(DSProject
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Conditionally link WebEngineWidgets if found
if (TARGET Qt${QT_VERSION_MAJOR}::WebEngineWidgets)
    target_link_libraries(DSProject PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::WebEngineWidgets)
    target_compile_definitions(DSProject PRIVATE USE_WEBENGINE)
else()
    target_link_libraries(DSProject PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
    message(WARNING "Qt${QT_VERSION_MAJOR}::WebEngineWidgets not found, proceeding without web functionality.")
endif()

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.DSProject)
endif()

set_target_properties(DSProject PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS DSProject
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(DSProject)
endif()
