cmake_minimum_required(VERSION 3.6.0)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
AUX_SOURCE_DIRECTORY(cpp SRC)
set(INCLUDE_PATH ${PROJECT_SOURCE_DIR}/include)
include_directories(${INCLUDE_PATH})
add_library(blurimageview SHARED ${SRC} ${INCLUDE_PATH})
# add lib dependencies
target_link_libraries(blurimageview
                      android
                      log
                      EGL
                      GLESv3
                      jnigraphics)
