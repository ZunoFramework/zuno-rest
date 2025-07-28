
# Add dependencies here
find_path(LIBPSL_INCLUDE_DIR libpsl.h PATHS /opt/homebrew/include)
find_library(LIBPSL_LIBRARY psl PATHS /opt/homebrew/lib)

add_dependency(nlohmann_json https://github.com/nlohmann/json.git v3.11.2)
add_dependency(libcurl https://github.com/curl/curl.git master)

