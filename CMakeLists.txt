cmake_minimum_required(VERSION 3.10)
project(DNSCache)

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} main.cpp dns_cache.cpp)

target_link_libraries(${PROJECT_NAME} pthread)