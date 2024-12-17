# **StolenEngine: 3D Game Engine**

A modular, component-based **3D Game Engine** designed for flexibility, performance, and ease of use. It includes robust systems for managing game objects, rendering, physics, and more, providing a foundation for developing games and simulations.

---

## **Overview**

StolenEngine is built around a component-based architecture to allow modular management of game objects, assets, and systems. It supports key features like **real-time rendering**, **scene management**, and **transform hierarchies**.

---

## **Key Features**

- **Component-Based System**: Dynamically attach and manage components (e.g., RenderComponent, TransformComponent, PhysicsComponent) to GameObjects for flexible behavior.  
- **Real-Time Rendering**: Powered by **OpenGL**, supporting materials, shaders, textures, and geometries for visually rich environments.  
- **Scene Management**: Maintain a hierarchical structure of nodes, scenes, and game objects for seamless world-building.  
- **Physics and Collision**: Integrates a physics manager and collision detection system for realistic simulations.

---

## **Core Systems**

1. **Game Object System**  
   Manages GameObjects, their lifecycles, and their attached components.

2. **Rendering System**  
   Handles real-time rendering using OpenGL, including shader management, materials, and geometry processing.

3. **Transform System**  
   Manages position, rotation, and scaling of objects with support for hierarchical transformations.

4. **Scenegraph**  
   A structured scene graph that organizes and manages all nodes and entities in the world.

5. **Input and Event Handling**  
   Provides input abstraction for keyboard, mouse, and gamepad devices.

6. **Service Locator Pattern**  
   A centralized point for accessing core engine services like **Renderer**, **WindowHandler**, and **PhysicsManager**.

---

## **How to Use**

1. **Initialize the Engine**: Set up the engine and load your game logic.  
2. **Create Game Objects**: Add game objects and attach components dynamically.  
3. **Run the Game**: Utilize the built-in engine loop to render and update game states.

---

## **Planned Features**

- Advanced physics engine integration.  
- Lighting and post-processing effects.  
- Asset management with runtime resource loading.

---
