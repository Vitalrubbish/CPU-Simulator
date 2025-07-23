cmake_minimum_required(VERSION 3.22)
project(code)

set(CMAKE_CXX_STANDARD 14)

include_directories(CPU-Simulator)

add_executable(code
    include/Instruction.h
    include/ALU.h
    include/InputManager.h
    include/Memory.h
    include/Register.h
    include/ROB.h
    src/Instruction.cpp
    src/ALU.cpp
    utility/utility.h
    main.cpp
    include/RS.h
    include/LSB.h
    include/Execute.h
        include/Predictor.h
)
