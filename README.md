# Renderdoc Lab

A simple Vulkan application setup for demonstrating RenderDoc usage with multiple examples.

## Prerequisites

- CMake 3.16 or higher
- C++ compiler with C++17 support
- Vulkan SDK installed
- RenderDoc installed (for debugging)

## Building the Project

Run the folowing commands in a shell that has your coding environment
set up. For example, open cmd and run `"C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat"` or whatever visual studio you have installed.

### Clone the repository

   ```pwsh
   git clone https://github.com/yourusername/RenderdocLab.git
   cd RenderdocLab
   ```

### Windows Environment Setup

![](Assets/Screenshots/WindowsEnvironment_VS.png)

- make sure you have C++ desktop development enabled in visual studio

### VS Code works well for build/debugging

1. run CMake: Configure - this creates the projects for compilation
2. run CMake: Build - compiles all the executables
3. CMake: Set Launch/Debug Target - let's you choose what to debug
4. CMake: Debug - debug the actual target.

### Or run cmake manually

1. cmake -S path_to_RenderdocLab -B path_to_RenderdocLab/build -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows

## Running the Examples

After building, you can run any of the examples in the build directory:

```pwsh
# Run the Hello Triangle example
.\bin\Debug\0_HelloTriangle.exe

# Run the Vertex Buffer example
.\bin\Debug\1_VertexBuffer.exe

# Run the Texture Mapping example
.\bin\Debug\2_TextureMapping.exe

# Run the Compute example
.\bin\Debug\3_Compute.exe

# Run the Compute Skinning example
.\bin\Debug\4_ComputeSkinning.exe
```

## Using with RenderDoc

1. Launch RenderDoc
2. File -> Launch Application
3. Select the example executable (e.g., `0_HelloTriangle`, `1_VertexBuffer`, `2_TextureMapping`, `3_Compute`, or `4_ComputeSkinning`)
4. Click Launch
5. Capture a frame by pressing F12 or using the RenderDoc UI
6. Analyze the captured frame in RenderDoc

The example runner compiles shaders on the fly using `glslc -g -O0`, so the
SPIR-V binaries contain debug information. When you open a capture in RenderDoc
you'll be able to see and step through the original GLSL source.

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

![](Assets/Screenshots/HelloTriangleApp.png)

#### Event Viewer

![](Assets/Screenshots/HelloTriangle_Events.png)

These show the calls you've made: just a single draw call

Selecting an event (like vkCmdDraw) lets you see what happened during that call.

#### Mesh Viewer

The mesh viewer shows you inputs and outputs of the vertex shader.

![](Assets/Screenshots/HelloTriangle_Mesh.png)

You can see here that there is no input mesh. that's becuase the triangle is drawn entirely by the shader.

![](Assets/Screenshots/HelloTriangle_MeshOut.png)

#### Debugging Verts

![](Assets/Screenshots/HelloTriangle_DebugVert.png)

you can debug the vertex shader for a given vertex by right clicking and choosing debug.

![](Assets/Screenshots/HelloTriangle_DebugDisasm.png)

By default you'll have the disassembly window open.

![](Assets/Screenshots/HelloTriangle_DebugSrc.png)

These examples also provide debug information so you can view the source of the shader.

![](Assets/Screenshots/HelloTriangle_DebuggerInUse.png)

#### Texture Viewer

![](Assets/Screenshots/0_HelloTriangle_TextuerViewer.png)

![](Assets/Screenshots/0_HelloTriangle_PixelDebug.png)

#### Debugging a pixel (fragment)

![](Assets/Screenshots/0_HelloTriangle_Disasm.png)

![](Assets/Screenshots/0_HelloTriangle_Src.png)

Similar controls to the above vertex debugger.

#### Debugging Fragments

And you can debug things pretty much like most debuggers.

#### Challenges

1. Debug into the vertex and fragment shaders and examine the variables.

### 1_VertexBuffer

This example builds on the Hello Triangle example and adds:

- Vertex buffer creation and usage
- Vertex input binding and attribute descriptions
- Passing vertex data from C++ to the shader

![](Assets/Screenshots/1_VertBufApp.png)

Visually it looks the same as 0_HelloTriangle. The difference is that this time the verts come from the CPU:

![](Assets/Screenshots/1_VertBuf_VertShader.png)

![](Assets/Screenshots/1_VertBuf_FragShader.png)

The vertex shader just passes through the position and color, and the fragment shader just uses the given color.

![](Assets/Screenshots/1_VertBuf_Ctor.png)

![](Assets/Screenshots/1_VertBuf_VertsCopied.png)

![](Assets/Screenshots/1_VertBuf_RecordDrawCmds.png)

![](Assets/Screenshots/1_VertBuf_MeshPanel_VertsIn.png)

Debugging this is basically identical to 0_HelloTriangle except for how the shader handles the input.

#### Pipeline State

![](Assets/Screenshots/1_VertBuf_PipelineState.png)

