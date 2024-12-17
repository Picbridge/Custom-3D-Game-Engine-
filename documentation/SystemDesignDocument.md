# Game Engine System Design Document

## Table of Contents
1. [Introduction](#introduction)
2. [Directory Structure](#directory-structure)
3. [Core Engine Systems](#core-engine-systems)
    - [Component System](#component-system)
    - [Game Object System](#game-object-system)
    - [Rendering System](#rendering-system)
    - [Material and Shader System](#material-and-shader-system)
    - [Geometry System](#geometry-system)
    - [Texture Management](#texture-management)
    - [Transform System](#transform-system)
4. [Tools](#tools)
5. [Tech Prototypes](#tech-prototypes)
6. [Game Prototypes](#game-prototypes)
7. [Game Projects](#game-projects)
8. [Content Pipeline](#content-pipeline)
    - [Model Importing](#model-importing)
    - [Shader Compilation](#shader-compilation)
    - [Audio Pipeline](#audio-pipeline)
9. [Testing](#testing)
10. [Documentation](#documentation)
11. [External Libraries](#external-libraries)
12. [Future Improvements](#future-improvements)

---

## Introduction
This document outlines the design of the **StolenEngine** game engine, its architecture, key systems, and directory structure. The goal of the engine is to provide a flexible framework for building game prototypes, tech prototypes, and full games while maintaining modularity and scalability.

## Directory Structure
The directory structure follows a modular approach, organizing the source code, tools, prototypes, games, content, tests, documentation, and external libraries into distinct sections:

```
main/ 
├── source/ 
│   ├── include/ # Public headers for the engine and libraries 
│   ├── engine/ # Engine source code and project files 
│   ├── tools/ # Source code for various development tools 
│   ├── tech-prototypes/ # Tech prototypes (project files and source code) 
│   ├── game-prototypes/ # Game prototypes (project files and source code) 
│   ├── game/ # Game project (project files and source code) 
│   └── <<team-name>>.sln # Optional team solution file for the entire project 
│
├── content/ 
│   ├── art/ # Art assets (models, materials, textures) 
│   ├── audio/ # Audio assets (music, sound effects) 
│   └── code/ # Code assets (shaders, scripts) 
│
├── test/ # Testing code and project files 
├── documentation/ # Documentation (TDDs, GDDs, coding standards) 
└── external/ # External libraries (FMOD, ImGui, etc.)

```
## Core Engine Systems

### Component System
The **Component System** is designed to follow the Entity-Component pattern, where game objects are constructed by attaching various components. Each component encapsulates specific functionality, such as rendering, physics, or behavior.

- **File Locations**:
  - **Header**: `source/include/Component.h`
  - **Implementation**: `source/engine/Component.cpp`
- **Filter**:
    - `SourceFiles/Application/GameManagement/GameObject`
- **Responsibilities**:
  - Allows flexible composition of behaviors for game objects.
  - Interacts with owner and can get other components of the owner

### Game Object System
The **Game Object System** manages the lifecycle of entities in the game. Each entity consists of multiple components.

- **File Locations**:
  - **Header**: `source/include/GameObject.h`
  - **Implementation**: `source/engine/GameObject.cpp`
- **Responsibilities**:
  - Manage game object creation, updates, and destruction.
  - Allow interaction between game objects and their components.

### Rendering System
The **Rendering System** uses OpenGL to draw game objects on the screen. Game objects must have a `GeometryComponent` (defining the mesh) and a `MaterialComponent` (defining the surface appearance) to be rendered.

- **File Locations**:
  - **Header**: `source/engine/Renderer.h`
  - **Implementation**: `source/engine/Renderer.cpp`
- **Responsibilities**:
  - Initialize rendering context (OpenGL).
  - Render objects in the scene.
  - Handle lighting, shadows, and post-processing effects.

### Material and Shader System
The **Material and Shader System** manages the appearance of game objects through materials and shaders. Shaders are written in GLSL, and materials define how game objects are shaded.

- **File Locations**:
  - **Headers**: `source/engine/Material.h`, `source/engine/Shader.h`
  - **Implementations**: `source/engine/Material.cpp`, `source/engine/Shader.cpp`
- **Responsibilities**:
  - Load and compile GLSL shaders.
  - Define surface properties using materials.
  - Provide dynamic shader switching for different rendering effects.

### Geometry System
The **Geometry System** manages the vertex data of 3D models. It interfaces with the `ObjLoader` to load model data (vertices, normals, textures) and prepares it for rendering.

- **File Locations**:
  - **Header**: `source/engine/Geometry.h`
  - **Implementation**: `source/engine/Geometry.cpp`
- **Responsibilities**:
  - Load and store mesh data.
  - Interface with the rendering system to draw models.

### Texture Management
The **Texture Management System** handles loading, storing, and applying textures to game objects. The engine supports multiple image formats.

- **File Locations**:
  - **Header**: `source/engine/Texture.h`
  - **Implementation**: `source/engine/Texture.cpp`
- **Responsibilities**:
  - Load textures from image files (PNG, JPG).
  - Bind textures to shaders and materials.

### Transform System
The **Transform System** manages the position, rotation, and scale of game objects. It supports hierarchical transformations for parent-child relationships.

- **File Locations**:
  - **Header**: `source/engine/Transform.h`
  - **Implementation**: `source/engine/Transform.cpp`
- **Responsibilities**:
  - Handle local and global transformations for game objects.
  - Manage parent-child relationships between game objects.

---

## Tools
The **Tools Directory** contains tools used for game development such as asset importers, editors, or debugging utilities. Each tool has its own project file.

- **File Location**:
  - `source/tools/`
- **Example**:
  - **Level Editor**: Tool for designing game levels.
  - **Asset Converter**: Tool for converting models, textures, or audio formats.

---

## Tech Prototypes
The **Tech Prototypes Directory** houses experimental code for testing new features or systems before integrating them into the engine.

- **File Location**:
  - `source/tech-prototypes/`
- **Example**:
  - **Rendering Demo**: Prototype for testing advanced rendering techniques.

---

## Game Prototypes
The **Game Prototypes Directory** holds early-stage game ideas or experimental gameplay mechanics.

- **File Location**:
  - `source/game-prototypes/`
- **Example**:
  - **Prototype Project**: A simple game prototype to test mechanics like AI or input systems.

---

## Game Projects
The **Game Directory** contains fully-fledged game projects that are built using the engine.

- **File Location**:
  - `source/game/`
- **Example**:
  - **Main Game Project**: A complete game developed using the engine.

---

## Content Pipeline

### Model Importing
The engine uses the **ObjLoader** to import `.obj` files for models. This loader parses the model data and converts it into a format that the geometry system can use.

- **File Locations**:
  - **Header**: `source/engine/ObjLoader.h`
  - **Implementation**: `source/engine/ObjLoader.cpp`
- **Supported Formats**: OBJ
- **Responsibilities**:
  - Load model data into memory.
  - Convert model data to a renderable format.

### Shader Compilation
The **Shader System** supports loading and compiling GLSL shaders. Custom shaders can be applied to materials to modify the appearance of game objects.

- **File Locations**:
  - **Header**: `source/engine/ShaderLoader.h`
  - **Implementation**: `source/engine/ShaderLoader.cpp`
- **Responsibilities**:
  - Compile GLSL vertex and fragment shaders.
  - Bind shaders to materials for custom rendering effects.

### Audio Pipeline
The **Audio Pipeline** will support sound effects and background music. It can be expanded by integrating external libraries like **FMOD**.

- **File Locations**:
  - `content/audio/`
- **Responsibilities**:
  - Load sound files.
  - Play music and sound effects in-game.

---

## Testing
The **Testing Directory** contains unit tests and integration tests for the engine.

- **File Location**:
  - `test/`
- **Responsibilities**:
  - Ensure that game objects and components behave as expected.
  - Validate rendering, physics, and input systems.

---

## Documentation
The **Documentation Directory** stores various design documents, including Technical Design Documents (TDDs) and Game Design Documents (GDDs).

- **File Location**:
  - `documentation/`
- **Example Documents**:
  - `coding-standards.md`: Coding standards and guidelines for the project.

---

## External Libraries
The **External Directory** contains third-party libraries required by the engine.

- OpenGL libraries
  - glew
  - glfw
  - glm

## TODO
- Separate the shader loader from Shader system


[Top](#table-of-contents)