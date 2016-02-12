cmake_minimum_required(VERSION 2.8)
# Project Name
PROJECT(gleem)

#Making the compiler as strict as possible
set(CMAKE_CXX_FLAGS "-std=c++14 -fno-elide-constructors -pedantic-errors -Werror -Wextra -Wall -Wpedantic -Winit-self -Wmissing-declarations -Wuninitialized -Woverloaded-virtual -Wold-style-cast -Wfatal-errors -O3")
add_definitions(${CMAKE_CXX_FLAGS})

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
  message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
  message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)


file(GLOB SOURCE src/*.cpp include/*.h)


#########################################################
# Include Files
#########################################################
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE})

########################################################
# Linking & stuff
#########################################################

# create the program "test"
target_link_libraries(${CMAKE_PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