Now that we have some input, it is worth looking at the Pipeline State Panel. This panel is the top-level view of what's being rendered this call.

You can click on each of the pipeline stages to see more info. There's not much to see beyond the Vertex Input so we'll cover the other stages
in more detail later.

##### Vertex Input and Buffer Contents

As shown above, the first stage in the pipeline

![](Assets/Screenshots/1_VertBuf_ViewInputGo.png)

If you click this green arrow you can view the input contents:

![](Assets/Screenshots/1_VertBuf_InputContents.png)

##### Rasterizer

![](Assets/Screenshots/1_VertBuf_RasterizerStage.png)

Various fixed-function and state info that may be useful in some situations.

##### Frame Buffer

![](Assets/Screenshots/1_VertBuf_FrameBufStage.png)

The only interesting thing here is that you can see what the final image looks like if you choose.

#### Challenges

1. Change the code to draw a second triangle, then follow it being drawn in renderdoc

### 2_TextureMapping

This example builds on the previous examples and demonstrates:

- Texture loading and uploading to the GPU
- Creating image views and samplers
- Using descriptor sets to bind textures to shaders
- UV coordinates in vertex data
- Sampling textures in fragment shaders
- Index buffer usage for efficient rendering
- Procedural texture generation

![](Assets/Screenshots/2_Tex_App.png)

This is the first example with enough complexity to dive into a few different things:

#### Vertex Input

![](Assets/Screenshots/2_Tex_VertInput.png)

- a vertex and index buffer
- the input vertex now contains a UV texture coord

![](Assets/Screenshots/InputBufContents.png)

#### Mesh Viewer

![](Assets/Screenshots/2_Tex_MeshViewerHighlight.png)

The mesh viewer will highlight the triangle that is being drawn as part of what you've selected now that we have more than one triangle

#### Fragment Shader

![](Assets/Screenshots/2_Tex_FragShaderStage.png)

The fragment shader now has a texture bound to it

#### TextureMapping Challenges

1. Step through this in the debugger and make sure you understand what is bound to the vertex and fragment shaders and how sampling works.
2. Add color back to each vertex

### 3_Compute

This example demonstrates a minimal compute pipeline:

- Creates a storage buffer filled with numbers
- Dispatches a compute shader that doubles each value
- Reads back and prints the results to the console

![](Assets/Screenshots/3_Comp_Cmdline.png)

Nothing visual about this one: it just doubles each value of the inputs.

One thing worth showing in this example is the in-app renderdoc api: the nutshell is that
if you launch an app with renderdoc, the renderdoc.dll will be injected and you can query it with
`GetModuleHandleA("renderdoc.dll");`

#### Enabling RenderDoc Capture in the Compute Example

The `3_Compute` example contains optional code that programmatically triggers a
RenderDoc capture around the compute dispatch. To enable this feature during the
build, configure CMake with the following options:

```bash
cmake .. -DENABLE_RENDERDOC_CAPTURE=ON -DRENDERDOC_INCLUDE_DIR=/path/to/renderdoc/include
```

`RENDERDOC_INCLUDE_DIR` should point to the directory containing
`renderdoc_app.h` (for example `C:/Program Files/RenderDoc/include` on Windows or
`/usr/include` on Linux). When enabled, running the compute example will produce
a capture without requiring manual interaction in the RenderDoc UI.

#### Debugging Compute Shaders

![](Assets/Screenshots/3_Compute_Pipeline.png)

The compute stage can be found in the Pipeline State panel on the far right. You can choose to debug from this view

![](Assets/Screenshots/3_Compute_Buffers.png)

You can see the input and output buffers here, and view their contents.

![](Assets/Screenshots/3_Compute_DebugDialog.png)

If you click debug you'll get this confusing dialog.

A compute dispatch is potentially organized into 1, 2, or 3 dimensions
depending on the local_size_x/y/z values in the shader. this project won't cover
the details any further but imagine it might be convenient to operate on
16x16 tiles with hyper-fast local memory and you can see why it'd
be convenient to break up a compute shader along multiple dimensions.

For this example, the first value (x) is all you need, e.g. x=11.

#### Compute Challenges

1. Debug the compute shader and observe the inputs and outputs for the given thread are as expected
2. change the dimensionality of the shader (e.g. change local_size_x/y/z) and get it working. Explore
   the gl_LocalInvocationID and gl_WorkGroupID variables to see that you have an understanding.

### 4_ComputeSkinning

This example shows how compute shaders can be used for vertex skinning:

- Uses a compute shader to transform quad vertices with two bone matrices
- Copies the skinned results into a vertex buffer
- Renders the textured quad using the skinned positions

![](Assets/Screenshots/4_Skin_App.png)

This example just combines elements from earlier ones into something
similar to what you'll find in an real game: most games will have a compute
pass of some kind, usually to do skinning, and then will render various things.

#### Challenges

1. there's a bug in the skinning code for some of the faces where they get skinned incorrectly. debug and fix this.
