cmake_minimum_required (VERSION 2.6)
project (Wunner)

set (Wunner_VERSION_MAJOR 1)
set (Wunner_VERSION_MINOR 0)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14 -Wall")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories (inc)
include_directories (lib)

add_library (porter_stemmer lib/PorterStemmer/porter2_stemmer.cpp)

if (BUILD_STATIC)
  set (BUILD_SHARED_LIBRARIES OFF)
  set (CMAKE_EXE_LINKER_FLAGS "-static")
  set (CMAKE_FIND_LIBRARY_SUFFIXES .a)
  set (Boost_USE_STATIC_LIBS ON)
endif()

find_package (Boost COMPONENTS regex REQUIRED)
include_directories (${BOOST_INCLUDE_DIR})

set (LIBS_WUNNER
        inc/autocomplete.hpp
        inc/crawler.hpp
        inc/index_builder.hpp
        inc/parser.hpp
        inc/query.hpp
        inc/validator.hpp

        src/autocomplete.cpp
        src/crawler.cpp
        src/index_builder.cpp
        src/parser.cpp
        src/query.cpp
        src/validator.cpp
    )
add_library (wunner_libs ${LIBS_WUNNER})

set (LIBS_TO_LINK 
        ${Boost_LIBRARIES}
        porter_stemmer
        wunner_libs
    )

add_executable (wunner_search src/main.cpp)
target_link_libraries (wunner_search ${LIBS_TO_LINK})
