cmake_minimum_required(VERSION 2.8)

# Set project filename
project(${PROJECT_NAME} Task1)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include_directories(${OpenCV_INCLUDE_DIRS} include)

add_executable(${PROJECT_NAME} main.cpp thinning.cpp include/thinning.h)

# add libs you need
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

# linking
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})