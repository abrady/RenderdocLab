# RenderdocBrownbag

A simple Vulkan application setup for demonstrating RenderDoc usage.

## Prerequisites

- CMake 3.16 or higher
- C++ compiler with C++17 support
- Vulkan SDK installed
- RenderDoc installed (for debugging)

## Building the Project

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/RenderdocBrownbag.git
   cd RenderdocBrownbag
   ```

2. Create a build directory and navigate to it:
   ```bash
   mkdir build
   cd build
   ```

3. Configure the project with CMake:
   ```bash
   cmake ..
   ```

4. Build the project:
   ```bash
   cmake --build .
   ```

## Running the Application

After building, you can run the application:

```bash
./bin/RenderdocBrownbag
```

## Using with RenderDoc

1. Launch RenderDoc
2. File -> Launch Application
3. Select the RenderdocBrownbag executable
4. Click Launch
5. Capture a frame by pressing F12 or using the RenderDoc UI
6. Analyze the captured frame in RenderDoc

## Project Structure

- `src/` - Source code files
  - `main.cpp` - Entry point
  - `vulkan_app.h` - Vulkan application header
  - `vulkan_app.cpp` - Vulkan application implementation
- `shaders/` - GLSL shader files
  - `shader.vert` - Vertex shader
  - `shader.frag` - Fragment shader
- `CMakeLists.txt` - CMake build configuration

## Features

- Basic Vulkan initialization
- Window creation with GLFW
- Swap chain setup
- Graphics pipeline configuration
- Simple triangle rendering
