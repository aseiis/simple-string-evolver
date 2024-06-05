cmake_minimum_required(VERSION 3.19)

project(genetic-prem LANGUAGES CXX)

add_executable( genetic-prem
    src/main.cpp
    src/Individual.cpp
    src/Population.cpp
)