cmake_minimum_required(VERSION 3.21)
project(Task4)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)


include_directories(${OpenCV_INCLUDE_DIRS} include)

add_executable(Task4 main.cpp)

# add libs you need
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

# linking
target_link_libraries(Task4 ${OpenCV_LIBS})