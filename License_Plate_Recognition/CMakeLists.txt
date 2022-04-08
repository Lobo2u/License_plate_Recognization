cmake_minimum_required(VERSION 2.8.4)
project(License_Plate_Recognition)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp preprocessing.cpp kNN.cpp refine.cpp SVM.cpp classify_objects.cpp compare_plate.cpp compare_plate.h)
add_executable(License_Plate_Recognition ${SOURCE_FILES})
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(License_Plate_Recognition ${OpenCV_LIBS})


