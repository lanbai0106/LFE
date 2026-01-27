cmake_minimum_required(VERSION 3.29)
project(LFE)

IF(CMAKE_COMPILER_IS_GNUCXX)
    ADD_COMPILE_OPTIONS(-O3)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        LFE main.cpp
        tool/BOBHash32.h
        tool/loadData.h
        report/report_M.h
        tool/common.h
        sketch/TowerSketch.h
        sketch/ElasticSketch.h
        sketch/CMSketch.h
        sketch/PRECISION.h
        sketch/CUSketch.h
        tool/Parameters_main.h
        tool/Parameters_sample_ratio.h
        report/report_thr.h

)