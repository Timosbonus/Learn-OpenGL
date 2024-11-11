cmake_minimum_required(VERSION 3.29)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)

# Include directories for header files
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Add glad as a static library
add_library(glad STATIC src/glad.c
        include/shader.h)
target_include_directories(glad PUBLIC include)

# Define the executable
add_executable(MyOpenGLProject src/main.cpp)

# Link the executable with glad, GLFW, and OpenGL
target_link_libraries(MyOpenGLProject glad glfw3 OpenGL::GL)

# Set output directory for the executable
set_target_properties(MyOpenGLProject PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Copy shader files to the build directory
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

