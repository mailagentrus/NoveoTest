project(NoveoTestUI)
cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${Qt5Core_INCLUDES}
        ${Qt5Widgets_INCLUDES})

SET(Sources
        main.cpp
        MainWindow.cpp
        ResultViewWidget.cpp)

SET(Headers
        MainWindow.h
        ResultViewWidget.h)

SET(UIElements
        ResultViewWidget.ui
        MainWindow.ui)

QT5_WRAP_UI(UIElementsHeaders ${UIElements})

SET(Resources MainWindow.qrc)

QT5_ADD_RESOURCES(ResourcesHeaders ${Resources})

add_executable(NoveoTestUI ${Sources} ${Headers} ${ResourcesHeaders} ${UIElementsHeaders})

target_link_libraries(NoveoTestUI ${Qt5Widgets_LIBRARIES} NoveoTestCore)