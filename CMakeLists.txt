cmake_minimum_required(VERSION 4.1)
project(SoftRayTracer)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/../SoftRasterizer/src/math)
file(GLOB MATH_SRC
        ../SoftRasterizer/src/math/*.cpp
        ../SoftRasterizer/src/math/*.h
)
add_executable(
        SoftRayTracer main.cpp
        Color.h
        ${MATH_SRC}
        Ray.h
        Hittable.h
        Sphere.h
        HittableList.h
        Interval.h
        Camera.h
        Material.h
)
