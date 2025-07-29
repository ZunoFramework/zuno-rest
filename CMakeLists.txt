cmake_minimum_required(VERSION 3.20)

project(zuno-rest VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POLICY_VERSION_MINIMUM 3.20)

enable_testing()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

include(cmake/AddDependency.cmake)
include(cmake/Dependencies.cmake)

set(SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RestClient.cpp
)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/zuno/RestClient.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/zuno/RequestInterceptor.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/zuno/ResponseInterceptor.hpp
)

# Define and configure library
add_library(zuno-rest STATIC ${SRC_FILES} ${HEADER_FILES})
set_target_properties(zuno-rest PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output"
  OUTPUT_NAME "zuno-rest"
)

# Include headers
target_include_directories(zuno-rest PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(zuno-rest PRIVATE ${LIBPSL_INCLUDE_DIR})
target_link_libraries(zuno-rest PRIVATE libcurl nlohmann_json ${LIBPSL_LIBRARY})

# Example executables
add_executable(zuno-rest-example
    ${CMAKE_CURRENT_SOURCE_DIR}/example/example.cpp
)
target_link_libraries(zuno-rest-example PRIVATE zuno-rest libcurl nlohmann_json ${LIBPSL_LIBRARY})

add_executable(zuno-rest-example-async
    ${CMAKE_CURRENT_SOURCE_DIR}/example/example_async.cpp
)
target_link_libraries(zuno-rest-example-async PRIVATE zuno-rest libcurl nlohmann_json ${LIBPSL_LIBRARY})
