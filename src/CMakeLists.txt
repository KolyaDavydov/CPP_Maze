cmake_minimum_required(VERSION 3.5)

project(Maze LANGUAGES CXX)

# set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        view/main.cc
        view/mainwindow.cc
        model/model.cc
        model/pathfinder.cc
        controller/controller.cc
        
)

set(PROJECT_HEADERS
        view/mainwindow.h
        model/model.h
        model/pathfinder.cc
        controller/controller.h
)

set(PROJECT_UI
        view/mainwindow.ui
)

set(PROJECT_RSC

)

set(PROJECT_FILES ${PROJECT_RSC} ${PROJECT_UI} ${PROJECT_HEADERS} ${PROJECT_SOURCES})


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Maze
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Calc APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Maze SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Maze
            ${PROJECT_SOURCES}
        )
    endif()
endif()


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    target_link_libraries(Maze PRIVATE Qt6::Widgets)
else()
    target_link_libraries(Maze PRIVATE Qt5::Widgets)
endif()
