cmake_minimum_required(VERSION 3.14.0)

# Project information
project(main VERSION 0.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Release)

# Set the compile options
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Always generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# Third-party libraries
# GLFW for windowing
add_subdirectory(external/glfw-3.3.9)

# GLM
add_subdirectory(external/glm)

# GLAD for OpenGL function loading
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PRIVATE external/glad/include)

# Assimp
add_subdirectory(external/assimp)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# ImGui setup
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui-1.90.4)
file(GLOB IMGUI_SOURCES ${IMGUI_DIR}/src/*.cpp)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_DIR}/include ${IMGUI_DIR}/backends)
target_link_libraries(imgui PUBLIC ${OPENGL_gl_LIBRARY} glfw)

# ImGui GLFW and OpenGL3 implementations
target_sources(imgui PUBLIC 
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# OpenAL
#add_subdirectory(external/openal-soft-master)

# Find and link OpenGL
find_package(OpenGL REQUIRED)

# Executable definition
file(GLOB PROJECT_SOURCES "src/*.cpp")
add_executable(main ${PROJECT_SOURCES})

target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    external/glfw-3.3.9/include
    external/glad/include
    external/assimp/include
    #external/openal-soft-master/include/AL
    external
)

# Add OpenGL to the target link libraries
target_link_libraries(main glfw glad imgui assimp ${OPENGL_LIBRARIES})

target_compile_definitions(main PRIVATE GLM_ENABLE_EXPERIMENTAL)

set_target_properties(glad imgui PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
