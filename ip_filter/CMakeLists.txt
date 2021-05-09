cmake_minimum_required(VERSION 3.5)

project(ip_filter VERSION LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ip_filter ip_filter.cpp)

install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
