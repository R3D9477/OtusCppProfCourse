#------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------
# MAIN PROJECT's SETTINGS

cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME "gveditor")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------
# EXECUTABLE

if($ENV{GITHUB_RUN_NUMBER})
    project(${PROJECT_NAME} LANGUAGES CXX VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})
else()
    project(${PROJECT_NAME} LANGUAGES CXX VERSION 0.0.1)
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(${PROJECT_NAME} "src/main.cpp")
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

#------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------
# LIBRARIES

set(INC_VER "lib/version/include")
set(SRC_VER "lib/version/src")

add_library(
    version
    "${SRC_VER}/version_lib.cpp"
)

target_include_directories(version PUBLIC ${INC_VER})
target_compile_options(version PRIVATE -Wall -Wextra -pedantic -Werror)
target_link_libraries(${PROJECT_NAME} PRIVATE version)

configure_file("${INC_VER}/version.h.in"   "${CMAKE_BINARY_DIR}/version.h")
target_include_directories(version PRIVATE "${CMAKE_BINARY_DIR}")
target_include_directories(version PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

#------------------------------------------------------------------------------------------------------

set(INC_SHAPE "lib/shape/include")
set(SRC_SHAPE "lib/shape/src")

add_library(
    shape
    "${SRC_SHAPE}/shape.cpp"
    "${SRC_SHAPE}/rectangle.cpp"
    "${SRC_SHAPE}/square.cpp"
    "${SRC_SHAPE}/round.cpp"
    "${SRC_SHAPE}/triangle.cpp"
    "${SRC_SHAPE}/shapeserializer.cpp"
)

target_include_directories(shape PUBLIC ${INC_SHAPE})
target_compile_options(shape PRIVATE -Wall -Wextra -pedantic -Werror)
target_link_libraries(${PROJECT_NAME} PRIVATE shape)

#------------------------------------------------------------------------------------------------------

set(INC_PICTURE "lib/picture/include")
set(SRC_PICTURE "lib/picture/src")

add_library(
    picture
    "${SRC_PICTURE}/picture.cpp"
)

target_include_directories(picture PUBLIC ${INC_SHAPE} ${INC_PICTURE})
target_compile_options(picture PRIVATE -Wall -Wextra -pedantic -Werror)
target_link_libraries(${PROJECT_NAME} PRIVATE picture)

#------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------
# TESTS

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
enable_testing()

#------------------------------------------------------------------------------------------------------

set(SRC_TEST_VER "lib/version/test")

add_executable(
   test_version
   "${SRC_TEST_VER}/test_version.cpp"
)

target_include_directories(test_version PUBLIC ${INC_VER})
set_target_properties(test_version PROPERTIES COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
target_compile_options(test_version PRIVATE -Wall -Wextra -pedantic -Werror)
target_link_libraries(test_version PRIVATE ${Boost_LIBRARIES} version)

add_test(test_version test_version)

#------------------------------------------------------------------------------------------------------

set(SRC_TEST_SHAPE "lib/shape/test")

add_executable(
   test_shape
   "${SRC_TEST_SHAPE}/test_shape.cpp"
)

target_include_directories(test_shape PUBLIC ${INC_SHAPE})
set_target_properties(test_shape PROPERTIES COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
target_compile_options(test_shape PRIVATE -Wall -Wextra -pedantic -Werror)
target_link_libraries(test_shape PRIVATE ${Boost_LIBRARIES} shape)

add_test(test_shape test_shape)

#------------------------------------------------------------------------------------------------------

set(SRC_TEST_PICTURE "lib/picture/test")

add_executable(
   test_picture
   "${SRC_TEST_PICTURE}/test_picture.cpp"
)

target_include_directories(test_picture PUBLIC ${INC_PICTURE})
set_target_properties(test_picture PROPERTIES COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
target_compile_options(test_picture PRIVATE -Wall -Wextra -pedantic -Werror)
target_link_libraries(test_picture PRIVATE ${Boost_LIBRARIES} picture)

add_test(test_picture test_picture)

#------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------
# DEPLOYMENT

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT r3d9477@gmail.com)

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
