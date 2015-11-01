MACRO(ADD_SOURCE_GROUP_FLAGS)
    SOURCE_GROUP("Sources" FILES ${Sources})
    SOURCE_GROUP("Headers" FILES ${Headers})
    SOURCE_GROUP("UI" FILES ${UIElements})
    SOURCE_GROUP("Resources" FILES ${Resources})
    SOURCE_GROUP("Resources" FILES ${Resources})

    FILE(GLOB AutoMOCFiles ${CMAKE_CURRENT_BINARY_DIR}/*automoc.cpp)
    SOURCE_GROUP("AutoMOC files" FILES ${AutoMOCFiles})

    FILE(GLOB AutoUIFiles ${CMAKE_CURRENT_BINARY_DIR}/ui_*.h)
    SOURCE_GROUP("AutoUI files" FILES ${AutoUIFiles})

    FILE(GLOB AutoQRCFiles ${CMAKE_CURRENT_BINARY_DIR}/qrc_*.cpp)
    SOURCE_GROUP("AutoQRC files" FILES ${AutoQRCFiles})
ENDMACRO(ADD_SOURCE_GROUP_FLAGS)
