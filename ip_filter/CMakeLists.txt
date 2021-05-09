cmake_minimum_required(VERSION 3.5)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
  project(ip_filter LANGUAGES CXX)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
else()
  project(ip_filter LANGUAGES CXX VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})
  configure_file(version.h.in version.h)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(VersionLib version_lib.cpp)

add_executable(ip_filter ip_filter.cpp)

target_link_libraries(ip_filter VersionLib)

install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
