cmake_minimum_required(VERSION 3.22)
project(Vacuum_cleaner_DFS)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Vacuum_cleaner_DFS
        abstract_algorithm.h
        battery_meter.h
        ConcreteSensors.cpp
        ConcreteSensors.h
        ConfigReader.h
        dirt_sensor.h
        enums.h
        House.cpp
        House.h
        main.cpp
        MyAlgorithm.cpp
        MyAlgorithm.h
        Simulation.cpp
        Simulation.h
        Vacuum.cpp
        Vacuum.h
        wall_sensor.h)
