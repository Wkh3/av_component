function(Custom_Fetch repository url tag)
    FetchContent_Declare(
        ${repository}
        GIT_REPOSITORY ${url}
        GIT_TAG ${tag}
    )

    FetchContent_GetProperties(${repository})
    
    if(NOT ${repository}_POPULATED)
        FetchContent_Populate(${repository})
        file(GLOB_RECURSE _headers "${${repository}_SOURCE_DIR}/include/*.h")
        file(GLOB_RECURSE _sources "${${repository}_SOURCE_DIR}/source/*.cc")
        add_library(${repository} ${_headers} ${_sources})
        set_target_properties(${repository} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${${repository}_BINARY_DIR}")
        target_include_directories(${repository} PUBLIC "${${repository}_SOURCE_DIR}/include")
    endif()

endfunction()

