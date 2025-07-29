# CMakeLists.txt
# CMake minimum version
cmake_minimum_required(VERSION 3.20)

# Project name and version
project(zuno-rest VERSION 0.1.0)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POLICY_VERSION_MINIMUM 3.20)

# Enable testing
enable_testing()

# Enable compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# FetchContent for dependencies
include(FetchContent)

# Include the dependency management files
include(cmake/AddDependency.cmake)
include(cmake/Dependencies.cmake)

# Define SRC_FILES 
set(SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RestClient.cpp
)

# Define HEADER_FILES
set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/zuno/RestClient.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/zuno/RequestInterceptor.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/zuno/ResponseInterceptor.hpp
)

# Add library
add_library(zuno-rest STATIC ${SRC_FILES} ${HEADER_FILES})

# Include directories
target_include_directories(zuno-rest PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies to zuno-rest
target_include_directories(zuno-rest PRIVATE ${LIBPSL_INCLUDE_DIR})
target_link_libraries(zuno-rest PRIVATE libcurl nlohmann_json ${LIBPSL_LIBRARY})

# Add example executable
add_executable(zuno-rest-example
    ${CMAKE_CURRENT_SOURCE_DIR}/example/example.cpp
)
add_executable(zuno-rest-example-async
    ${CMAKE_CURRENT_SOURCE_DIR}/example/example_async.cpp
)

# Link zuno-rest library to the example executable
target_link_libraries(zuno-rest-example PRIVATE zuno-rest libcurl nlohmann_json ${LIBPSL_LIBRARY})

# Link zuno-rest library to the example executable
target_link_libraries(zuno-rest-example-async PRIVATE zuno-rest libcurl nlohmann_json ${LIBPSL_LIBRARY})
