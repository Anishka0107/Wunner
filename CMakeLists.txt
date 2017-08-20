cmake_minimum_required (VERSION 2.6)
project (Wunner)

set (Wunner_VERSION_MAJOR 1)
set (Wunner_VERSION_MINOR 0)

include_directories (inc)
include_directories (lib)
add_subdirectory (lib/PorterStemmer)

add_library (porter_stemmer lib/PorterStemmer)

add_executable (wunner_search src/main.cpp)
