#------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------
# MAIN PROJECT's SETTINGS

cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME "gveditor")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------
# COMPILATION

if($ENV{GITHUB_RUN_NUMBER})
    project(${PROJECT_NAME} LANGUAGES CXX VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})
else()
    project(${PROJECT_NAME} LANGUAGES CXX VERSION 0.0.1)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME} main.cpp)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

#------------------------------------------------------------------------------------------------------

add_library(VersionLib version_lib.cpp)
target_compile_options(VersionLib PRIVATE -Wall -Wextra -pedantic -Werror)
target_link_libraries(${PROJECT_NAME} PRIVATE VersionLib)

configure_file(version.h.in version.h)
target_include_directories(VersionLib PRIVATE "${CMAKE_BINARY_DIR}")

target_include_directories(VersionLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

#------------------------------------------------------------------------------------------------------

add_library(canvas canvas.cpp shape.cpp rectangle.cpp square.cpp triangle.cpp round.cpp)
target_compile_options(canvas PRIVATE -Wall -Wextra -pedantic -Werror)
target_link_libraries(${PROJECT_NAME} PRIVATE canvas)

#------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------
# TESTS

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
enable_testing()

#------------------------------------------------------------------------------------------------------

add_executable(test_version test_version.cpp)
set_target_properties(test_version PROPERTIES COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
target_compile_options(test_version PRIVATE -Wall -Wextra -pedantic -Werror)
target_link_libraries(test_version ${Boost_LIBRARIES} VersionLib)
add_test(test_version test_version)

#------------------------------------------------------------------------------------------------------



#------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------
# DEPLOYMENT

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

#------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------
# DOXYGEN

#option(BUILD_DOC "Build documentation" ON)

#find_package(Doxygen)

#if (DOXYGEN_FOUND)

#    set(DOXYGEN_IN  ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
#    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
#    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

#    message("Doxygen build started")

#    add_custom_target( doc_doxygen ALL
#        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
#        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
#        COMMENT "Generating API documentation with Doxygen"
#        VERBATIM
#    )

#else (DOXYGEN_FOUND)
#    message("Doxygen need to be installed to generate the doxygen documentation")

#endif (DOXYGEN_FOUND)
