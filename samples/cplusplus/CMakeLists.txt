cmake_minimum_required(VERSION 3.14)
project(cplusplus)

set(CMAKE_CXX_STANDARD 17)

add_executable(bseearch
        bsearch.cc)

add_executable(getloadavg
        getloadavg.cc)

add_executable(lambda
        lambda.cc)

add_executable(pthread_cleanup
        pthread_cleanup.cc)

