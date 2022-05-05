cmake_minimum_required(VERSION 3.21)

# Set project filename
project(Task3)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)


#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include_directories(${OpenCV_INCLUDE_DIRS} include)

add_executable(Task3 main.cpp)

# add libs you need
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

# linking
target_link_libraries(Task3 ${OpenCV_LIBS})