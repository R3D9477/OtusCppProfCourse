cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(libasync_shr_test LANGUAGES CXX)
add_executable(libasync_shr_test main.cpp)


set(LIB_DIR "/home/eugene/Desktop/OTUS_CppProfCourse/homework/libasync/bin")

add_library(libaysnc SHARED IMPORTED)
set_property(TARGET libaysnc PROPERTY IMPORTED_LOCATION "${LIB_DIR}/libasync.so")

target_link_libraries(libasync_shr_test libaysnc)
target_include_directories(libasync_shr_test PUBLIC "${LIB_DIR}")
