cmake_minimum_required(VERSION 3.16)
project(RenderdocBrownbag VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set compiler flags with debug information
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /Zi")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -g")
endif()

# Define examples
set(EXAMPLES
    0_HelloTriangle
    1_VertexBuffer
    2_TextureMapping
    3_Compute
    4_ComputeSkinning
)

# Option to build all examples (default: ON)
option(BUILD_ALL_EXAMPLES "Build all examples" ON)

# Optional RenderDoc capture support
option(ENABLE_RENDERDOC_CAPTURE "Enable RenderDoc frame capture" OFF)
set(RENDERDOC_INCLUDE_DIR "" CACHE PATH "Path to renderdoc_app.h")

# DONOTCHECKIN
set(ENABLE_RENDERDOC_CAPTURE OFF)
set(RENDERDOC_INCLUDE_DIR "")

# Option to select which example to build (used only if BUILD_ALL_EXAMPLES is OFF)
set(EXAMPLE "0_HelloTriangle" CACHE STRING "Example to build when BUILD_ALL_EXAMPLES is OFF")
set_property(CACHE EXAMPLE PROPERTY STRINGS ${EXAMPLES})

# Find Vulkan package
find_package(Vulkan REQUIRED)

# Add GLFW for window creation
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# Add GLM for mathematics
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

# Add stb_image for texture loading
include(FetchContent)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)
FetchContent_MakeAvailable(stb)

# Set common source files
set(COMMON_SOURCES
    common/vulkan_app.cpp
    common/vulkan_compute_app.cpp
)

# Set common header files
set(COMMON_HEADERS
    common/vulkan_app.h
    common/vulkan_compute_app.h
)

# Create common library
add_library(vulkan_common STATIC ${COMMON_SOURCES} ${COMMON_HEADERS})

# Link common libraries
target_link_libraries(vulkan_common PUBLIC
    Vulkan::Vulkan
    glfw
    glm
)

# Include directories for common library
target_include_directories(vulkan_common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${Vulkan_INCLUDE_DIRS}
    ${stb_SOURCE_DIR}
)

# Add compile definitions for common library
target_compile_definitions(vulkan_common PUBLIC
    VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
)

# Function to add an example
function(add_example name)
    # Set source files for the example
    set(EXAMPLE_SOURCES
        examples/${name}/main.cpp
    )

    # Create executable
    add_executable(${name} ${EXAMPLE_SOURCES})

    # Link libraries
    target_link_libraries(${name} PRIVATE
        vulkan_common
    )

    # Include directories
    target_include_directories(${name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/${name}
        ${stb_SOURCE_DIR}
    )

    if(ENABLE_RENDERDOC_CAPTURE)
        target_compile_definitions(${name} PRIVATE ENABLE_RENDERDOC_CAPTURE)
        if(RENDERDOC_INCLUDE_DIR)
            target_include_directories(${name} PRIVATE ${RENDERDOC_INCLUDE_DIR})
        endif()
    endif()

    # Copy shader files to build directory
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/examples/${name}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)

    # Copy texture files to build directory if they exist
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples/${name}/textures)
        file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/examples/${name}/textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
    endif()

    # Set output directory
    set_target_properties(${name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endfunction()

# Add examples based on configuration
if(BUILD_ALL_EXAMPLES)
    # Build all examples
    foreach(example ${EXAMPLES})
        add_example(${example})
        message(STATUS "Adding example: ${example}")
    endforeach()
    message(STATUS "Building all examples")
else()
    # Build only the selected example
    if(EXAMPLE IN_LIST EXAMPLES)
        add_example(${EXAMPLE})
        message(STATUS "Building example: ${EXAMPLE}")
    else()
        message(FATAL_ERROR "Unknown example: ${EXAMPLE}")
    endif()
endif()

# Print information
message(STATUS "Vulkan_FOUND: ${Vulkan_FOUND}")
message(STATUS "Vulkan_INCLUDE_DIRS: ${Vulkan_INCLUDE_DIRS}")
message(STATUS "Vulkan_LIBRARIES: ${Vulkan_LIBRARIES}")
