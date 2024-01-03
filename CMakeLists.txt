cmake_minimum_required(VERSION 3.12)
project(dynamic_lookup)
set(CMAKE_CXX_STANDARD 17)

option(DYNAMIC_LOOKUP "Adds dynamic lookup linker flag" OFF)

add_library(AbstractPlugin OBJECT AbstractPlugin.cpp)
add_library(ModulePlugin MODULE ModulePlugin.cpp)
if (${DYNAMIC_LOOKUP})
    target_link_options(ModulePlugin PRIVATE -undefined dynamic_lookup)
endif()

add_executable(dynamic_lookup main.cpp)
target_link_libraries(dynamic_lookup PUBLIC AbstractPlugin ${CMAKE_DL_LIBS})

set_property(TARGET dynamic_lookup PROPERTY ENABLE_EXPORTS ON)
