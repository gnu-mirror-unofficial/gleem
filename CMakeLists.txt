cmake_minimum_required(VERSION 2.8.11)
project(nibbles)

# State directories for modules and binaries
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

#Making the compiler as strict as possible
set(CMAKE_CXX_FLAGS "-std=c++14 -fno-elide-constructors -pedantic-errors -Werror -Wextra -Wall -Wpedantic -Winit-self -Wmissing-declarations -Wuninitialized -Woverloaded-virtual -Wold-style-cast -Wfatal-errors -O3")
add_definitions(${CMAKE_CXX_FLAGS})

file(GLOB SOURCE src/*.cpp src/*.h)

find_package(OpenGL REQUIRED)
find_package(GLUT)

# Specify include directories, we need to set the include directory as an include dir
include_directories(
  ${GLUT_INCLUDE_DIR}
  ${OpenGL_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/src
  )

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE})

target_link_libraries(${CMAKE_PROJECT_NAME}
  ${GLUT_LIBRARIES}
  ${OpenGL_LIBRARIES}
  )
