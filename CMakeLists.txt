cmake_minimum_required(VERSION 4.1)
project(Yadro_test_task)

set(CMAKE_CXX_STANDARD 20)

add_executable(Yadro_test_task 
        include/BaseStructs.h
        include/Client.h
        include/ClientTime.h
        include/Club.h
        include/Constants.h
        include/EventHandlers.h
        include/InputParser.h
        include/Table.h
        
        src/Client.cpp
        src/ClientTime.cpp
        src/Club.cpp
        src/EventHandlers.cpp
        src/InputParser.cpp
        src/Table.cpp
        src/main.cpp
)

target_include_directories(Yadro_test_task PUBLIC "${CMAKE_SOURCE_DIR}/include")
