
# Add dependencies here
find_path(LIBPSL_INCLUDE_DIR NAMES libpsl.h PATHS ENV LIBPSL_INCLUDE_DIR)
find_library(LIBPSL_LIBRARY NAMES psl PATHS ENV LIBPSL_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Libpsl REQUIRED_VARS LIBPSL_INCLUDE_DIR LIBPSL_LIBRARY)

add_dependency(nlohmann_json https://github.com/nlohmann/json.git v3.11.2)
add_dependency(libcurl https://github.com/curl/curl.git master)

