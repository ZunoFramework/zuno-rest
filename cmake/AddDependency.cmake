function(add_dependency name repo tag)
    FetchContent_Declare(
        ${name}
        GIT_REPOSITORY ${repo}
        GIT_TAG        ${tag}
    )
    FetchContent_MakeAvailable(${name})
endfunction()