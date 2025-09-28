cmake_minimum_required(VERSION 3.16)

project(GraphicViewExample VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS Core)

    qt_add_executable(GraphicViewExample
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        schematicviewer.h schematicviewer.cpp
        verilogparser.h
        verilogparser.cpp
        verilogblock.h
        verilogblock.cpp
        pin.h pin.cpp

        rcs.qrc
        wire.h
        wire.cpp
        GridBlock.h
        GridBlock.cpp
        block_view_form.h block_view_form.cpp block_view_form.ui
        codehighlighter.h codehighlighter.cpp
        crashhandler.h crashhandler.cpp
        childFilterProxyModel.h
        childFilterProxyModel.cpp
        TreeViewItemWithPath.h TreeViewItemWithPath.cpp



    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET GraphicViewExample APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(GraphicViewExample SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(GraphicViewExample
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(GraphicViewExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(GraphicViewExample PRIVATE Qt6::Core)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.GraphicViewExample)
endif()
set_target_properties(GraphicViewExample PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS GraphicViewExample
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GraphicViewExample)
endif()
