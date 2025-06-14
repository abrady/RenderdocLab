# RenderdocBrownbag

A simple Vulkan application setup for demonstrating RenderDoc usage with multiple examples.

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
   # Build all examples (default)
   cmake ..

   # Or build only a specific example
   cmake .. -DBUILD_ALL_EXAMPLES=OFF -DEXAMPLE=0_HelloTriangle

   # Or build only the Vertex Buffer example
   cmake .. -DBUILD_ALL_EXAMPLES=OFF -DEXAMPLE=1_VertexBuffer
   ```

4. Build the project:
   ```bash
   cmake --build .
   ```

## Running the Examples

After building, you can run any of the examples:

```bash
# Run the Hello Triangle example
./bin/0_HelloTriangle

# Run the Vertex Buffer example
./bin/1_VertexBuffer
```

## Using with RenderDoc

1. Launch RenderDoc
2. File -> Launch Application
3. Select the example executable (e.g., `0_HelloTriangle` or `1_VertexBuffer`)
4. Click Launch
5. Capture a frame by pressing F12 or using the RenderDoc UI
6. Analyze the captured frame in RenderDoc

## Project Structure

- `common/` - Common code shared between examples
  - `vulkan_app.h` - Vulkan application header
  - `vulkan_app.cpp` - Vulkan application implementation
- `examples/` - Example applications
  - `0_HelloTriangle/` - Basic triangle rendering using hardcoded vertices
    - `main.cpp` - Entry point
    - `shaders/` - GLSL shader files
  - `1_VertexBuffer/` - Triangle rendering using vertex buffers
    - `main.cpp` - Entry point
    - `shaders/` - GLSL shader files
- `CMakeLists.txt` - CMake build configuration

## Examples

### 0_HelloTriangle

This example demonstrates the most basic Vulkan rendering setup:
- Basic Vulkan initialization
- Window creation with GLFW
- Swap chain setup
- Graphics pipeline configuration
- Simple triangle rendering with hardcoded vertices in the shader

### 1_VertexBuffer

This example builds on the Hello Triangle example and adds:
- Vertex buffer creation and usage
- Vertex input binding and attribute descriptions
- Passing vertex data from C++ to the shader

## Debugging with RenderDoc

RenderDoc is a powerful graphics debugging tool that allows you to:
- Capture frames from Vulkan applications
- Inspect draw calls, resources, and pipeline state
- View and edit shaders
- Analyze performance

When using RenderDoc with these examples, you can:
1. Inspect the vertex data
2. View the shader code
3. See the pipeline state
4. Analyze the rendered output

This is particularly useful for understanding how Vulkan works and for debugging rendering issues.
