cmake_minimum_required(VERSION 3.9)
project(RL2048 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

add_compile_options("-Wall" "-pedantic" "-Ofast" "-march=native" "-fomit-frame-pointer")

find_package(OpenMP REQUIRED)

file(GLOB SOURCES "headers/*.hpp" "src/*.cpp")

add_executable(RL2048 ${SOURCES})

target_link_libraries(RL2048 PUBLIC OpenMP::OpenMP_CXX)
