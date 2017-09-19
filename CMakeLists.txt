cmake_minimum_required (VERSION 2.8)
project (Wunner)

set (Wunner_VERSION_MAJOR 1)
set (Wunner_VERSION_MINOR 0)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories (inc)
include_directories (lib)

set (BUILD_SHARED_LIBRARIES OFF)
set (CMAKE_EXE_LINKER_FLAGS "-static")
set (CMAKE_FIND_LIBRARY_SUFFIXES .a)
set (CMAKE_EXE_LINK_DYNAMIC_C_FLAGS)       # remove -Wl,-Bdynamic
set (CMAKE_EXE_LINK_DYNAMIC_CXX_FLAGS)
set (CMAKE_SHARED_LIBRARY_C_FLAGS)         # remove -fPIC
set (CMAKE_SHARED_LIBRARY_CXX_FLAGS)
set (CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)    # remove -rdynamic
set (CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

add_library (porter_stemmer lib/PorterStemmer/porter2_stemmer.cpp)

find_package (Threads REQUIRED)

set (LIBS_WUNNER
        inc/autocomplete.hpp
        inc/crawler.hpp
        inc/crawler_info.hpp
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
        wunner_libs
        porter_stemmer
        ${CMAKE_THREAD_LIBS_INIT}
    )

add_executable (wunner_search src/main.cpp)
target_link_libraries (wunner_search ${LIBS_TO_LINK})
