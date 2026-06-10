cmake_minimum_required(VERSION 3.10)
project(SimulationPlatform VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/msvc2022_64")
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

add_executable(SimulationPlatform main.cpp 
				mainwindow.ui
				simulation/SimulationWorker.h 
				simulation/SimulationWorker.cpp
				core/Vector.cpp 
				core/RigidBody.cpp
				core/Environment.cpp
				core/Target.cpp "MainWindow.cpp") 

target_include_directories(SimulationPlatform PRIVATE core)
target_link_libraries(SimulationPlatform PRIVATE Qt6::Core Qt6::Widgets)

if(WIN32)
    get_target_property(_qt_bindir Qt6::Widgets LOCATION)
    get_filename_component(_qt_bindir "${_qt_bindir}" DIRECTORY)

    add_custom_command(TARGET SimulationPlatform POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${_qt_bindir}/Qt6Core.dll"
        "${_qt_bindir}/Qt6Widgets.dll"
        "${_qt_bindir}/Qt6Gui.dll"
        $<TARGET_FILE_DIR:SimulationPlatform>
    )
endif()