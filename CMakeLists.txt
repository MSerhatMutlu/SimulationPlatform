cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SimulationPlatform VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/msvc2022_64")

find_package(Threads REQUIRED)
set(OpenCV_DIR "C:/OpenCV/build")
find_package(OpenCV REQUIRED)
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

add_executable(SimulationPlatform 
    main.cpp 
    Mainwindow.ui
    MainWindow.cpp
    simulation/SimulationWorker.h 
    simulation/SimulationWorker.cpp
    core/Vector.cpp 
    core/RigidBody.cpp
    core/Environment.cpp
    core/Target.cpp
    core/VirtualCamera.h
    core/VirtualCamera.cpp)

target_compile_features(SimulationPlatform PRIVATE cxx_std_20)

target_include_directories(SimulationPlatform PRIVATE
                            /core
                            /simulation
                            ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(SimulationPlatform PRIVATE 
    Qt6::Core 
    Qt6::Widgets
    Threads::Threads
    opencv_world
)