project(NobeoTest)
cmake_minimum_required(VERSION 2.8.12)

SET(QT_MIN_VERSION "5.4.0")
SET(QT_OFFICIAL_VERSION "5.4")
add_definitions(-DUSE_QT5)
find_package(Qt5 COMPONENTS Gui Widgets Network Sql Xml Concurrent)

add_subdirectory(Core)
add_subdirectory(UI)

#-----------------------------------------------------------------------------
# Add extra compilation flags
#-----------------------------------------------------------------------------
include(CMakeModules/AddExtraCompilationFlags.cmake)
ADD_EXTRA_COMPILATION_FLAGS()