cmake_minimum_required(VERSION 3.12)

project(ll
    VERSION       0.0.1
    LANGUAGES     CXX C ASM
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(ll)
target_sources(ll
    PRIVATE
        ll.cpp
)

target_include_directories(ll
    PRIVATE
        /home/vsh/Hearth/install_root/include
)

target_link_libraries(ll
    PRIVATE
        /home/vsh/Hearth/install_root/lib/libgmpxx.a
        /home/vsh/Hearth/install_root/lib/libgmp.a
#        gmpxx
#        gmp
)

target_compile_options(ll
    PRIVATE
#        -pg
#        -fno-omit-frame-pointer
        -march=native
)

#target_link_options(ll
#    PRIVATE
#        -pg
#)
