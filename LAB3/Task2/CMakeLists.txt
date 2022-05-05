cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)



# Set project filename
project(${PROJECT_NAME} Task2)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} include)

add_executable(${PROJECT_NAME} main.cpp)

# add libs you need
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

# linking
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})