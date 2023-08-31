cmake_minimum_required(VERSION 3.7)

project(exercise1)


find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/glfw)


add_executable(exercise1 src/main.c)

target_link_libraries(exercise1 glfw ${OPENGL_LIBRARIES})
