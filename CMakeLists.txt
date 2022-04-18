cmake_minimum_required(VERSION 3.12)

project(llt
    VERSION       0.0.1
    LANGUAGES     CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include(FeatureSummary)

option(WITH_PROGRESS "Display progress bar in run-time" false)
add_feature_info([${PROJECT_NAME}].display_progress WITH_PROGRESS
        "Display progress bar in run-time"
)

option(WITH_GPROF "Gemerate profiling output suitable for gprof" false)
add_feature_info([${PROJECT_NAME}].generate_gprof WITH_GPROF
        "Gemerate profiling output suitable for gprof"
)

add_feature_info([${PROJECT_NAME}].custom_gmp CUSTOM_GMP
        "Using precompiled custom gmp library"
)

add_executable(llt)
target_sources(llt
    PRIVATE
        llt.cpp
)

target_include_directories(llt
    PRIVATE
        include
)

# TODO:VSH: handle custom libs properly with find_package
if(DEFINED CUSTOM_GMP)
    target_include_directories(llt
        PRIVATE
            external/libgmp/${CUSTOM_GMP}/include
    )
    target_link_libraries(llt
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/external/libgmp/${CUSTOM_GMP}/lib/libgmpxx.a
            ${CMAKE_CURRENT_LIST_DIR}/external/libgmp/${CUSTOM_GMP}/lib/libgmp.a
    )
else()
    target_link_libraries(llt
        PRIVATE
            gmpxx
            gmp
    )
endif()

target_compile_options(llt
    PRIVATE
        -march=native
)

if(WITH_PROGRESS)
    target_compile_definitions(llt
        PRIVATE
            PROGRESS
    )
endif()

# TODO:VSH: add interface library and specific targets to generate readable output
if(WITH_GPROF)
    target_compile_options(llt
        PRIVATE
            -pg
            -fno-omit-frame-pointer
    )

    target_link_options(llt
        PRIVATE
            -pg
    )
endif()

feature_summary(WHAT ALL
    DESCRIPTION "-- [${PROJECT_NAME} summary] ---------------------------------"
)