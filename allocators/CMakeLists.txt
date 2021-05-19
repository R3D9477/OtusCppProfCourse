#------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME "allocators")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#------------------------------------------------------------------------------------------------------

if($ENV{GITHUB_RUN_NUMBER})
    project(${PROJECT_NAME} LANGUAGES CXX VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})
else()
    project(${PROJECT_NAME} LANGUAGES CXX VERSION 0.0.1)
endif()

add_executable(${PROJECT_NAME} main.cpp)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)

#------------------------------------------------------------------------------------------------------

add_library(VersionLib version_lib.cpp)
target_compile_options(VersionLib PRIVATE -Wall -Wextra -pedantic -Werror)
target_link_libraries(${PROJECT_NAME} PRIVATE VersionLib)

configure_file(version.h.in version.h)
target_include_directories(VersionLib PRIVATE "${CMAKE_BINARY_DIR}")

#------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

#------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
enable_testing()

#------------------------------------------------------------------------------------------------------

add_executable(test_version test_version.cpp)
set_target_properties(test_version PROPERTIES COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
target_compile_options(test_version PRIVATE -Wall -Wextra -pedantic -Werror)
target_link_libraries(test_version ${Boost_LIBRARIES} VersionLib)
add_test(test_version test_version)

#------------------------------------------------------------------------------------------------------

add_executable(test_factorial test_factorial.cpp)
set_target_properties(test_factorial PROPERTIES COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
target_compile_options(test_factorial PRIVATE -Wall -Wextra -pedantic -Werror)
target_link_libraries(test_factorial ${Boost_LIBRARIES})
add_test(test_factorial test_factorial)

#------------------------------------------------------------------------------------------------------

add_executable(test_lalloc10 test_lalloc10.cpp)
set_target_properties(test_lalloc10 PROPERTIES COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
target_compile_options(test_lalloc10 PRIVATE -Wall -Wextra -pedantic -Werror)
target_link_libraries(test_lalloc10 ${Boost_LIBRARIES})
add_test(test_lalloc10 test_lalloc10)

#------------------------------------------------------------------------------------------------------

add_executable(test_customcontainer test_customcontainer.cpp)
set_target_properties(test_customcontainer PROPERTIES COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
target_compile_options(test_customcontainer PRIVATE -Wall -Wextra -pedantic -Werror)
target_link_libraries(test_customcontainer ${Boost_LIBRARIES})
add_test(test_customcontainer test_customcontainer)

#------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------
