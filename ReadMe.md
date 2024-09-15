# 3D Graphics Demo

**Team Name:** jjamptong  
**Members:** Sunwoo Lee, Donga Choi

## How to Run
- To run on Visual Studio 2019, open the solution file (`opengl-dev.sln`) located in the `jjamptong` folder. Change the start projects and build & run with the **Release** configuration.
- The `EXE` folder contains three pre-built executables along with the necessary files.
- You can experiment with different values using the **IMGUI check box** in each demo.

## Projects and Executables

### 1. **tutorial-1 project / tutorial-1.exe**
- **Procedural Geometric Modeling:**
  - Shapes: Plane, Cube, Sphere, Torus, Cylinder, and Cone
- **Toon / Cell Shading & Fog**
  - Linear and Exponential
- **Value Noise:**
  - Implemented in C++ to generate textures
  - Used texture mapping
- **Gradient Noise / Perlin Noise:**
  - Implemented in C++ to generate terrain meshes
- **Hermite Curves / Catmull-Rom Splines**

### 2. **Geometry+Tessellation project / Geometry+Tessellation.exe**
- Geometry Shaders
    - Grass rendered with geometry shader
    - Using random value to make each grass face / edge point different directions
- Tessellation Shaders
    - To adjust density of grass

### 3. **ShadowMapping project / ShadowMapping.exe**
- **Shadow Mapping:**
  - Implemented for directional (sun) lighting
