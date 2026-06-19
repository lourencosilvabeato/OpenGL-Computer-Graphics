# OpenGL Computer Graphics

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=c%2B%2B&logoColor=white)
![OpenGL](https://img.shields.io/badge/OpenGL-4.x-5586A4?logo=opengl&logoColor=white)
![GLSL](https://img.shields.io/badge/GLSL-Shaders-orange)
![CMake](https://img.shields.io/badge/CMake-Build-064F8C?logo=cmake&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)

A series of four interactive C++/OpenGL applications developed for the Computer Graphics 1 course at TU Dresden, covering core rendering and geometry topics: half-edge mesh processing, procedural texturing via GLSL fragment shaders, GPU terrain rendering with multi-texture blending, and spatial collision detection using AABB trees and hash grids.

## Screenshots

- **Mesh Processing (Exercises 1 & 2)** — interactive viewer for loading OBJ meshes and geometric primitives, with controls for Laplacian smoothing, surface area, volume, valence computation, and triangle strip extraction.

  ![Mesh Processing](screenshots/exercise1_2_mesh.png)

- **Procedural Julia Fractal (Exercise 3)** — real-time Julia set rendered on a 3D quad via a GLSL fragment shader, with interactive Julia C and zoom parameters controllable at runtime.

  ![Julia Fractal](screenshots/exercise3_fractal.png)

- **Terrain Rendering (Exercise 4)** — GPU-rendered terrain with multi-texture blending (grass, rock), normal-mapped road overlay, and a procedural sky, all driven from a height map.

  ![Terrain Rendering](screenshots/exercise4_terrain.png)

- **Collision Detection (Exercise 5)** — spatial indexing with AABB tree and uniform hash grid for efficient ray-mesh intersection and nearest-neighbour queries, with configurable ray and query parameters.

  ![Collision Detection](screenshots/exercise5_collision.png)

## Features

- **Half-Edge Mesh Operations** — load OBJ meshes and compute surface area, volume, and vertex valences using OpenMesh's half-edge data structure
- **Geometric Primitives** — procedural generation of quads, disks, spheres, cubes, cylinders, tori, icosahedra, tetrahedra, and arrows
- **Laplacian Mesh Smoothing** — uniform and cotangent-weight Laplacian smoothing with configurable iterations and strength
- **Triangle Stripification** — greedy triangle strip extraction for optimised GPU memory usage
- **Shell Extraction** — identification and isolation of connected mesh components
- **GLSL Procedural Texturing** — Julia set fractal rendered as a fragment shader with real-time parameter tweaking
- **Terrain Rendering** — height-map terrain with multi-texture blending, normal mapping, and sky dome rendering
- **Spatial Indexing** — AABB tree and hash grid structures for efficient nearest-neighbour and ray intersection queries
- **Interactive Viewer** — NanoGUI-based control panels across all exercises with mouse-orbit camera

## Tech Stack

| Layer | Technology | Purpose |
|-------|-----------|---------|
| Language | C++17 | Core algorithm implementations |
| Graphics API | OpenGL 4.x | GPU rendering pipeline |
| Shader Language | GLSL | Procedural texturing, terrain, sky |
| Mesh Library | OpenMesh | Half-edge mesh structure and I/O |
| Math | Eigen | Linear algebra and matrix operations |
| GUI | NanoGUI | Interactive control panels |
| Windowing | GLFW | Window creation and input handling |
| Build System | CMake | Cross-platform project configuration |

## Project Structure

```
computer-graphics/
├── common/              # Shared base classes: camera, shader pool, GL wrappers
├── data/                # Sample 3D models (.obj) — bunny, dragon, cow, horse, venus…
├── exercise1_2/         # Mesh processing: primitives, smoothing, area, volume
│   ├── include/         # Valence, Smoothing, Stripification, ShellExtraction headers
│   └── src/             # Viewer and algorithm implementations
├── exercise3/           # Julia fractal procedural texturing
│   ├── glsl/            # shader.frag / shader.vert
│   └── src/             # Viewer
├── exercise4/           # Terrain rendering
│   ├── glsl/            # terrain.frag/vert, sky.frag/vert
│   └── resources/       # Grass, rock, road textures + normal maps
├── exercise5/           # Collision detection and spatial indexing
│   ├── include/         # AABBTree, HashGrid, GridTraverser headers
│   └── src/             # Ray-triangle intersection implementations
└── ext/                 # External dependencies (NanoGUI, OpenMesh — git submodules)
```

## Getting Started

### Prerequisites

- **CMake** 3.x or newer
- **C++17 compiler** — MSVC 2019+ (Windows), GCC 8+ (Linux), or Clang 7+ (macOS)
- **OpenGL 4.x** capable GPU and up-to-date drivers

> **Windows:** install [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/) (free) and during setup select the **"Desktop development with C++"** workload — this includes MSVC, CMake, and all required build tools.

---

### 1. Clone

```bash
git clone --recursive https://github.com/lourencosilvabeato/OpenGL-Computer-Graphics.git
cd OpenGL-Computer-Graphics
```

The `--recursive` flag is required to pull in the NanoGUI and OpenMesh submodules.

---

### 2. Build

**Linux / macOS**
```bash
mkdir build && cd build
cmake ..
make
```

**Windows (Visual Studio)**
1. Open **CMake GUI**
2. Set *Where is the source code* → the cloned folder
3. Set *Where to build the binaries* → a new `build/` subfolder inside it
4. Click **Configure** → select your Visual Studio version → click **Generate** → **Open Project**
5. In Visual Studio, right-click the desired project in the Solution Explorer and select **Set as Startup Project**, then press **F5** to build and run

---

### 3. Run

All executables are built into `build/bin/`. **Always run them from the project root** so they can locate the `data/` folder (3D models) and shader files.

**Linux / macOS**
```bash
# From the project root:
./build/bin/Exercise1_2   # Mesh processing
./build/bin/Exercise3     # Julia fractal
./build/bin/Exercise4     # Terrain rendering
./build/bin/Exercise5     # Collision detection
```

**Windows**
```
# Open a terminal in the project root, then:
.\build\bin\Exercise1_2.exe
.\build\bin\Exercise3.exe
.\build\bin\Exercise4.exe
.\build\bin\Exercise5.exe
```

Once open, use the control panel on the left to interact with each exercise. For mesh exercises (1+2 and 5), click **Load Mesh** to open a `.obj` file from the `data/` folder, or **Create Primitive** to generate a shape directly.

---

### Linux dependencies (Debian/Ubuntu)

```bash
sudo apt install git make cmake libxxf86vm1 libxrandr2 libxinerama1 libxcursor1 \
  libx11-6 libc6 libstdc++6 libgcc-8-dev libxext6 libxrender1 libxfixes3 \
  libxcb1 libxau6 libxdmcp6 libbsd0
```
