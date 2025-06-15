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

   # Or build only the Texture Mapping example
   cmake .. -DBUILD_ALL_EXAMPLES=OFF -DEXAMPLE=2_TextureMapping

   # Or build only the Compute example
   cmake .. -DBUILD_ALL_EXAMPLES=OFF -DEXAMPLE=3_Compute

   # Or build only the Compute Skinning example
   cmake .. -DBUILD_ALL_EXAMPLES=OFF -DEXAMPLE=4_ComputeSkinning
   ```

4. Build the project:

   ```bash
   cmake --build .
   ```

## Running the Examples

After building, you can run any of the examples in the build directory:

```bash
# Run the Hello Triangle example
./bin/0_HelloTriangle

# Run the Vertex Buffer example
./bin/1_VertexBuffer

# Run the Texture Mapping example
./bin/2_TextureMapping

# Run the Compute example
./bin/3_Compute

# Run the Compute Skinning example
./bin/4_ComputeSkinning
```

## Using with RenderDoc

1. Launch RenderDoc
2. File -> Launch Application
3. Select the example executable (e.g., `0_HelloTriangle`, `1_VertexBuffer`, `2_TextureMapping`, `3_Compute`, or `4_ComputeSkinning`)
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
  - `2_TextureMapping/` - Textured quad rendering using vertex and index buffers
    - `main.cpp` - Entry point
    - `shaders/` - GLSL shader files
  - `3_Compute/` - Minimal compute shader example
    - `main.cpp` - Entry point
    - `shaders/` - GLSL compute shader
  - `4_ComputeSkinning/` - Compute skinning using a texture-mapped quad
    - `main.cpp` - Entry point
    - `shaders/` - Vertex, fragment, and compute shaders
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

### 2_TextureMapping

This example builds on the previous examples and demonstrates:

- Texture loading and uploading to the GPU
- Creating image views and samplers
- Using descriptor sets to bind textures to shaders
- UV coordinates in vertex data
- Sampling textures in fragment shaders
- Index buffer usage for efficient rendering
- Procedural texture generation

### 3_Compute

This example demonstrates a minimal compute pipeline:

- Creates a storage buffer filled with numbers
- Dispatches a compute shader that doubles each value
- Reads back and prints the results to the console

### 4_ComputeSkinning

This example shows how compute shaders can be used for vertex skinning:

- Uses a compute shader to transform quad vertices with two bone matrices
- Copies the skinned results into a vertex buffer
- Renders the textured quad using the skinned positions

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
5. Examine texture data and sampling
6. Debug UV mapping issues

This is particularly useful for understanding how Vulkan works and for debugging rendering issues.
