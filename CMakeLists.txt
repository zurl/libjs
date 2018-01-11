cmake_minimum_required(VERSION 3.9)
project(libjs)

set(CMAKE_CXX_STANDARD 11)

add_executable(libjs main.cpp
        js/common.h
        js/var.h
        js/var.cpp
        js/var_ref.h
        js/var_ref.cpp
        js/var_math.cpp
        js/js.h
        js/object.h
        js/function.h
        js/array.h
        js/exception.h
        )