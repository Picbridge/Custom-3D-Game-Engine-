# Game Engine API Reference

## Table of Contents

### Singletons

  - [Engine](#engine)
  - [ServiceLocator](#servicelocator)
  - [Renderer](#renderer)
  - [WindowHandler](#windowhandler)

### Classes

  - [Shader](#shader)
  - [Texture](#texture)
  - [Material](#material)
  - [Geometry](#geometry)
  - [Transform](#transform)
  - [SceneGraph](#scenegraph)
  - [Node](#node)
  - [GameObject](#gameobject)
  - [Component](#component)
  - [RenderComponent](#rendercomponent)
  - [TransformComponent](#transformcomponent)
  - [Game](#game)
  - [Utils](#utils)
  - [Time](#time)
  - [VectorCalculations](#vectorcalculations)

---

## API

## Engine
<details>
  <summary>Engine</summary>

### Description
The `Engine` class is responsible for managing the entire game lifecycle. It runs the main game loop, handles initialization and shutdown of subsystems, and provides control over the game's execution.

### Private Members
- `bool m_isRunning`: A flag that determines whether the engine is currently running.
- `Game* m_pGame`: A pointer to the currently loaded game.
- `ServiceLocator* m_pServiceLocator`: A pointer to the service locator for accessing engine services.

### Public Methods

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">SetGame</span>(<span style="color:#4EC9B0;">Game*</span> game)
  </summary>

- **Description**: Sets the game object for the engine to manage.
- **Parameters**: 
  - `game`: A pointer to the game object. If no game is set, the engine will not be able to run.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Run</span>()
  </summary>

- **Description**: Starts the main loop of the engine. This function initializes all subsystems and enters the game loop where it continually updates and renders the game.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Init</span>()
  </summary>

- **Description**: Initializes the engine, including setting up services and any necessary systems (such as rendering or input).
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Shutdown</span>()
  </summary>

- **Description**: Shuts down the engine, releasing all resources and performing any necessary cleanup before exiting.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">bool</span> 
    <span style="color:#E2C636;">IsRunning</span>() 
  </summary>

- **Description**: Returns whether the engine is currently running.
- **Returns**: `true` if the engine is running, otherwise `false`.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Stop</span>() 
  </summary>

- **Description**: Stops the engine, signaling that the main loop should exit.
</details>

### Usage Example

```cpp
int main() 
{
    // Create an instance of the Engine
    Engine* engine = Engine::GetInstance();  // Engine is typically a singleton

    // Set the game for the engine to run
    Game* game = new MyGame(1280, 720, "My Awesome Game");
    engine->SetGame(game);

    // Initialize and run the engine
    engine->Init();
    engine->Run();

    // Shutdown the engine once the game exits
    engine->Shutdown();

    return 0;
}

```
</details>

---

## ServiceLocator
<details>
  <summary>ServiceLocator</summary>
  
### Description
The `ServiceLocator` class follows the Service Locator design pattern, providing a central access point to various engine services like [Renderer](#renderer) and [WindowHandler](windowhandler). This class allows different parts of the engine to retrieve services without requiring direct dependencies.

### Private Members
- `static ServiceLocator* instance`: A static pointer to the singleton instance of the `ServiceLocator` class.
- `struct Services`: A private inner struct that holds references to the engine services:
  - `WindowHandler* AWindow`: A pointer to the window handler service.
  - `Renderer* Renderer`: A pointer to the renderer service.
- `Services EngineServices`: A struct instance that contains service objects like the window handler and renderer.

### Public Methods

<details> 
  <summary> 
    <span style="color:#569CD6;">template</span>  <span style="color:#569CD6;">typename</span> <span style="color:#4EC9B0;">T</span>
    <span style="color:#569CD6;">static</span> <span style="color:#4EC9B0;">T</span>*
    <span style="color:#E2C636;">GetService</span>()
  </summary>

- **Description**: A static template function to retrieve a pointer to a specific engine service.
- **Returns**: A pointer to the requested service of type `T`.
- **Usage**: This method can retrieve services like `Renderer`, `WindowHandler`, etc.
- **Implementation Details**:
  - If the singleton instance is not created yet, it will initialize `instance` with a new `ServiceLocator` object.
  - It uses `std::is_same_v<T, ServiceType>` to check if the requested service type is available. If true, it returns the corresponding instance.
  - For example, if the requested service is `Renderer`, it returns the renderer instance.
  - If the requested type does not match any supported services, it triggers a `static_assert` to inform the developer that the service type is unavailable.
</details>

### Usage Example

```cpp
int main() 
{
    // Retrieve the Renderer service
    Renderer* renderer = ServiceLocator::GetService<Renderer>();

    // Retrieve the WindowHandler service
    WindowHandler* windowHandler = ServiceLocator::GetService<WindowHandler>();

    // Use the services in the engine
    windowHandler->CreateWindow();
    renderer->Init();

    return 0;
}
```

</details>

---

## Renderer
<details>
  <summary>Renderer</summary>

### Description
The `Renderer` class manages the core rendering functionality of the engine. It is responsible for rendering all game objects, handling shaders, materials, and geometries. The `Renderer` interacts with the scene graph to draw all visible objects in the game world.

### Private Members
- `static Renderer* instance`: A static singleton instance of the `Renderer` class.
- `SceneGraph* m_pSceneGraph`: A pointer to the scene graph, which holds all the nodes (game objects) to be rendered.
- `std::unordered_map<std::string, Shader*> m_shaders`: A map of loaded shaders, keyed by their names.
- `std::unordered_map<std::string, Material*> m_materials`: A map of loaded materials, keyed by their names.
- `std::unordered_map<std::string, Geometry*> m_geometries`: A map of loaded geometries, keyed by their names.

### Public Methods

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Init</span>()
  </summary>

- **Description**: Initializes the renderer. This sets up rendering configurations and prepares the necessary shaders, materials, and geometries.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Render</span>()
  </summary>

- **Description**: Renders all objects in the scene graph by iterating through nodes and drawing each one using its associated geometry and material.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">PostUpdate</span>()
  </summary>

- **Description**: Handles post-rendering operations, such as clearing the frame buffers or preparing for the next frame.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Shutdown</span>()
  </summary>

- **Description**: Shuts down the renderer, releasing all rendering resources such as shaders, materials, and geometries.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">AddShader</span>(<span style="color:#4EC9B0;">const std::string</span> name, <span style="color:#4EC9B0;">const char*</span> vertexPath, <span style="color:#4EC9B0;">const char*</span> fragmentPath, <span style="color:#4EC9B0;">const char*</span> geometryPath = nullptr)
  </summary>

- **Description**: Adds a new shader to the renderer. The shader is loaded from the specified vertex, fragment, and optional geometry shader files.
- **Parameters**:
  - `name`: The name of the shader to be used as a key in the shader map.
  - `vertexPath`: The path to the vertex shader file.
  - `fragmentPath`: The path to the fragment shader file.
  - `geometryPath`: The path to the optional geometry shader file.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">RemoveShader</span>(<span style="color:#4EC9B0;">const std::string</span> name)
  </summary>

- **Description**: Removes a shader from the renderer using the shader's name as a key.
- **Parameters**:
  - `name`: The name of the shader to be removed.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">AddMaterial</span>(<span style="color:#4EC9B0;">const std::string</span> name)
  </summary>

- **Description**: Adds a new material to the renderer. The material is stored in the material map, keyed by its name.
- **Parameters**:
  - `name`: The name of the material.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">RemoveMaterial</span>(<span style="color:#4EC9B0;">const std::string</span> name)
  </summary>

- **Description**: Removes a material from the renderer using the material's name as a key.
- **Parameters**:
  - `name`: The name of the material to be removed.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">AddGeometry</span>(<span style="color:#4EC9B0;">const std::string</span> name, <span style="color:#4EC9B0;">const char*</span> path)
  </summary>

- **Description**: Adds a new geometry to the renderer, loading it from the specified file path.
- **Parameters**:
  - `name`: The name of the geometry to be used as a key in the geometry map.
  - `path`: The path to the geometry file.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">RemoveGeometry</span>(<span style="color:#4EC9B0;">const std::string</span> name)
  </summary>

- **Description**: Removes a geometry from the renderer using its name as a key.
- **Parameters**:
  - `name`: The name of the geometry to be removed.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">SceneGraph*</span> 
    <span style="color:#E2C636;">GetSceneGraph</span>()
  </summary>

- **Description**: Returns the scene graph managed by the renderer.
- **Returns**: A pointer to the `SceneGraph` object.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">Shader*</span> 
    <span style="color:#E2C636;">GetShader</span>(<span style="color:#4EC9B0;">const std::string</span> name)
  </summary>

- **Description**: Returns a shader from the shader map using the shader's name as a key.
- **Returns**: A pointer to the shader.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">Material*</span> 
    <span style="color:#E2C636;">GetMaterial</span>(<span style="color:#4EC9B0;">const std::string</span> name)
  </summary>

- **Description**: Returns a material from the material map using the material's name as a key.
- **Returns**: A pointer to the material.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">Geometry*</span> 
    <span style="color:#E2C636;">GetGeometry</span>(<span style="color:#4EC9B0;">const std::string</span> name)
  </summary>

- **Description**: Returns a geometry from the geometry map using the geometry's name as a key.
- **Returns**: A pointer to the geometry.
</details>

### Usage Example

```cpp
int main() 
{
    // Initialize the Renderer
    Renderer* renderer = Renderer::GetInstance();
    renderer->Init();

    // Set up a shader, material, and geometry
    renderer->AddShader("basic", "vertex.glsl", "fragment.glsl");
    renderer->AddMaterial("default");
    renderer->AddGeometry("cube", "cube.obj");

    // Render loop
    while (true) {
        renderer->Render();
        renderer->PostUpdate();
    }

    // Shutdown the Renderer
    renderer->Shutdown();

    return 0;
}
```
</details>

---

## WindowHandler
<details>
  <summary>WindowHandler</summary>

### Description
The `WindowHandler` class manages the creation, updating, and destruction of the application window. It handles window-related events such as resizing, key input, and buffer swapping. This class also interacts with the system's graphical framework (GLFW) to display and manage the game window.

### Public Members
- `WINDOW_PROPS Props`: A structure that holds the window's properties.
- `FRAME_BUFFER_PROPS FrameBuffer`: A structure that holds the frame buffer properties.

### Constructor

<details>
  <summary>
    <span style="color:#E2C636;">WindowHandler</span>()
  </summary>

- **Description**: Constructor for the `WindowHandler` class, which initializes the internal state and prepares the window for setup.
</details>

### Public Methods

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Init</span>()
  </summary>

- **Description**: Initializes the window by setting up the required GLFW context and properties.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Update</span>()
  </summary>

- **Description**: Updates the window by handling input events and checking for system messages. Typically called once per frame to ensure the window remains responsive.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Shutdown</span>()
  </summary>

- **Description**: Shuts down the window, releasing all GLFW resources associated with it.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">bool</span> 
    <span style="color:#E2C636;">ShouldClose</span>()
  </summary>

- **Description**: Checks whether the window should close, based on input or system events.
- **Returns**: `true` if the window should close, otherwise `false`.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">SwapBuffers</span>()
  </summary>

- **Description**: Swaps the buffers, displaying the rendered content on the screen. This function is called after rendering each frame.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">GLFWwindow*</span> 
    <span style="color:#E2C636;">GetCurrentContext</span>()
  </summary>

- **Description**: Returns the current OpenGL or rendering context associated with the window.
- **Returns**: Pointer to the `GLFWwindow` that represents the current window context.
</details>

### Static Methods

<details>
  <summary>
    <span style="color:#569CD6;">static void</span> 
    <span style="color:#E2C636;">GLFWErrorCallback</span>(<span style="color:#4EC9B0;">int</span> error, <span style="color:#4EC9B0;">const char*</span> description)
  </summary>

- **Description**: Handles GLFW errors by providing an error code and description of the error.
- **Parameters**: 
  - `error`: Error code.
  - `description`: Description of the error.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">static void</span> 
    <span style="color:#E2C636;">GLFWWindowSizeCallback</span>(<span style="color:#4EC9B0;">GLFWwindow*</span> window, <span style="color:#4EC9B0;">int</span> width, <span style="color:#4EC9B0;">int</span> height)
  </summary>

- **Description**: Callback function that is triggered when the window is resized.
- **Parameters**: 
  - `window`: Pointer to the `GLFWwindow` that was resized.
  - `width`: New width of the window.
  - `height`: New height of the window.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">static void</span> 
    <span style="color:#E2C636;">GLFWWindowKeyCallback</span>(<span style="color:#4EC9B0;">GLFWwindow*</span> window, <span style="color:#4EC9B0;">int</span> key, <span style="color:#4EC9B0;">int</span> scancode, <span style="color:#4EC9B0;">int</span> action, <span style="color:#4EC9B0;">int</span> mods)
  </summary>

- **Description**: Callback function triggered when a key is pressed or released within the window.
- **Parameters**: 
  - `window`: Pointer to the `GLFWwindow` receiving the input.
  - `key`: The key that was pressed or released.
  - `scancode`: Platform-specific scan code of the key.
  - `action`: Action related to the key press (press, release, or repeat).
  - `mods`: Bit field describing modifier keys (Shift, Ctrl, etc.).
</details>

<details>
  <summary>
    <span style="color:#569CD6;">static void</span> 
    <span style="color:#E2C636;">MessageCallback</span>(<span style="color:#4EC9B0;">GLenum</span> source, <span style="color:#4EC9B0;">GLenum</span> type, <span style="color:#4EC9B0;">GLuint</span> id, <span style="color:#4EC9B0;">GLenum</span> severity, <span style="color:#4EC9B0;">GLsizei</span> length, <span style="color:#4EC9B0;">const GLchar*</span> message, <span style="color:#4EC9B0;">const void*</span> userParam)
  </summary>

- **Description**: Callback function for handling OpenGL debug messages.
- **Parameters**: 
  - `source`: Source of the debug message.
  - `type`: Type of the debug message.
  - `id`: ID of the message.
  - `severity`: Severity level of the message.
  - `length`: Length of the message string.
  - `message`: The actual debug message.
  - `userParam`: User-defined parameter passed to the callback.
</details>

### Usage Example

```cpp
int main() 
{
    // Create an instance of the WindowHandler
    WindowHandler* windowHandler = new WindowHandler();

    // Initialize the window
    windowHandler->Init();

    // Run the window update in the main loop
    while (!windowHandler->ShouldClose()) {
        windowHandler->Update();
        windowHandler->SwapBuffers();
    }

    // Clean up and shut down the window
    windowHandler->Shutdown();
    delete windowHandler;

    return 0;
}

```
</details>

---

## Shader
<details>
  <summary>Shader</summary>

### Description
The `Shader` class is responsible for loading, compiling, and managing shaders used in the rendering process. It also provides utilities to manage uniforms and attributes in the shader programs. Shaders control the appearance of objects rendered by the engine.

### Private Members
- `unsigned int m_id`: The unique ID of the shader program.
- `std::unordered_map<std::string, GLint> m_uniformCache`: A cache of uniform locations to avoid querying the same uniform location multiple times.

### Constructor

<details>
  <summary>
    <span style="color:#569CD6;">Shader</span>(<span style="color:#4EC9B0;">const char*</span> vertexPath, <span style="color:#4EC9B0;">const char*</span> fragmentPath, <span style="color:#4EC9B0;">const char*</span> geometryPath = <span style="color:#569CD6;">nullptr</span>)
  </summary>

- **Description**: Loads and compiles the vertex, fragment, and optionally the geometry shader from the specified file paths.
- **Parameters**: 
  - `vertexPath`: Path to the vertex shader file.
  - `fragmentPath`: Path to the fragment shader file.
  - `geometryPath`: (Optional) Path to the geometry shader file.
</details>

### Public Methods

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Use</span>()
  </summary>

- **Description**: Activates the shader program for use in rendering.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Unuse</span>()
  </summary>

- **Description**: Deactivates the shader program.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">GLint</span> 
    <span style="color:#E2C636;">GetAttributeLocation</span>(<span style="color:#4EC9B0;">const std::string&</span> name)
  </summary>

- **Description**: Returns the location of the specified attribute in the shader program.
- **Parameters**: 
  - `name`: The name of the attribute.
- **Returns**: Location of the attribute as `GLint`.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">GLint</span> 
    <span style="color:#E2C636;">GetUniformLocation</span>(<span style="color:#4EC9B0;">const std::string&</span> name)
  </summary>

- **Description**: Returns the location of the specified uniform in the shader program. If the uniform has been queried before, it retrieves the location from the cache.
- **Parameters**: 
  - `name`: The name of the uniform.
- **Returns**: Location of the uniform as `GLint`.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">ClearUniformCache</span>()
  </summary>

- **Description**: Clears the uniform cache to ensure all locations are re-queried when needed.
</details>

### Usage Example

```cpp
int main() 
{
    // Create and load a shader
    Shader* shader = new Shader("vertex.glsl", "fragment.glsl");

    // Use the shader for rendering
    shader->Use();

    // Set a uniform in the shader
    GLint location = shader->GetUniformLocation("myUniform");

    // Unuse the shader after rendering
    shader->Unuse();

    // Clean up
    delete shader;
    return 0;
}
```

</details>

---

## Texture

<details> 
  <summary>Texture</summary>

### Description
The `Texture` class handles loading image files and managing texture data in the engine. It provides functionality for loading textures, assigning texture data, and retrieving texture dimensions.

### Private Members
- `int m_size`: Size of the texture.
- `unsigned int m_width`: Width of the texture in pixels.
- `unsigned int m_height`: Height of the texture in pixels.
- `unsigned int m_bytesPerPix`: Number of bytes per pixel.
- `unsigned int m_bitDepth`: Bit depth of the texture.
- `glm::vec2* m_pTexcoord`: Pointer to texture coordinates.
- `glm::vec3* m_pImage`: Pointer to the image buffer.


### Constructor

<details>
  <summary>
    <span style="color:#E2C636;">Texture</span>(<span style="color:#4EC9B0;">const char*</span> filename)
  </summary>

- **Description**: Constructor that loads the texture from the specified file.
- **Parameters**:
  - `filename`: The path to the texture file.
</details>

### Public Methods

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">AssignTextureToDest</span>(<span style="color:#4EC9B0;">glm::vec3*&</span> dest)
  </summary>

- **Description**: Assigns the image buffer to the specified destination.
- **Parameters**:
  - `dest`: The destination where the image buffer will be stored.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">LoadTexture</span>(<span style="color:#4EC9B0;">const char*</span> filename)
  </summary>

- **Description**: Loads the texture from the specified file.
- **Parameters**:
  - `filename`: The path to the texture file.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">unsigned int</span>
    <span style="color:#E2C636;">GetWidth</span>()
  </summary>

- **Description**: Returns the width of the texture.
- **Returns**: The width of the texture in pixels.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">unsigned int</span>
    <span style="color:#E2C636;">GetHeight</span>()
  </summary>

- **Description**: Returns the height of the texture.
- **Returns**: The height of the texture in pixels.
</details>

### Private Methods

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">loadPNGFile</span>(<span style="color:#4EC9B0;">const char*</span> filename)
  </summary>

- **Description**: Reads the PNG file and extracts the image data.
- **Parameters**:
  - `filename`: The path to the PNG file.
</details>

### Usage Example

```cpp
int main()
{
    // Create a Texture object and load a texture file
    Texture* texture = new Texture("assets/texture.png");

    // Load the texture into a destination buffer
    glm::vec3* imageBuffer = nullptr;
    texture->AssignTextureToDest(imageBuffer);

    // Retrieve texture dimensions
    unsigned int width = texture->GetWidth();
    unsigned int height = texture->GetHeight();

    // Print texture information
    std::cout << "Loaded texture with width: " << width << " and height: " << height << std::endl;

    // Clean up
    delete texture;
    return 0;
}
```
</details>

---

## Material
<details>
  <summary>Material</summary>

### Description
The `Material` class is responsible for managing materials in the engine. It handles assigning textures (diffuse and specular), colors, and shininess, and it passes material data to shaders during rendering.

### Private Members
- `MaterialData m_data`: A struct containing the material's properties such as color, diffuse texture, specular texture, and shininess.
- `Shader* m_pShader`: A pointer to the shader associated with the material.
- `Texture* m_pDiffuse`: A pointer to the diffuse texture (to be removed after Texture Manager integration).
- `Texture* m_pSpecular`: A pointer to the specular texture (to be removed after Texture Manager integration).

### Constructor

<details>
  <summary>
    <span style="color:#569CD6;">Material</span>()
  </summary>

- **Description**: Default constructor that initializes the material with default values.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">Material</span>(<span style="color:#4EC9B0;">Shader*</span> pShader)
  </summary>

- **Description**: Constructor that initializes the material with the provided shader.
- **Parameters**:
  - `pShader`: A pointer to the shader to be used with this material.
</details>

### Public Methods

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">SetShader</span>(<span style="color:#4EC9B0;">Shader*</span> shader)
  </summary>

- **Description**: Replaces the current shader with the user-defined shader.
- **Parameters**:
  - `shader`: A pointer to the new shader.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">SetShader</span>(<span style="color:#4EC9B0;">const std::string</span> name)
  </summary>

- **Description**: Replaces the current shader with a shader from the shader map by name.
- **Parameters**:
  - `name`: The name of the shader to be used.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">SetTextureDiffuse</span>(<span style="color:#4EC9B0;">const char*</span> path)
  </summary>

- **Description**: Applies a diffuse texture to the current material.
- **Parameters**:
  - `path`: The path to the diffuse texture.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">SetTextureSpecular</span>(<span style="color:#4EC9B0;">const char*</span> path)
  </summary>

- **Description**: Applies a specular texture to the current material.
- **Parameters**:
  - `path`: The path to the specular texture.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">SetColor</span>(<span style="color:#4EC9B0;">glm::vec3</span> color)
  </summary>

- **Description**: Sets the color of the current material.
- **Parameters**:
  - `color`: A `glm::vec3` containing RGB values in the range [0, 255].
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">SetColor</span>(<span style="color:#4EC9B0;">float</span> r, <span style="color:#4EC9B0;">float</span> g, <span style="color:#4EC9B0;">float</span> b)
  </summary>

- **Description**: Sets the color of the current material using individual RGB values.
- **Parameters**:
  - `r`: Red component in the range [0, 255].
  - `g`: Green component in the range [0, 255].
  - `b`: Blue component in the range [0, 255].
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">SetShininess</span>(<span style="color:#4EC9B0;">float</span> shininess)
  </summary>

- **Description**: Sets the shininess value for the material.
- **Parameters**:
  - `shininess`: A `float` value representing the material's shininess.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">SetupUniformData</span>()
  </summary>

- **Description**: Passes the material data (color, textures, shininess) to the shader as uniform values.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">Bind</span>()
  </summary>

- **Description**: Binds the diffuse and specular textures to be used in the rendering pipeline.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">Unbind</span>()
  </summary>

- **Description**: Unbinds the current textures, preparing for the next object render.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">Shader*</span>
    <span style="color:#E2C636;">GetShader</span>()
  </summary>

- **Description**: Returns the shader associated with the current material.
- **Returns**: A pointer to the shader.
</details>

### Usage Example

```cpp
int main()
{
    // Create a shader and a material
    Shader* shader = new Shader("vertex.glsl", "fragment.glsl");
    Material* material = new Material(shader);

    // Set material properties
    material->SetColor(255.0f, 0.0f, 0.0f);  // Set material color to red
    material->SetShininess(32.0f);  // Set material shininess

    // Apply textures
    material->SetTextureDiffuse("assets/diffuse.png");
    material->SetTextureSpecular("assets/specular.png");

    // Bind material for rendering
    material->Bind();

    // Set up material data in shader uniforms
    material->SetupUniformData();

    // After rendering
    material->Unbind();

    // Clean up
    delete material;
    delete shader;

    return 0;
}

```
</details>

---

## Geometry
<details>
  <summary>Geometry</summary>

### Description
The `Geometry` class handles loading, binding, and rendering of 3D model data. It manages vertex, normal, and UV data for rendering objects in the engine.

### Private Members
- `UV_TYPE m_uvType`: The type of UV mapping applied to the geometry (e.g., planar, cylindrical, spherical, cube).
- `GLuint m_VAO`: The Vertex Array Object (VAO) for storing vertex attribute configurations.
- `GLuint m_VBO`: The Vertex Buffer Object (VBO) for storing vertex data.
- `GLuint m_IBO`: The Index Buffer Object (IBO) for storing index data.
- `GLuint m_UV`: The UV buffer for texture coordinates.
- `GLuint m_NORMALBUFFER`: The buffer storing normals for the geometry.
- `VERTEX_DATA m_vertexData`: Struct containing the vertex data.
- `NORMAL_DATA m_normalData`: Struct containing the normal data.
- `UV_INFO m_uvInfo`: Struct containing the UV mapping information.

### Constructor

<details>
  <summary>
    <span style="color:#569CD6;">Geometry</span>()
  </summary>

- **Description**: Default constructor that initializes the geometry.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">Geometry</span>(<span style="color:#4EC9B0;">const char*</span> path)
  </summary>

- **Description**: Constructor that loads the geometry from the specified OBJ file.
- **Parameters**:
  - `path`: The path to the OBJ file containing the geometry data.
</details>

### Public Methods

<details>
  <summary>
    <span style="color:#569CD6;">bool</span>
    <span style="color:#E2C636;">LoadGeometry</span>(<span style="color:#4EC9B0;">const char*</span> path)
  </summary>

- **Description**: Loads the geometry data from the specified OBJ file.
- **Parameters**:
  - `path`: The path to the OBJ file.
- **Returns**: `true` if the geometry is successfully loaded, `false` otherwise.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">Bind</span>(<span style="color:#4EC9B0;">Material*</span> material)
  </summary>

- **Description**: Binds the vertex data and buffers to the current material's shader attributes.
- **Parameters**:
  - `material`: A pointer to the `Material` to fetch the shader attribute locations.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">Unbind</span>()
  </summary>

- **Description**: Unbinds the vertex data from the buffer, preparing the geometry for the next object render.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">CleanUpBuffers</span>()
  </summary>

- **Description**: Cleans up the buffers and releases the resources used by the geometry.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">Render</span>()
  </summary>

- **Description**: Renders the geometry using the currently bound shader and textures.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">SetUVType</span>(<span style="color:#4EC9B0;">UV_TYPE</span> type)
  </summary>

- **Description**: Sets the UV mapping type for the geometry.
- **Parameters**:
  - `type`: The UV mapping type (e.g., planar, cylindrical, spherical, cube).
</details>

<details>
  <summary>
    <span style="color:#569CD6;">int</span>
    <span style="color:#E2C636;">GetUVType</span>()
  </summary>

- **Description**: Returns the UV mapping type as an integer.
- **Returns**: The UV type (0 for planar, 1 for cylindrical, 2 for spherical, 3 for cube).
</details>

### Usage Example

```cpp
int main()
{
    // Create a Geometry object
    Geometry* geometry = new Geometry("assets/models/cube.obj");

    // Load geometry data from the OBJ file
    if (geometry->LoadGeometry("assets/models/cube.obj")) {
        std::cout << "Geometry loaded successfully!" << std::endl;
    } else {
        std::cerr << "Failed to load geometry!" << std::endl;
    }

    // Create a material and bind it to the geometry
    Material* material = new Material();
    geometry->Bind(material);

    // Render the geometry
    geometry->Render();

    // Clean up buffers after rendering
    geometry->Unbind();
    geometry->CleanUpBuffers();

    // Clean up
    delete geometry;
    delete material;

    return 0;
}
```
</details>

---

## Transform
<details>
  <summary>Transform</summary>

### Description
The `Transform` class manages the position, rotation, and scale of objects in the game world. It provides matrix transformations (model, view, projection) for rendering.

### Private Members
- `glm::mat4 m_model`: The model matrix for the transform.
- `glm::mat4 m_projection`: The projection matrix.
- `glm::mat4 m_view`: The view matrix.
- `glm::mat4 m_translationMatrix`: The translation matrix.
- `glm::mat4 m_rotationMatrix`: The rotation matrix.
- `glm::mat4 m_scaleMatrix`: The scale matrix.
- `glm::vec3 m_position`: The position of the object.
- `glm::vec3 m_rotation`: The rotation of the object.
- `glm::vec3 m_scale`: The scale of the object.

### Public Methods

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">SetPosition</span>(<span style="color:#4EC9B0;">glm::vec3</span> position)
  </summary>

- **Description**: Sets the position of the transform.
- **Parameters**:
  - `position`: A `glm::vec3` specifying the position of the transform.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">SetRotation</span>(<span style="color:#4EC9B0;">glm::vec3</span> rotation)
  </summary>

- **Description**: Sets the rotation of the transform.
- **Parameters**:
  - `rotation`: A `glm::vec3` specifying the rotation angles in degrees.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">SetScale</span>(<span style="color:#4EC9B0;">glm::vec3</span> scale)
  </summary>

- **Description**: Sets the scale of the transform.
- **Parameters**:
  - `scale`: A `glm::vec3` specifying the scale factors.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">SetProjection</span>(<span style="color:#4EC9B0;">glm::mat4</span> projection)
  </summary>

- **Description**: Assigns the projection matrix to the transform.
- **Parameters**:
  - `projection`: The projection matrix.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">SetView</span>(<span style="color:#4EC9B0;">glm::mat4</span> view)
  </summary>

- **Description**: Assigns the view matrix to the transform.
- **Parameters**:
  - `view`: The view matrix.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">glm::vec3</span>
    <span style="color:#E2C636;">GetPosition</span>()
  </summary>

- **Description**: Returns the position of the transform.
- **Returns**: A `glm::vec3` representing the position of the object.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">glm::vec3</span>
    <span style="color:#E2C636;">GetRotation</span>()
  </summary>

- **Description**: Returns the rotation of the transform.
- **Returns**: A `glm::vec3` representing the rotation angles.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">glm::vec3</span>
    <span style="color:#E2C636;">GetScale</span>()
  </summary>

- **Description**: Returns the scale of the transform.
- **Returns**: A `glm::vec3` representing the scale factors.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">glm::mat4</span>
    <span style="color:#E2C636;">GetTranslationMatrix</span>()
  </summary>

- **Description**: Returns the translation matrix.
- **Returns**: A `glm::mat4` representing the translation matrix.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">glm::mat4</span>
    <span style="color:#E2C636;">GetRotationMatrix</span>()
  </summary>

- **Description**: Returns the rotation matrix.
- **Returns**: A `glm::mat4` representing the rotation matrix.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">glm::mat4</span>
    <span style="color:#E2C636;">GetScaleMatrix</span>()
  </summary>

- **Description**: Returns the scale matrix.
- **Returns**: A `glm::mat4` representing the scale matrix.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">glm::mat4</span>
    <span style="color:#E2C636;">GetModel</span>()
  </summary>

- **Description**: Returns the model matrix of the transform.
- **Returns**: A `glm::mat4` representing the model matrix.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">glm::mat4</span>
    <span style="color:#E2C636;">GetProjection</span>()
  </summary>

- **Description**: Returns the projection matrix of the transform.
- **Returns**: A `glm::mat4` representing the projection matrix.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">glm::mat4</span>
    <span style="color:#E2C636;">GetView</span>()
  </summary>

- **Description**: Returns the view matrix of the transform.
- **Returns**: A `glm::mat4` representing the view matrix.
</details>

### Usage Example

```cpp
int main()
{
    // Create a Transform object
    Transform* transform = new Transform();

    // Set position, rotation, and scale
    transform->SetPosition(glm::vec3(1.0f, 2.0f, 3.0f));
    transform->SetRotation(glm::vec3(45.0f, 0.0f, 90.0f));
    transform->SetScale(glm::vec3(1.0f, 2.0f, 1.0f));

    // Get the model matrix for rendering
    glm::mat4 modelMatrix = transform->GetModel();

    // Print out the transform position
    glm::vec3 position = transform->GetPosition();
    std::cout << "Position: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;

    // Clean up
    delete transform;

    return 0;
}

```
</details>

---

## SceneGraph
<details>
  <summary>SceneGraph</summary>

### Description
The `SceneGraph` class manages a collection of nodes in the game scene. It handles adding, removing, and deleting nodes, providing the structure for organizing game objects in a hierarchical manner.

### Implementation Details

The `SceneGraph` in the [Renderer](#renderer) class manages the organization and traversal of nodes (game objects) in the scene. During each frame, the `Renderer` invokes the `SceneGraph` to update all nodes, ensuring that their transformations (position, rotation, scale) are propagated correctly through parent-child relationships. The `SceneGraph` also handles node additions, removals, and deletions, automatically updating their status in the rendering pipeline.

Nodes are rendered in a hierarchical order, where the Renderer fetches the geometry and material data from each node and issues the necessary draw calls. The SceneGraph simplifies scene management by automatically updating nodes every frame, allowing the engine to maintain real-time object transformations and rendering with minimal overhead.

### Private Members
- `int m_nodeCount`: The total number of nodes in the scene graph.
- `std::vector<Node*> m_nodes`: A list of pointers to nodes present in the scene.

### Constructor

<details>
  <summary>
    <span style="color:#E2C636;">SceneGraph</span>()
  </summary>

- **Description**: Default constructor that initializes the scene graph.
</details>

### Public Methods

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">AddNode</span>(<span style="color:#4EC9B0;">Node*</span> node)
  </summary>

- **Description**: Adds a node to the current scene.
- **Parameters**:
  - `node`: A pointer to the `Node` to be added to the scene graph.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">DeleteNode</span>(<span style="color:#4EC9B0;">Node*</span> node)
  </summary>

- **Description**: Deletes a node from the scene.
- **Parameters**:
  - `node`: A pointer to the `Node` to be deleted.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">Node*</span>
    <span style="color:#E2C636;">RemoveNode</span>(<span style="color:#4EC9B0;">Node*</span> node)
  </summary>

- **Description**: Removes a node from the scene without deleting it.
- **Parameters**:
  - `node`: A pointer to the `Node` to be removed from the scene graph.
- **Returns**: A pointer to the removed `Node`.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">std::vector<Node*></span>
    <span style="color:#E2C636;">GetNodes</span>()
  </summary>

- **Description**: Returns the list of nodes in the scene.
- **Returns**: A `std::vector` containing pointers to the nodes in the scene graph.
</details>

### Usage Example

```cpp
int main()
{
    // Create a SceneGraph object
    SceneGraph* sceneGraph = new SceneGraph();

    // Create some nodes
    Node* node1 = new Node();
    Node* node2 = new Node();

    // Add nodes to the scene graph
    sceneGraph->AddNode(node1);
    sceneGraph->AddNode(node2);

    // Remove a node without deleting it
    Node* removedNode = sceneGraph->RemoveNode(node1);

    // Delete a node from the scene graph
    sceneGraph->DeleteNode(node2);

    // Get the list of nodes
    std::vector<Node*> nodes = sceneGraph->GetNodes();
    std::cout << "Number of nodes in the scene: " << nodes.size() << std::endl;

    // Clean up
    delete node1;
    delete node2;
    delete sceneGraph;

    return 0;
}
```
</details>

---

## Node
<details>
  <summary>Node</summary>

### Description
The `Node` class represents a hierarchical object in the scene graph, capable of containing components and child nodes. It manages transformations, components, and the relationship between parent and child nodes.

### Private Members
- `size_t m_id`: The unique ID for the node.
- `Transform* m_pTransform`: The transform component for the node.
- `Node* m_pParent`: A pointer to the parent node.
- `glm::mat4 m_worldTransform`: The world transformation matrix for the node.
- `std::unordered_map<std::string, Component*> m_components`: A map of components attached to this node.
- `std::vector<Node*> m_children`: A list of child nodes.
- `bool m_needsDeletion`: A flag indicating whether the node needs to be deleted.

### Constructor

<details>
  <summary>
    <span style="color:#E2C636;">Node</span>()
  </summary>

- **Description**: Default constructor that initializes the node.
</details>

### Public Methods

<details>
  <summary>
    <span style="color:#569CD6;">virtual</span>
    <span style="color:#E2C636;">~Node</span>()
  </summary>

- **Description**: Virtual destructor that cleans up the node's resources.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">virtual void</span>
    <span style="color:#E2C636;">AddChild</span>(<span style="color:#4EC9B0;">Node*</span> node)
  </summary>

- **Description**: Adds a child node to the current node.
- **Parameters**:
  - `node`: A pointer to the child `Node` to be added.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">virtual void</span>
    <span style="color:#E2C636;">RemoveChild</span>(<span style="color:#4EC9B0;">Node*</span> node)
  </summary>

- **Description**: Removes a child node from the current node.
- **Parameters**:
  - `node`: A pointer to the child `Node` to be removed.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">virtual void</span>
    <span style="color:#E2C636;">SetID</span>(<span style="color:#4EC9B0;">size_t</span> id)
  </summary>

- **Description**: Sets the ID for the current node.
- **Parameters**:
  - `id`: A unique ID for the node.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">Destroy</span>()
  </summary>

- **Description**: Destroys the current node, marking it for deletion.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span>
    <span style="color:#E2C636;">Flush</span>()
  </summary>

- **Description**: Flushes the node from memory if it is marked for deletion.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">template</span> <typename DataType>
    <span style="color:#569CD6;">DataType*</span> 
    <span style="color:#E2C636;">AddComponent</span>()
  </summary>

- **Description**: Adds a component of type `DataType` to the current node.
- **Returns**: A pointer to the newly added component.
- **Usage**: This method is only valid for types derived from the `Component` class.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">template</span> <typename DataType>
    <span style="color:#569CD6;">void*</span> 
    <span style="color:#E2C636;">RemoveComponent</span>()
  </summary>

- **Description**: Removes a component of type `DataType` from the current node.
- **Usage**: This method is only valid for types derived from the `Component` class.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">template</span> <typename DataType>
    <span style="color:#569CD6;">DataType*</span> 
    <span style="color:#E2C636;">GetComponent</span>()
  </summary>

- **Description**: Retrieves a component of type `DataType` from the current node.
- **Returns**: A pointer to the component if found, otherwise `nullptr`.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">size_t</span> 
    <span style="color:#E2C636;">GetID</span>()
  </summary>

- **Description**: Returns the unique ID of the node.
- **Returns**: A `size_t` representing the node's ID.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">Transform*</span> 
    <span style="color:#E2C636;">GetTransform</span>()
  </summary>

- **Description**: Returns the transform component of the node.
- **Returns**: A pointer to the node's `Transform`.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">glm::mat4</span> 
    <span style="color:#E2C636;">GetWorldTransform</span>()
  </summary>

- **Description**: Returns the world transformation matrix for the node.
- **Returns**: A `glm::mat4` representing the node's world transformation matrix.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">Node*</span> 
    <span style="color:#E2C636;">GetParent</span>()
  </summary>

- **Description**: Returns the parent node of the current node.
- **Returns**: A pointer to the parent `Node`.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">std::vector<Node*></span> 
    <span style="color:#E2C636;">GetChildren</span>()
  </summary>

- **Description**: Returns the list of child nodes.
- **Returns**: A `std::vector` containing pointers to the node's children.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">bool</span> 
    <span style="color:#E2C636;">NeedsDeletion</span>()
  </summary>

- **Description**: Returns whether the node is marked for deletion.
- **Returns**: `true` if the node is marked for deletion, otherwise `false`.
</details>

### Usage Example

```cpp
int main()
{
    // Create a root node and a child node
    Node* rootNode = new Node();
    Node* childNode = new Node();

    // Add the child node to the root node
    rootNode->AddChild(childNode);

    // Set and retrieve transform component
    Transform* transform = rootNode->GetTransform();
    transform->SetPosition(glm::vec3(1.0f, 2.0f, 3.0f));

    // Add a component to the root node
    rootNode->AddComponent<RenderComponent>();

    // Remove a child node from the root node
    rootNode->RemoveChild(childNode);

    // Mark node for deletion
    rootNode->Destroy();

    // Clean up
    delete rootNode;
    delete childNode;

    return 0;
}
```
</details>

---

## GameObject
<details>
  <summary> GameObject</summary>

### Description
The `GameObject` class represents a game entity that extends the functionality of a `Node`. It includes properties like name and life state, and provides methods for initializing, updating, rendering, and managing the object's lifecycle.

### Private Members
- `bool m_isAlive`: A flag indicating whether the game object is alive or marked for destruction.
- `std::string m_name`: The name of the game object.

### Constructor

<details>
  <summary>
    <span style="color:#E2C636;">GameObject</span>()
  </summary>

- **Description**: Default constructor that initializes the game object with default values (`m_isAlive` is `true` and `m_name` is an empty string).
</details>

### Public Methods

<details>
  <summary>
    <span style="color:#569CD6;">virtual</span> 
    <span style="color:#E2C636;">~GameObject</span>()
  </summary>

- **Description**: Destructor that cleans up the game object's resources.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">SetName</span>(<span style="color:#4EC9B0;">std::string</span> name)
  </summary>

- **Description**: Sets the name of the game object.
- **Parameters**:
  - `name`: A string representing the name of the game object.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Init</span>()
  </summary>

- **Description**: Initializes all components attached to the game object.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Update</span>()
  </summary>

- **Description**: Updates the game object (e.g., applies transformations, handles logic).
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Render</span>()
  </summary>

- **Description**: Renders the game object, including its components like geometry or material.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">SetDead</span>(<span style="color:#4EC9B0;">bool</span> isDead)
  </summary>

- **Description**: Marks the game object as dead or alive.
- **Parameters**:
  - `isDead`: A boolean indicating whether the game object should be considered "dead."
</details>

<details>
  <summary>
    <span style="color:#569CD6;">std::string</span> 
    <span style="color:#E2C636;">GetName</span>()
  </summary>

- **Description**: Returns the name of the game object.
- **Returns**: A `std::string` representing the object's name.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">bool</span> 
    <span style="color:#E2C636;">IsDead</span>()
  </summary>

- **Description**: Returns whether the game object is dead.
- **Returns**: `true` if the object is marked as dead, otherwise `false`.
</details>

### Usage Example

```cpp
int main()
{
    // Create a game object
    GameObject* player = new GameObject();

    // Set name for the game object
    player->SetName("Player");

    // Initialize the object and its components
    player->Init();

    player->AddComponent<RenderComponent>();
    player->GetComponent<RenderComponent>()->SetShader("Default");
    player->GetComponent<RenderComponent>()->SetMaterial("Default");
    player->GetComponent<RenderComponent>()->SetGeometry("Default");

    // In the game loop, update and render the object
    while (!player->IsDead()) {
        player->Update();
        player->Render();
    }

    player->RemoveComponent<RenderComponent>()
    // Mark the object as dead
    player->SetDead(true);

    // Clean up
    delete player;

    return 0;
}
```
</details>

---

## Component
<details>
  <summary>Component</summary>

### Description
The `Component` class serves as the base class for all game components, such as rendering, physics, and AI. Components are attached to `GameObject` instances (or `Node` instances) and contribute behavior and functionality. Components rely on their owner (the node or game object) to update, render, and manage their lifecycle. 

- [Node](#node): Each `Component` is associated with a `Node` (or `GameObject`) that acts as its owner. The owner is responsible for managing the lifecycle of the component (initializing, updating, and shutting down). Components rely on their owner to provide access to other components within the same node.
- [GameObject](#gameobject): A `GameObject` can have multiple components attached, each adding distinct functionality (such as rendering, transformation, or physics). The `Component` class provides methods to interact with other components in the same `GameObject` or `Node`.

### Private Members
- `Node* pOwner`: A pointer to the owner `Node` or `GameObject` to which the component is attached.

### Constructor

<details>
  <summary>
    <span style="color:#E2C636;">Component</span>()
  </summary>

- **Description**: Default constructor that initializes the component without an owner.
</details>

### Public Methods

<details>
  <summary>
    <span style="color:#569CD6;">virtual</span> 
    <span style="color:#E2C636;">~Component</span>()
  </summary>

- **Description**: Virtual destructor that allows for cleanup of component resources when destroyed.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">virtual void</span> 
    <span style="color:#E2C636;">Init</span>() = 0
  </summary>

- **Description**: Pure virtual method that initializes the component. Must be implemented by derived classes.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">virtual void</span> 
    <span style="color:#E2C636;">Update</span>() = 0
  </summary>

- **Description**: Pure virtual method that updates the component. Called every frame.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">virtual void</span> 
    <span style="color:#E2C636;">Shutdown</span>() = 0
  </summary>

- **Description**: Pure virtual method that shuts down the component, allowing for cleanup before the component is destroyed.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">SetOwner</span>(<span style="color:#4EC9B0;">Node*</span> owner)
  </summary>

- **Description**: Sets the owner of the component, typically a `Node` or `GameObject`.
- **Parameters**:
  - `owner`: A pointer to the `Node` that owns the component.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">Node*</span> 
    <span style="color:#E2C636;">GetOwner</span>()
  </summary>

- **Description**: Returns the owner of the component.
- **Returns**: A pointer to the `Node` that owns the component.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">template</span> <typename DataType>
    <span style="color:#569CD6;">DataType*</span> 
    <span style="color:#E2C636;">GetComponent</span>()
  </summary>

- **Description**: Retrieves a component of type `DataType` from the owner `Node` or `GameObject`.
- **Returns**: A pointer to the component if found, otherwise `nullptr`.
- **Usage**: Useful for accessing other components attached to the same `Node` or `GameObject`.
</details>

### Usage Example

```cpp
class RenderComponent : public Component {
public:
    void Init() override {
        // Initialize rendering logic
    }

    void Update() override {
        // Update rendering logic
    }

    void Shutdown() override {
        // Clean up rendering resources
    }
};

int main()
{
    // Create a GameObject
    GameObject* player = new GameObject();
    
    // Add a render component to the player GameObject
    RenderComponent* render = player->AddComponent<RenderComponent>();

    // Initialize the player and its components
    player->Init();

    // In the game loop, update the player and its components
    while (!player->IsDead()) {
        player->Update();
    }

    // Shutdown the player and its components
    player->SetDead(true);
    render->Shutdown();

    // Clean up
    delete player;

    return 0;
}
```
</details>

---

## RenderComponent
<details>
  <summary>RenderComponent</summary>

### Description
The `RenderComponent` class handles rendering functionality for a game object. It allows setting materials, shaders, geometry, and UV mapping for the game object. The `RenderComponent` interacts with the `Material`, `Shader`, and `Geometry` classes to render the game object with the appropriate settings.

### Private Members
- `Geometry* m_pGeometry`: A pointer to the geometry associated with the game object.
- `Material* m_pMaterial`: A pointer to the material used for rendering the game object.

### Constructor

<details>
  <summary>
    <span style="color:#E2C636;">RenderComponent</span>()
  </summary>

- **Description**: Default constructor that initializes the render component.
</details>

### Public Methods

<details>
  <summary>
    <span style="color:#E2C636;">~RenderComponent</span>()
  </summary>

- **Description**: Destructor that cleans up the render component's resources.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Init</span>()
  </summary>

- **Description**: Initializes the render component.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Update</span>()
  </summary>

- **Description**: Updates the render component, typically called every frame.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Shutdown</span>()
  </summary>

- **Description**: Shuts down the render component, releasing any associated resources.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Render</span>()
  </summary>

- **Description**: Renders the game object using the associated geometry, material, and shader.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">SetColor</span>(<span style="color:#4EC9B0;">glm::vec3</span> color)
  </summary>

- **Description**: Sets the color of the material.
- **Parameters**:
  - `color`: A `glm::vec3` representing the color (RGB) in the range [0, 1].
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">SetColor</span>(<span style="color:#4EC9B0;">float</span> r, <span style="color:#4EC9B0;">float</span> g, <span style="color:#4EC9B0;">float</span> b)
  </summary>

- **Description**: Sets the color of the material using individual RGB values.
- **Parameters**:
  - `r`: Red component in the range [0, 1].
  - `g`: Green component in the range [0, 1].
  - `b`: Blue component in the range [0, 1].
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">SetMaterial</span>(<span style="color:#4EC9B0;">Material*</span> pMaterial)
  </summary>

- **Description**: Sets the material used for rendering the game object.
- **Parameters**:
  - `pMaterial`: A pointer to the `Material` to be used.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">SetMaterial</span>(<span style="color:#4EC9B0;">const std::string</span> name)
  </summary>

- **Description**: Sets the material by its name from the material map.
- **Parameters**:
  - `name`: The name of the material.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">SetShader</span>(<span style="color:#4EC9B0;">Shader*</span> pShader)
  </summary>

- **Description**: Replaces the current shader with a user-defined shader.
- **Parameters**:
  - `pShader`: A pointer to the new shader.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">SetShader</span>(<span style="color:#4EC9B0;">const std::string</span> name)
  </summary>

- **Description**: Sets the shader by its name from the shader map.
- **Parameters**:
  - `name`: The name of the shader.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">SetGeometry</span>(<span style="color:#4EC9B0;">Geometry*</span> pGeometry)
  </summary>

- **Description**: Sets the geometry for the game object.
- **Parameters**:
  - `pGeometry`: A pointer to the `Geometry` to be used.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">SetGeometry</span>(<span style="color:#4EC9B0;">const std::string</span> geometryName)
  </summary>

- **Description**: Sets the geometry by its class name.
- **Parameters**:
  - `geometryName`: The name of the geometry class.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">SetUVType</span>(<span style="color:#4EC9B0;">UV_TYPE</span> type)
  </summary>

- **Description**: Sets the UV mapping type for the geometry.
- **Parameters**:
  - `type`: The UV mapping type (e.g., planar, cylindrical, spherical, cube).
</details>

<details>
  <summary>
    <span style="color:#569CD6;">Geometry*</span> 
    <span style="color:#E2C636;">GetGeometry</span>()
  </summary>

- **Description**: Returns the geometry of the current game object.
- **Returns**: A pointer to the `Geometry`.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">Shader*</span> 
    <span style="color:#E2C636;">GetShader</span>()
  </summary>

- **Description**: Returns the shader used by the material of the current game object.
- **Returns**: A pointer to the `Shader`.
</details>

### Usage Example

```cpp
int main()
{
    // Create a game object
    GameObject* object = new GameObject();

    // Add a render component to the game object
    RenderComponent* renderComponent = object->AddComponent<RenderComponent>();

    // Set geometry and material for the render component
    Geometry* geometry = new Geometry("cube.obj");
    renderComponent->SetGeometry(geometry);

    Material* material = new Material();
    renderComponent->SetMaterial(material);

    // In the game loop, update and render the object
    object->Init();
    while (!object->IsDead()) {
        object->Update();
        renderComponent->Render();
    }

    // Shutdown and clean up
    renderComponent->Shutdown();
    delete object;
    delete geometry;
    delete material;

    return 0;
}
```
</details>

---

## TransformComponent

<details>
  <summary>TransformComponent</summary>

### Description
The `TransformComponent` class is responsible for managing the position, rotation, and scale of a game object. It provides an interface to manipulate these transformations and retrieve the necessary matrices for rendering. The component is built on top of a `Transform` object, which handles the mathematical operations.

### Private Members
- `Transform* m_pTransform`: A pointer to the `Transform` object that manages the position, rotation, and scale of the game object.

### Constructor

<details>
  <summary>
    <span style="color:#E2C636;">TransformComponent</span>()
  </summary>

- **Description**: Default constructor that initializes the transform component.
</details>

### Public Methods

<details>
  <summary>
    <span style="color:#569CD6;">virtual</span> 
    <span style="color:#E2C636;">~TransformComponent</span>()
  </summary>

- **Description**: Destructor that cleans up the transform component's resources.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Init</span>()
  </summary>

- **Description**: Initializes the transform component.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Update</span>()
  </summary>

- **Description**: Updates the transform component, typically called every frame.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">Shutdown</span>()
  </summary>

- **Description**: Shuts down the transform component, releasing any associated resources.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">SetPosition</span>(<span style="color:#4EC9B0;">glm::vec3</span> pos)
  </summary>

- **Description**: Sets the position of the game object.
- **Parameters**:
  - `pos`: A `glm::vec3` representing the new position of the game object.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">SetRotation</span>(<span style="color:#4EC9B0;">glm::vec3</span> angle)
  </summary>

- **Description**: Sets the rotation of the game object.
- **Parameters**:
  - `angle`: A `glm::vec3` representing the new rotation angles (in degrees) of the game object.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">SetScale</span>(<span style="color:#4EC9B0;">glm::vec3</span> scale)
  </summary>

- **Description**: Sets the scale of the game object.
- **Parameters**:
  - `scale`: A `glm::vec3` representing the new scale of the game object.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">SetScale</span>(<span style="color:#4EC9B0;">float</span> scale)
  </summary>

- **Description**: Sets a uniform scale for the game object.
- **Parameters**:
  - `scale`: A float representing the new uniform scale for the game object.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">glm::vec3</span> 
    <span style="color:#E2C636;">GetPosition</span>()
  </summary>

- **Description**: Returns the position of the game object.
- **Returns**: A `glm::vec3` representing the position of the game object.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">glm::vec3</span> 
    <span style="color:#E2C636;">GetRotation</span>()
  </summary>

- **Description**: Returns the rotation of the game object.
- **Returns**: A `glm::vec3` representing the rotation angles of the game object.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">glm::vec3</span> 
    <span style="color:#E2C636;">GetScale</span>()
  </summary>

- **Description**: Returns the scale of the game object.
- **Returns**: A `glm::vec3` representing the scale of the game object.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">glm::mat4</span> 
    <span style="color:#E2C636;">GetTranslationMatrix</span>()
  </summary>

- **Description**: Returns the translation matrix of the game object.
- **Returns**: A `glm::mat4` representing the translation matrix of the game object.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">glm::mat4</span> 
    <span style="color:#E2C636;">GetRotationMatrix</span>()
  </summary>

- **Description**: Returns the rotation matrix of the game object.
- **Returns**: A `glm::mat4` representing the rotation matrix of the game object.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">glm::mat4</span> 
    <span style="color:#E2C636;">GetScaleMatrix</span>()
  </summary>

- **Description**: Returns the scale matrix of the game object.
- **Returns**: A `glm::mat4` representing the scale matrix of the game object.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">glm::mat4</span> 
    <span style="color:#E2C636;">GetModelMatrix</span>()
  </summary>

- **Description**: Returns the model matrix of the game object.
- **Returns**: A `glm::mat4` representing the model matrix of the game object.
</details>

### Usage Example

```cpp
int main()
{
    // Create a game object
    GameObject* object = new GameObject();

    // Add a transform component to the game object
    TransformComponent* transformComponent = object->AddComponent<TransformComponent>();

    // Set position, rotation, and scale
    transformComponent->SetPosition(glm::vec3(0.0f, 1.0f, 2.0f));
    transformComponent->SetRotation(glm::vec3(45.0f, 0.0f, 90.0f));
    transformComponent->SetScale(glm::vec3(1.0f, 2.0f, 1.0f));

    // Get the model matrix for rendering
    glm::mat4 modelMatrix = transformComponent->GetModelMatrix();

    // Clean up
    delete object;

    return 0;
}

```
</details>

---

## Game

<details>
  <summary>Game</summary>

### Description
The `Game` class serves as a base class for managing the core game logic, such as initialization, updating, rendering, and shutting down the game. It includes basic properties like the game window's dimensions and title, and a flag to determine if the game is running.

### Private Members
- `bool m_isRunning`: A flag indicating whether the game is currently running.
- `WINDOW_PROPS m_windowProps`: A structure containing the window properties (width, height, title).

### Constructor

<details>
  <summary>
    <span style="color:#E2C636;">Game</span>()
  </summary>

- **Description**: Default constructor that initializes the game with default settings and sets the game to a running state.
</details>


<details>
  <summary>
    <span style="color:#E2C636;">Game</span>(<span style="color:#4EC9B0;">WINDOW_PROPS</span> props)
  </summary>

- **Description**: Constructor that initializes the game with specific window properties.
- **Parameters**:
  - `props`: A `WINDOW_PROPS` structure containing window width, height, and title.
</details>

<details>
  <summary>
    <span style="color:#E2C636;">Game</span>(<span style="color:#4EC9B0;">int</span> width, <span style="color:#4EC9B0;">int</span> height, <span style="color:#4EC9B0;">std::string</span> title)
  </summary>

- **Description**: Constructor that initializes the game with specific width, height, and window title.
- **Parameters**:
  - `width`: The width of the window.
  - `height`: The height of the window.
  - `title`: The title of the window.
</details>

### Public Methods

<details>
  <summary>
    <span style="color:#569CD6;">virtual</span> 
    <span style="color:#E2C636;">~Game</span>()
  </summary>

- **Description**: Destructor that cleans up the game resources.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">virtual void</span> 
    <span style="color:#E2C636;">Init</span>() = 0
  </summary>

- **Description**: Pure virtual function to initialize the game. It must be implemented by any derived class.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">virtual void</span> 
    <span style="color:#E2C636;">Update</span>() = 0
  </summary>

- **Description**: Pure virtual function to update the game state. It must be implemented by any derived class.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">virtual void</span> 
    <span style="color:#E2C636;">Render</span>() = 0
  </summary>

- **Description**: Pure virtual function to render the game. It must be implemented by any derived class.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">virtual void</span> 
    <span style="color:#E2C636;">Shutdown</span>() = 0
  </summary>

- **Description**: Pure virtual function to shut down the game. It must be implemented by any derived class.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">void</span> 
    <span style="color:#E2C636;">SetRunning</span>(<span style="color:#4EC9B0;">bool</span> running)
  </summary>

- **Description**: Sets whether the game is currently running.
- **Parameters**:
  - `running`: A boolean indicating whether the game should be running.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">bool</span> 
    <span style="color:#E2C636;">IsRunning</span>()
  </summary>

- **Description**: Returns whether the game is currently running.
- **Returns**: `true` if the game is running, `false` otherwise.
</details>

<details>
  <summary>
    <span style="color:#569CD6;">WINDOW_PROPS</span> 
    <span style="color:#E2C636;">GetWindowProps</span>()
  </summary>

- **Description**: Returns the window properties of the game (e.g., width, height, title).
- **Returns**: A `WINDOW_PROPS` structure containing the window properties.
</details>

### Usage Example

```cpp
class MyGame : public Game {
public:
    void Init() override {
        // Initialize game resources
    }

    void Update() override {
        // Update game state
    }

    void Render() override {
        // Render game objects
    }

    void Shutdown() override {
        // Clean up resources
    }
};

int main()
{
    // Create a new game object
    Game* game = new MyGame(1280, 720, "My Game");

    // Initialize the game
    game->Init();

    // Game loop
    while (game->IsRunning()) {
        game->Update();
        game->Render();
    }

    // Shutdown the game
    game->Shutdown();

    // Clean up
    delete game;

    return 0;
}

```
</details>

---

## Utils
<details>
  <summary>Utils</summary>

### Description
The `Utils` class provides utility functions that assist with tasks such as extracting class names from data types and checking for OpenGL errors. These functions are intended to support various operations within the engine, particularly for debugging and logging.

### Public Methods

<details>
  <summary>
    <span style="color:#569CD6;">template</span> <typename DataType>
    <span style="color:#569CD6;">static std::string</span> 
    <span style="color:#E2C636;">GetClassName</span>()
  </summary>

- **Description**: Extracts the class name from the data type `DataType`.
- **Returns**: A `std::string` containing the class name.
- **Implementation Details**:
  - The method uses `typeid(DataType).name()` to retrieve the type name.
  - If the type name includes the prefix `"class "`, this method removes the prefix and returns the cleaned class name.
- **Example**:
  ```cpp
  std::string className = Utils::GetClassName<MyClass>();
  std::cout << "Class Name: " << className << std::endl;
  ```
  </details> 
  <details> 
    <summary> 
      <span style="color:#569CD6;">static void</span> 
      <span style="color:#E2C636;">GetGLError</span>() 
    </summary>

- **Description**: Checks for OpenGL errors and logs the error message.
- **Returns**: A `std::string` containing the class name.
- **Implementation Details**:
  - This method calls `glGetError()` to check for any OpenGL errors.
  - If an error is found, it prints an appropriate error message based on the error code (e.g., `GL_INVALID_ENUM`, `GL_OUT_OF_MEMORY`).
  - The method outputs the error message to the console.
- **Example**:
  ```cpp
  Utils::GetGLError();  // Checks for and logs OpenGL errors
  ```
  </details> 
### Usage Example

```cpp
int main()
{
    // Example of using GetClassName utility
    std::string className = Utils::GetClassName<MyGameClass>();
    std::cout << "Class Name: " << className << std::endl;

    // Example of using GetGLError utility
    Utils::GetGLError();  // Check for OpenGL errors

    return 0;
}

```
</details>

---

## Time
<details>
  <summary>Time</summary>

### Description
The `Time` class manages and updates the passage of time within the application, specifically keeping track of **delta time** (the time between the current and previous frame) and **fixed delta time** (a constant timestep used for physics updates). This class can be accessed as a singleton and is designed to ensure consistent timing, which is important for frame-rate-independent movement and simulation updates.

### Public Methods

<details>
  <summary><code>void Init(float fixedDT)</code></summary>

- **Description**: Initializes the `Time` class with a specified **fixed delta time**.
- **Parameters**:
  - `fixedDT`: A `float` representing the fixed delta time for physics or other consistent updates.
- **Implementation Details**:
  - Sets the initial value for `m_fixedDeltaTime`.
  - Initializes `m_lastTime` to the current time to start tracking frame delta times.

</details>

<details>
  <summary><code>void Update()</code></summary>

- **Description**: Updates the **delta time** (time elapsed since the last frame).
- **Implementation Details**:
  - Calculates the time difference between the current frame and the previous frame using the high-resolution clock.
  - Stores the result in `m_deltaTime`.

</details>

<details>
  <summary><code>inline const float GetFixedDeltaTime() const</code></summary>

- **Description**: Returns the **fixed delta time**, which is a predefined constant time step used for consistent updates such as physics simulations.
- **Returns**: A `float` representing the fixed delta time.
- **Implementation Details**:
  - This method is marked `inline` for performance, allowing the compiler to potentially optimize it by expanding the function at the call site.

</details>

<details>
  <summary><code>inline const float GetDeltaTime() const</code></summary>

- **Description**: Returns the **delta time**, which is the time elapsed between the current frame and the previous frame. This is typically used for frame-rate-independent operations like animations and movement.
- **Returns**: A `float` representing the delta time.
- **Implementation Details**:
  - This method is marked `inline` for performance, allowing the compiler to potentially optimize it by expanding the function at the call site.

</details>

### Private Methods

<details>
  <summary><code>static Time* GetInstance()</code></summary>

- **Description**: Provides access to the singleton instance of the `Time` class.
- **Implementation Details**:
  - Implements lazy initialization by creating an instance of `Time` only when it is first accessed.

</details>

### Usage Example

```cpp
int main() {
    // Example usage of Time in combination with ServiceLocator

    // Get the delta time using ServiceLocator and Time singleton
    float dt = ServiceLocator::GetService<Time>()->GetDeltaTime();

    // Rotate an object using delta time
    glm::vec3 rotation = objTransform->GetRotation();
    objTransform->SetRotation(rotation + glm::vec3(0.0f, 0.0f, 5.0f * dt));

    return 0;
}
```
</details>

---

## VectorCalculation
<details>
  <summary>VectorCalculation</summary>

### Description
The `VectorCalculation` class provides utility functions to calculate directional vectors (Forward, Right, and Up) based on an object's rotation in 3D space. It uses rotation matrices to transform base vectors to the correct orientation given a specific set of Euler angles (rotation). These functions are particularly useful for handling camera or object orientation in 3D environments like games or simulations.

### Public Methods

<details>
  <summary><code>static glm::vec3 GetForwardVec(glm::vec3 rotation)</code></summary>

- **Description**: Calculates the **Forward vector** based on the given rotation (Euler angles in degrees).
- **Parameters**:
  - `rotation`: A `glm::vec3` representing the object's rotation in the order (Pitch, Yaw, Roll).
- **Returns**: A normalized `glm::vec3` representing the Forward vector of the object in world space.
- **Implementation Details**:
  - The Forward vector is calculated using trigonometric functions on the Euler angles.
  - The calculation assumes that the object's forward direction is initially along the positive Z-axis and is transformed based on the given rotation.

</details>

<details>
  <summary><code>static glm::vec3 GetRightVec(glm::vec3 rotation)</code></summary>

- **Description**: Calculates the **Right vector** based on the given rotation (Euler angles in degrees).
- **Parameters**:
  - `rotation`: A `glm::vec3` representing the object's rotation in the order (Pitch, Yaw, Roll).
- **Returns**: A normalized `glm::vec3` representing the Right vector of the object in world space.
- **Implementation Details**:
  - The Right vector is calculated by transforming the base right vector (1, 0, 0) using the calculated rotation matrix.

</details>

<details>
  <summary><code>static glm::vec3 GetUpVec(glm::vec3 rotation)</code></summary>

- **Description**: Calculates the **Up vector** based on the given rotation (Euler angles in degrees).
- **Parameters**:
  - `rotation`: A `glm::vec3` representing the object's rotation in the order (Pitch, Yaw, Roll).
- **Returns**: A normalized `glm::vec3` representing the Up vector of the object in world space.
- **Implementation Details**:
  - The Up vector is calculated by transforming the base up vector (0, 1, 0) using the calculated rotation matrix.

</details>

### Private Methods

<details>
  <summary><code>static glm::mat3 calculateRotationMatrix(glm::vec3 rotation)</code></summary>

- **Description**: Generates the **rotation matrix** from the given Euler angles (rotation vector).
- **Parameters**:
  - `rotation`: A `glm::vec3` representing the rotation in degrees (Pitch, Yaw, Roll).
- **Returns**: A `glm::mat3` representing the rotation matrix for the given rotation.
- **Implementation Details**:
  - The method generates individual rotation matrices for the X, Y, and Z axes and multiplies them to form the final rotation matrix.

</details>

### Usage Example

```cpp
int main() {
    auto objTransform = m_gameObjects[0]->GetComponent<TransformComponent>();
    auto rotation = objTransform->GetRotation();
    // Get the forward, right, and up vectors based on the rotation
    glm::vec3 forward = VectorCalculation::GetForwardVec(rotation);
    glm::vec3 right = VectorCalculation::GetRightVec(rotation);
    glm::vec3 up = VectorCalculation::GetUpVec(rotation);

    std::cout << "Forward: " << forward.x << ", " << forward.y << ", " << forward.z << std::endl;
    std::cout << "Right: " << right.x << ", " << right.y << ", " << right.z << std::endl;
    std::cout << "Up: " << up.x << ", " << up.y << ", " << up.z << std::endl;

    return 0;
}
```
</details>

---