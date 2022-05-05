cmake_minimum_required(VERSION 2.8)

# Set project filename
project(${PROJECT_NAME} ImageStitching)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include_directories(${OpenCV_INCLUDE_DIRS} header)

add_executable(${PROJECT_NAME} code/main.cpp code/panoramic_utils.cpp)

# add libs you need
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs opencv_xfeatures2d)

# linking
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})