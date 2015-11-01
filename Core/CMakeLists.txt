project(NoveoTestCore)
cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${Qt5Core_INCLUDES}
        ${Qt5Sql_INCLUDES}
        ${Qt5Network_INCLUDES}
        ${Qt5Xml_INCLUDES}
        private)

SET(Sources
        ITunesSearchEngine.cpp
        JSONSearchResult.cpp
        JSONSearchResultList.cpp
        JSONParserAbstract.cpp
        ModelInfo.cpp
        ModelUserView.cpp)

SET(Headers
        ITunesSearchEngine.h
        private/ITunesSearchEngine_p.h
        JSONSearchResult.h
        JSONSearchResultList.h
        JSONParserAbstract.h
        Singleton.h
        ModelInfo.h
        ModelUserView.h
        Model.h
        DBTableInfo.h
        DBTableUserView.h
        DBTableAbstract.h)

add_library(NoveoTestCore STATIC ${Sources} ${Headers})

target_link_libraries(NoveoTestCore ${Qt5Core_LIBRARIES} ${Qt5Sql_LIBRARIES} ${Qt5Network_LIBRARIES} ${Qt5Xml_LIBRARIES})

target_include_directories(NoveoTestCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})