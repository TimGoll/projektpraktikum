cmake_minimum_required(VERSION 3.6)
project(V0_0_1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp labCom.cpp labCom.h mfcCtrl.cpp mfcCtrl.h valveCtrl.cpp valveCtrl.h StroreD.cpp StoreD.h boschCom.cpp boschCom.h)
add_executable(V0_0_1 ${SOURCE_FILES})