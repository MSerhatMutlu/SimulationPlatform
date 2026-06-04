cmake_minimum_required(VERSION 3.10)
project(SimulationPlatform VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SimulationPlatform 
    main.cpp 
    core/Vector.cpp 
    core/RigidBody.cpp
    core/Environment.cpp 
    core/Target.cpp)

target_include_directories(SimulationPlatform PRIVATE core)
