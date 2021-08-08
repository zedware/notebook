cmake_minimum_required(VERSION 3.10)
project(template)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ASan includes LSan, to use LSan stand-alone, replace it with -fsanitize=leak
# Add -static-libasan to avoid error in VScode debug.
# Add -O1 to coredump if used strdup which it not in C99.
# set(COMMON_FLAGS "-O0 -Wall -Wextra -g -fsanitize=address -static-libasan -fno-omit-frame-pointer")
set(COMMON_FLAGS "-O1 -Wall -Wextra -g -fno-omit-frame-pointer")

set(CMAKE_C_FLAGS ${COMMON_FLAGS})
set(CMAKE_CXX_FLAGS ${COMMON_FLAGS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/)

add_executable(template template.c)
# target_include_directories(template PUBLIC x/include)
# target_link_libraries(template x.a)
