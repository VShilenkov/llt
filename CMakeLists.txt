cmake_minimum_required(VERSION 3.12)

project(llt
    VERSION       0.0.1
    LANGUAGES     CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(llt)
target_sources(llt
    PRIVATE
        llt.cpp
)

target_include_directories(llt
    PRIVATE
        /home/vsh/Hearth/install_root/include
)

target_link_libraries(llt
    PRIVATE
        /home/vsh/Hearth/install_root/lib/libgmpxx.a
        /home/vsh/Hearth/install_root/lib/libgmp.a
#        gmpxx
#        gmp
)

target_compile_options(llt
    PRIVATE
#        -pg
#        -fno-omit-frame-pointer
        -march=native
)

#target_compile_definitions(llt
#    PRIVATE
#        PROGRESS
#)

#target_link_options(ll
#    PRIVATE
#        -pg
#)
