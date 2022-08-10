cmake_minimum_required(VERSION 3.4...3.18)
project(car2022)

set (CMAKE_CXX_STANDARD 17)


#---------------------------------------------------------------------
#       [ Include、Lib  : Define ] ==> []
#---------------------------------------------------------------------

set(COMMON_LIB_DIR "${PROJECT_SOURCE_DIR}/lib/")

set(COMMON_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")


link_directories(${COMMON_LIB_DIR})
include_directories( ${COMMON_INCLUDE_DIR} )

#---------------------------------------------------------------------
#       [ Include、Lib  : Path ] ==> [ glib opencv ]
#---------------------------------------------------------------------
find_package(PkgConfig)
pkg_search_module(GLIB REQUIRED glib-2.0)
include_directories(${GLIB_INCLUDE_DIRS})

find_package( OpenCV REQUIRED )    
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package(PaddleLite QUIET)
include_directories(${PADDLELITE_INCLUDE_DIR})
LINK_DIRECTORIES("/usr/local/lib/paddle_lite/")
message(${PADDLELITE_LIBRARY})

#---------------------------------------------------------------------
#               [ bin ] ==> [ demo ]
#---------------------------------------------------------------------

aux_source_directory(. source_list)

set(MAIN_PROJECT_NAME "run_car")

add_executable(${MAIN_PROJECT_NAME} ${source_list})
target_link_libraries(${MAIN_PROJECT_NAME} PRIVATE pthread )
target_link_libraries(${MAIN_PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
target_link_libraries(${MAIN_PROJECT_NAME} PRIVATE serial)
target_link_libraries(${MAIN_PROJECT_NAME} PRIVATE paddle_full_api_shared)



