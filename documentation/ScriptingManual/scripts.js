// Content data for each section
const sections = {
    engine: `
    <div class="class">
    <h2>Engine</h2>
    <p>The <code>Engine</code> class is responsible for managing the game lifecycle, including initializing, updating, rendering, and shutting down the engine. It supports managing multiple games.</p>
    
    <h3>Public Methods</h3>
    <div class="method">
        <details>
        <summary>
            <span class="return-type">static Engine*</span>
            <span class="method-name">GetInstance</span>()
        </summary>
        <p><strong>Description:</strong> Returns the singleton instance of the <code>Engine</code> class.</p>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">~Engine</span>()
        </summary>
        <p><strong>Description:</strong> Destructor for the <code>Engine</code> class.</p>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span>
            <span class="method-name">SetGame</span>(<span class="param-type">Game*</span> game)
        </summary>
        <p><strong>Description:</strong> Sets the game to be managed by the engine.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>game</code>: A pointer to the game to set.</li>
        </ul>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span>
            <span class="method-name">PushGame</span>(<span class="param-type">Game*</span> game)
        </summary>
        <p><strong>Description:</strong> Pushes a new game onto the vector of managed games.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>game</code>: A pointer to the game to add.</li>
        </ul>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">Game*</span>
            <span class="method-name">FindGame</span>(<span class="param-type">std::string</span> name)
        </summary>
        <p><strong>Description:</strong> Finds a game by its name in the list of managed games.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>name</code>: The name of the game to find.</li>
        </ul>
        <p><strong>Returns:</strong> A pointer to the game if found, otherwise <code>nullptr</code>.</p>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span>
            <span class="method-name">Run</span>()
        </summary>
        <p><strong>Description:</strong> Runs the main engine loop, updating and rendering the current game.</p>
        </details>
    </div>

    <h3>Private Methods</h3>
    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span>
            <span class="method-name">init</span>()
        </summary>
        <p><strong>Description:</strong> Initializes the engine and its components.</p>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span>
            <span class="method-name">update</span>()
        </summary>
        <p><strong>Description:</strong> Updates the engine state during each frame.</p>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span>
            <span class="method-name">render</span>()
        </summary>
        <p><strong>Description:</strong> Renders the content for the current frame.</p>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span>
            <span class="method-name">postUpdate</span>()
        </summary>
        <p><strong>Description:</strong> Handles post-update logic after the frame is rendered.</p>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span>
            <span class="method-name">shutdown</span>()
        </summary>
        <p><strong>Description:</strong> Shuts down the engine, releasing all resources.</p>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span>
            <span class="method-name">initGames</span>()
        </summary>
        <p><strong>Description:</strong> Initializes all games managed by the engine.</p>
        </details>
    </div>

    <h3>Private Members</h3>
    <ul>
        <li><code>unsigned int m_prevGameIndex</code>: Index of the previously active game.</li>
        <li><code>static Engine* instance</code>: Singleton instance of the <code>Engine</code> class.</li>
        <li><code>Game* m_pGame</code>: Pointer to the currently active game.</li>
        <li><code>std::vector&lt;Game*&gt; m_games</code>: Vector containing pointers to all managed games.</li>
    </ul>
    </div>
    <pre><code class="language-cpp">
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
    </code></pre>
  `,
    serviceLocator: `
    <div class="class">
    <h2>ServiceLocator</h2>
    <p>The <code>ServiceLocator</code> class provides a centralized point to access various engine services, such as the renderer, window handler, input manager, and others.</p>

    <h3>Public Methods</h3>
    <div class="method">
        <details>
        <summary>
            <span class="return-type">template &lt;typename T&gt;</span>
            <span class="method-name">GetService&lt;/span>()
        </summary>
        <p><strong>Description:</strong> Returns the requested service of type <code>T</code>.</p>
        <p><strong>Template Parameter:</strong></p>
        <ul>
            <li><code>T</code>: The type of service to retrieve.</li>
        </ul>
        <p><strong>Returns:</strong> A pointer to the requested service or <code>nullptr</code> if the service type is unavailable.</p>
        </details>
    </div>

    <h3>Services Provided</h3>
    <ul>
        <li><code>Renderer</code>: Handles rendering operations.</li>
        <li><code>WindowHandler</code>: Manages the application window.</li>
        <li><code>Input</code>: Manages input devices like keyboard and mouse.</li>
        <li><code>UI</code>: Manages the user interface.</li>
        <li><code>Time</code>: Provides time-related utilities.</li>
        <li><code>PhysicsManager</code>: Manages physics simulations.</li>
        <li><code>CollisionManager</code>: Handles collision detection and resolution.</li>
        <li><code>EventHandler</code>: Manages game events.</li>
        <li><code>ScriptManager</code>: Manages scripting operations.</li>
        <li><code>SceneManager</code>: Manages scenes within the game.</li>
        <li><code>ResourceFactory</code>: Creates resources.</li>
        <li><code>ResourceManager</code>: Manages resources.</li>
        <li><code>GameObjectFactory</code>: Creates game objects.</li>
        <li><code>GameObjectManager</code>: Manages game objects.</li>
    </ul>

    <h3>Private Members</h3>
    <ul>
        <li><code>static ServiceLocator* instance</code>: Singleton instance of the <code>ServiceLocator</code> class.</li>
        <li><code>Services EngineServices</code>: Contains all engine services as a struct.</li>
    </ul>

    <h3>Example Usage</h3>
    <pre><code class="language-cpp">
        void Engine::init()
        {
            GLFWwindow* context = nullptr;

            // Initialize services
            ServiceLocator::GetService&lt;Time&gt;()-&gt;Init(1.0f / 60.0f);
            ServiceLocator::GetService&lt;WindowHandler&gt;()-&gt;Init();
            ServiceLocator::GetService&lt;Renderer&gt;()-&gt;Init();
            
            // Get the current context
            context = ServiceLocator::GetService&lt;WindowHandler&gt;()-&gt;GetCurrentContext();
            if (!context)
            {
                std::cerr &lt;&lt; "Error: Failed to initialize GLFW context!" &lt;&lt; std::endl;
                return;
            }
            
            // Initialize input system
            ServiceLocator::GetService&lt;Input&gt;()-&gt;Init(context);

            // Initialize and configure UI
            UI&amp; ui = ServiceLocator::GetService&lt;UI&gt;()-&gt;GetInstance();
            ui.Init(context);
            ui.SetDebug(false);

            // Initialize game-specific logic
            initGames();

            std::cout &lt;&lt; "Engine Initialized" &lt;&lt; std::endl;
        }
    </code></pre>
    </div>
  `,
    renderer: `
    <div class="class">
    <h2>Renderer</h2>
    <p>The <code>Renderer</code> class is responsible for managing the core rendering functionality of the game engine. It initializes rendering settings, renders scenes, and cleans up resources during shutdown.</p>

    <h3>Public Methods</h3>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span>
            <span class="method-name">Init</span>()
        </summary>
        <p><strong>Description:</strong> Initializes the renderer. Sets up necessary OpenGL settings such as enabling depth testing.</p>
        <pre><code class="language-cpp">
            Renderer* renderer = Renderer::GetInstance();
            renderer->Init();
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span>
            <span class="method-name">Render</span>()
        </summary>
        <p><strong>Description:</strong> Clears the screen and depth buffer, preparing for the next frame's rendering.</p>
        <pre><code class="language-cpp">
            Renderer* renderer = Renderer::GetInstance();
            renderer->Render();
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span>
            <span class="method-name">Shutdown</span>()
        </summary>
        <p><strong>Description:</strong> Shuts down the renderer and disables depth testing.</p>
        <pre><code class="language-cpp">
            Renderer* renderer = Renderer::GetInstance();
            renderer->Shutdown();
        </code></pre>
        </details>
    </div>

    <h3>Private Methods</h3>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">static Renderer*</span>
            <span class="method-name">GetInstance</span>()
        </summary>
        <p><strong>Description:</strong> Returns the singleton instance of the <code>Renderer</code> class. Creates the instance if it doesn't exist.</p>
        <pre><code class="language-cpp">
            Renderer* renderer = Renderer::GetInstance();
        </code></pre>
        </details>
    </div>

    <h3>Example Usage</h3>
    <pre><code class="language-cpp">
        #include "Renderer.h"

        int main() {
            // Get the Renderer instance
            Renderer* renderer = Renderer::GetInstance();

            // Initialize the renderer
            renderer->Init();

            // Render loop
            while (gameIsRunning) {
                renderer->Render();
            }

            // Shutdown the renderer
            renderer->Shutdown();

            return 0;
        }
    </code></pre>
    </div>
  `,
    windowHandler: `
    <div class="class">
    <h2>WindowHandler</h2>
    <p>The <code>WindowHandler</code> class manages the window lifecycle, including initialization, updates, and shutdown. It also handles OpenGL context setup and GLFW integration.</p>

    <h3>Public Methods</h3>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">Init</span>()
        </summary>
        <p><strong>Description:</strong> Initializes the window and OpenGL context, setting up necessary properties and GLFW.</p>
        <pre><code class="language-cpp">
            WindowHandler& window = WindowHandler::GetInstance();
            window.Init();
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">Update</span>()
        </summary>
        <p><strong>Description:</strong> Updates the window, processes events, and manages the viewport size.</p>
        <pre><code class="language-cpp">
            WindowHandler& window = WindowHandler::GetInstance();
            window.Update();
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">Shutdown</span>()
        </summary>
        <p><strong>Description:</strong> Shuts down the window and cleans up GLFW resources.</p>
        <pre><code class="language-cpp">
            WindowHandler& window = WindowHandler::GetInstance();
            window.Shutdown();
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">bool</span> 
            <span class="method-name">ShouldClose</span>()
        </summary>
        <p><strong>Description:</strong> Checks whether the window should close.</p>
        <pre><code class="language-cpp">
            WindowHandler& window = WindowHandler::GetInstance();
            if (window.ShouldClose()) {
            // Handle window close logic
            }
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">SwapBuffers</span>()
        </summary>
        <p><strong>Description:</strong> Swaps the front and back buffers to display the rendered content.</p>
        <pre><code class="language-cpp">
            WindowHandler& window = WindowHandler::GetInstance();
            window.SwapBuffers();
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">GLFWwindow*</span> 
            <span class="method-name">GetCurrentContext</span>()
        </summary>
        <p><strong>Description:</strong> Returns the current OpenGL context.</p>
        <pre><code class="language-cpp">
            WindowHandler& window = WindowHandler::GetInstance();
            GLFWwindow* context = window.GetCurrentContext();
        </code></pre>
        </details>
    </div>

    <h3>Static Methods</h3>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">static WindowHandler&</span> 
            <span class="method-name">GetInstance</span>()
        </summary>
        <p><strong>Description:</strong> Returns the singleton instance of <code>WindowHandler</code>.</p>
        <pre><code class="language-cpp">
            WindowHandler& window = WindowHandler::GetInstance();
        </code></pre>
        </details>
    </div>

    <h3>Private Methods</h3>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">static void</span> 
            <span class="method-name">GLFWErrorCallback</span>(int error, const char* description)
        </summary>
        <p><strong>Description:</strong> Handles GLFW errors by logging them.</p>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">static void</span> 
            <span class="method-name">GLFWWindowSizeCallback</span>(GLFWwindow* window, int width, int height)
        </summary>
        <p><strong>Description:</strong> Handles window resize events and updates the viewport.</p>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">static void</span> 
            <span class="method-name">MessageCallback</span>(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
        </summary>
        <p><strong>Description:</strong> Handles OpenGL debug messages.</p>
        </details>
    </div>

    <h3>Example Usage</h3>
    <pre><code class="language-cpp">
        #include "WindowHandler.h"

        int main() {
            // Get the WindowHandler instance
            WindowHandler& window = WindowHandler::GetInstance();

            // Initialize the window
            window.Init();

            // Game loop
            while (!window.ShouldClose()) {
                // Update window
                window.Update();

                // Swap buffers
                window.SwapBuffers();
            }

            // Shutdown the window
            window.Shutdown();

            return 0;
        }
    </code></pre>
    </div>
  `,
    input: `
    <div class="class">
    <h2>Input</h2>
    <p>The <code>Input</code> class handles keyboard, mouse, and gamepad inputs, providing abstraction over GLFW input functions and extending support for gamepad interactions.</p>

    <h3>Public Methods</h3>
    
    <div class="method">
        <details>
        <summary>
            <span class="return-type">static Input&</span> 
            <span class="method-name">GetInstance</span>()
        </summary>
        <p><strong>Description:</strong> Returns the singleton instance of the <code>Input</code> class.</p>
        <pre><code class="language-cpp">
            Input& input = Input::GetInstance();
        </code></pre>
        </details>
    </div>
    
    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">Init</span>(GLFWwindow* window)
        </summary>
        <p><strong>Description:</strong> Initializes the input system and sets up GLFW callbacks for handling keyboard, mouse, and joystick inputs.</p>
        <pre><code class="language-cpp">
            GLFWwindow* window = /* Your GLFW window */;
            Input& input = Input::GetInstance();
            input.Init(window);
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">Update</span>()
        </summary>
        <p><strong>Description:</strong> Updates the current frame's input state, including gamepad states.</p>
        <pre><code class="language-cpp">
            Input& input = Input::GetInstance();
            input.Update();
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">PostUpdate</span>()
        </summary>
        <p><strong>Description:</strong> Stores the current frame's input state as the previous frame's state for comparison.</p>
        <pre><code class="language-cpp">
            Input& input = Input::GetInstance();
            input.PostUpdate();
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">bool</span> 
            <span class="method-name">IsKeyPressed</span>(int key) const
        </summary>
        <p><strong>Description:</strong> Checks if a specific key is currently pressed.</p>
        <pre><code class="language-cpp">
            if (input.IsKeyPressed(GLFW_KEY_W)) {
                // Move forward
            }
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">bool</span> 
            <span class="method-name">IsMouseButtonPressed</span>(int button) const
        </summary>
        <p><strong>Description:</strong> Checks if a specific mouse button is currently pressed.</p>
        <pre><code class="language-cpp">
            if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
                // Handle left mouse click
            }
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">glm::vec2</span> 
            <span class="method-name">GetMouseNormalizedCoords</span>() const
        </summary>
        <p><strong>Description:</strong> Returns the normalized device coordinates of the mouse.</p>
        <pre><code class="language-cpp">
            glm::vec2 coords = input.GetMouseNormalizedCoords();
            std::cout << "Mouse X: " << coords.x << ", Y: " << coords.y << std::endl;
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">bool</span> 
            <span class="method-name">IsGamepadButtonPressed</span>(int jid, int button) const
        </summary>
        <p><strong>Description:</strong> Checks if a specific button on a gamepad is currently pressed.</p>
        <pre><code class="language-cpp">
            if (input.IsGamepadButtonPressed(0, GLFW_GAMEPAD_BUTTON_A)) {
                // Jump action
            }
        </code></pre>
        </details>
    </div>

    <h3>Static Callbacks</h3>

    <div class="callback">
        <details>
        <summary>
            <span class="return-type">static void</span> 
            <span class="method-name">KeyCallback</span>(GLFWwindow* window, int key, int scancode, int action, int mods)
        </summary>
        <p><strong>Description:</strong> Handles key press and release events.</p>
        </details>
    </div>

    <div class="callback">
        <details>
        <summary>
            <span class="return-type">static void</span> 
            <span class="method-name">MousePositionCallback</span>(GLFWwindow* window, double xpos, double ypos)
        </summary>
        <p><strong>Description:</strong> Updates the mouse position.</p>
        </details>
    </div>

    <div class="callback">
        <details>
        <summary>
            <span class="return-type">static void</span> 
            <span class="method-name">MouseScrollCallback</span>(GLFWwindow* window, double x, double y)
        </summary>
        <p><strong>Description:</strong> Updates the scroll state based on mouse wheel input.</p>
        </details>
    </div>

    <h3>Example Usage</h3>
    <pre><code class="language-cpp">
        #include "Input.h"

        int main() {
            GLFWwindow* window = glfwCreateWindow(800, 600, "Input Example", NULL, NULL);
            Input& input = Input::GetInstance();
            input.Init(window);

            while (!glfwWindowShouldClose(window)) {
                input.Update();

                if (input.IsKeyPressed(GLFW_KEY_ESCAPE)) {
                    glfwSetWindowShouldClose(window, true);
                }

                input.PostUpdate();
            }

            glfwDestroyWindow(window);
            return 0;
        }
    </code></pre>
    </div>
    `,
    ui:`
    <div class="class">
    <h2>UI</h2>
    <p>The <code>UI</code> class manages the graphical user interface using ImGui, including window rendering, user input handling, and various debug functionalities.</p>

    <h3>Public Methods</h3>
    
    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span>
            <span class="method-name">Init</span>(GLFWwindow* window)
        </summary>
        <p><strong>Description:</strong> Initializes the UI system, sets up ImGui context, and links it with the given GLFW window.</p>
        <pre><code class="language-cpp">
            GLFWwindow* window = /* GLFW window instance */;
            UI& ui = UI::GetInstance();
            ui.Init(window);
        </code></pre>
        </details>
    </div>
    
    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span>
            <span class="method-name">ShutDown</span>()
        </summary>
        <p><strong>Description:</strong> Cleans up resources and shuts down the ImGui context.</p>
        <pre><code class="language-cpp">
            UI& ui = UI::GetInstance();
            ui.ShutDown();
        </code></pre>
        </details>
    </div>
    
    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span>
            <span class="method-name">Update</span>()
        </summary>
        <p><strong>Description:</strong> Updates the UI state by capturing inputs and processing UI interactions.</p>
        <pre><code class="language-cpp">
            UI& ui = UI::GetInstance();
            ui.Update();
        </code></pre>
        </details>
    </div>
    
    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span>
            <span class="method-name">Render</span>()
        </summary>
        <p><strong>Description:</strong> Renders the ImGui windows and elements.</p>
        <pre><code class="language-cpp">
            UI& ui = UI::GetInstance();
            ui.Render();
        </code></pre>
        </details>
    </div>

    <h3>Private Members</h3>
    <details>
        <summary>Private Member Variables</summary>
        <ul>
        <li><code>std::vector&lt;IMGUI_ELEMENT*&gt; m_ImGuiElements</code>: Stores ImGui elements.</li>
        <li><code>std::ostringstream m_consoleBuffer</code>: Holds the console output buffer.</li>
        <li><code>IMGUI_DROPDOWN_MENU m_gameSelection</code>: Dropdown menu for selecting scenes.</li>
        <li><code>GLFWwindow* mp_window</code>: Pointer to the GLFW window instance.</li>
        <li><code>float m_mainMenuBarHeight</code>: Height of the main menu bar.</li>
        <li><code>bool m_toggleDebug</code>: Toggles debug mode.</li>
        </ul>
    </details>

    <h3>Static Methods</h3>
    <details>
        <summary>
        <span class="return-type">static UI&</span> 
        <span class="method-name">GetInstance</span>()
        </summary>
        <p><strong>Description:</strong> Returns the singleton instance of the <code>UI</code> class.</p>
        <pre><code class="language-cpp">
        UI& ui = UI::GetInstance();
        </code></pre>
    </details>

    <h3>Example Usage</h3>
    <pre><code class="language-cpp">
        #include "UI.h"

        int main() {
            // Initialize GLFW
            GLFWwindow* window = glfwCreateWindow(800, 600, "UI Example", NULL, NULL);
            UI& ui = UI::GetInstance();

            // Initialize the UI system
            ui.Init(window);

            // Main loop
            while (!glfwWindowShouldClose(window)) {
                ui.Update();

                // Render the UI
                ui.Render();
            }

            // Shutdown UI and GLFW
            ui.ShutDown();
            glfwDestroyWindow(window);

            return 0;
        }
    </code></pre>
    </div>
    `,
    time: `
    <div class="class">
    <h2>Time</h2>
    <p>The <code>Time</code> class handles time management for the application, providing functionalities for delta time and fixed delta time calculations.</p>

    <h3>Public Methods</h3>
    
    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">Init</span>(double fixedDT)
        </summary>
        <p><strong>Description:</strong> Initializes the time system with a fixed delta time.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>fixedDT</code>: The fixed delta time to be used in the time system.</li>
        </ul>
        <pre><code class="language-cpp">
            Time* time = Time::GetInstance();
            time->Init(0.016); // Initialize with a fixed delta time of 16ms (60 FPS)
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">Update</span>()
        </summary>
        <p><strong>Description:</strong> Updates the time system by calculating the delta time since the last update.</p>
        <pre><code class="language-cpp">
            Time* time = Time::GetInstance();
            time->Update();
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">const double</span> 
            <span class="method-name">GetFixedDeltaTime</span>() const
        </summary>
        <p><strong>Description:</strong> Returns the fixed delta time.</p>
        <p><strong>Returns:</strong> The fixed delta time as a <code>double</code>.</p>
        <pre><code class="language-cpp">
            Time* time = Time::GetInstance();
            double fixedDT = time->GetFixedDeltaTime();
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">const double</span> 
            <span class="method-name">GetDeltaTime</span>() const
        </summary>
        <p><strong>Description:</strong> Returns the delta time, representing the time elapsed since the last frame.</p>
        <p><strong>Returns:</strong> The delta time as a <code>double</code>.</p>
        <pre><code class="language-cpp">
            Time* time = Time::GetInstance();
            double deltaTime = time->GetDeltaTime();
        </code></pre>
        </details>
    </div>

    <h3>Private Members</h3>
    <details>
        <summary>Private Member Variables</summary>
        <ul>
        <li><code>static Time* instance</code>: Singleton instance of the <code>Time</code> class.</li>
        <li><code>double m_fixedDeltaTime</code>: The fixed delta time used in the system.</li>
        <li><code>double m_deltaTime</code>: The time elapsed since the last frame.</li>
        <li><code>std::chrono::time_point&lt;std::chrono::high_resolution_clock&gt; m_lastTime</code>: Timestamp of the last update.</li>
        </ul>
    </details>

    <h3>Static Methods</h3>
    <div class="method">
        <details>
        <summary>
            <span class="return-type">static Time*</span> 
            <span class="method-name">GetInstance</span>()
        </summary>
        <p><strong>Description:</strong> Returns the singleton instance of the <code>Time</code> class.</p>
        <pre><code class="language-cpp">
            Time* time = Time::GetInstance();
        </code></pre>
        </details>
    </div>

    <h3>Example Usage</h3>
    <pre><code class="language-cpp">
        #include "Time.h"

        int main() {
            // Get the Time instance
            Time* time = Time::GetInstance();

            // Initialize the time system with a fixed delta time of 16ms
            time->Init(0.016);

            // Main game loop
            while (true) {
                // Update the time system
                time->Update();

                // Retrieve the delta time
                double deltaTime = time->GetDeltaTime();
                double fixedDeltaTime = time->GetFixedDeltaTime();

                // Use delta time for updates (e.g., animations, physics)
                std::cout << "Delta Time: " << deltaTime << " seconds" << std::endl;
            }

            return 0;
        }
    </code></pre>
    </div>
    `,
    physicsManager: `
    <div class="class">
    <h2>PhysicsManager</h2>
    <p>The <code>PhysicsManager</code> class is responsible for managing all physics components and performing physics calculations, such as updating positions, velocities, and handling collisions.</p>

    <h3>Public Methods</h3>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">Init</span>()
        </summary>
        <p><strong>Description:</strong> Initializes the physics engine.</p>
        <pre><code class="language-cpp">
            PhysicsManager* physicsManager = PhysicsManager::GetInstance();
            physicsManager->Init();
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">Update</span>(double dt)
        </summary>
        <p><strong>Description:</strong> Updates the physics engine, applying physics calculations to all registered components.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>dt</code>: The delta time since the last update.</li>
        </ul>
        <pre><code class="language-cpp">
            double deltaTime = 0.016; // Assume 60 FPS
            physicsManager->Update(deltaTime);
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">Shutdown</span>()
        </summary>
        <p><strong>Description:</strong> Shuts down the physics engine, cleaning up all resources.</p>
        <pre><code class="language-cpp">
            physicsManager->Shutdown();
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">AddPhysicsComponent</span>(PhysicsComponent* component)
        </summary>
        <p><strong>Description:</strong> Adds a physics component to the physics engine for processing.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>component</code>: Pointer to the physics component to add.</li>
        </ul>
        <pre><code class="language-cpp">
            PhysicsComponent* newComponent = new PhysicsComponent();
            physicsManager->AddPhysicsComponent(newComponent);
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">RemovePhysicsComponent</span>(PhysicsComponent* component)
        </summary>
        <p><strong>Description:</strong> Removes a physics component from the physics engine.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>component</code>: Pointer to the physics component to remove.</li>
        </ul>
        <pre><code class="language-cpp">
            physicsManager->RemovePhysicsComponent(existingComponent);
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">glm::dvec3</span> 
            <span class="method-name">GetGravity</span>() const
        </summary>
        <p><strong>Description:</strong> Returns the current gravity vector applied to the physics world.</p>
        <p><strong>Returns:</strong> The gravity vector as a <code>glm::dvec3</code>.</p>
        <pre><code class="language-cpp">
            glm::dvec3 gravity = physicsManager->GetGravity();
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">SetGravity</span>(glm::vec3 g)
        </summary>
        <p><strong>Description:</strong> Sets a new gravity vector for the physics world.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>g</code>: The new gravity vector as <code>glm::vec3</code>.</li>
        </ul>
        <pre><code class="language-cpp">
            physicsManager->SetGravity(glm::vec3(0, -9.8, 0)); // Standard gravity
        </code></pre>
        </details>
    </div>

    <h3>Private Members</h3>
    <details>
        <summary>Private Member Variables</summary>
        <ul>
        <li><code>static PhysicsManager* instance</code>: Singleton instance of the <code>PhysicsManager</code>.</li>
        <li><code>glm::dvec3 m_gravity</code>: Gravity vector applied to the physics world.</li>
        <li><code>std::vector&lt;PhysicsComponent*&gt; m_physicsComponents</code>: List of all registered physics components.</li>
        </ul>
    </details>

    <h3>Static Methods</h3>
    <div class="method">
        <details>
        <summary>
            <span class="return-type">static PhysicsManager*</span> 
            <span class="method-name">GetInstance</span>()
        </summary>
        <p><strong>Description:</strong> Returns the singleton instance of the <code>PhysicsManager</code>.</p>
        <pre><code class="language-cpp">
            PhysicsManager* physicsManager = PhysicsManager::GetInstance();
        </code></pre>
        </details>
    </div>

    <h3>Example Usage</h3>
    <pre><code class="language-cpp">
        #include "PhysicsManager.h"
        #include "PhysicsComponent.h"

        int main() {
            // Get the PhysicsManager instance
            PhysicsManager* physicsManager = PhysicsManager::GetInstance();

            // Initialize the physics engine
            physicsManager->Init();

            // Set gravity
            physicsManager->SetGravity(glm::vec3(0, -9.8, 0));

            // Add a physics component
            PhysicsComponent* newComponent = new PhysicsComponent();
            physicsManager->AddPhysicsComponent(newComponent);

            // Run the physics update loop
            double deltaTime = 0.016; // Assume 60 FPS
            while (true) {
                physicsManager->Update(deltaTime);
            }

            // Shutdown the physics engine
            physicsManager->Shutdown();

            return 0;
        }
    </code></pre>
    </div>
    `,
    collisionManager:`
    <div class="class">
    <h2>CollisionManager</h2>
    <p>The <code>CollisionManager</code> class manages all collision components, performs collision detection, and handles collision responses for the game world.</p>

    <h3>Public Methods</h3>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">Init</span>()
        </summary>
        <p><strong>Description:</strong> Initializes the collision manager.</p>
        <pre><code class="language-cpp">
            CollisionManager* collisionManager = CollisionManager::GetInstance();
            collisionManager->Init();
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">Update</span>()
        </summary>
        <p><strong>Description:</strong> Updates the collision manager, checking for collisions and handling responses.</p>
        <pre><code class="language-cpp">
            collisionManager->Update();
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">Shutdown</span>()
        </summary>
        <p><strong>Description:</strong> Cleans up and shuts down the collision manager.</p>
        <pre><code class="language-cpp">
            collisionManager->Shutdown();
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">CollisionComponent*</span> 
            <span class="method-name">ShapeIsColliding</span>(CollisionComponent* component) const
        </summary>
        <p><strong>Description:</strong> Checks if the given component is colliding with any other collision component.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>component</code>: Pointer to the collision component to check.</li>
        </ul>
        <p><strong>Returns:</strong> A pointer to the colliding component, or <code>nullptr</code> if no collision.</p>
        <pre><code class="language-cpp">
            CollisionComponent* other = collisionManager->ShapeIsColliding(myComponent);
            if (other) {
                std::cout << "Collision detected!" << std::endl;
            }
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">bool</span> 
            <span class="method-name">ShapeIsCollidingWith</span>(CollisionComponent* component, CollisionComponent* other) const
        </summary>
        <p><strong>Description:</strong> Checks if one component is colliding with another specific component.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>component</code>: The first collision component.</li>
            <li><code>other</code>: The second collision component to check against.</li>
        </ul>
        <p><strong>Returns:</strong> <code>true</code> if the components are colliding, otherwise <code>false</code>.</p>
        <pre><code class="language-cpp">
            if (collisionManager->ShapeIsCollidingWith(componentA, componentB)) {
                std::cout << "Components are colliding!" << std::endl;
            }
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">AddCollisionComponent</span>(CollisionComponent* component)
        </summary>
        <p><strong>Description:</strong> Adds a collision component to the collision manager.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>component</code>: Pointer to the collision component to add.</li>
        </ul>
        <pre><code class="language-cpp">
            collisionManager->AddCollisionComponent(newCollisionComponent);
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">RemoveCollisionComponent</span>(CollisionComponent* component)
        </summary>
        <p><strong>Description:</strong> Removes a collision component from the collision manager.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>component</code>: Pointer to the collision component to remove.</li>
        </ul>
        <pre><code class="language-cpp">
            collisionManager->RemoveCollisionComponent(existingCollisionComponent);
        </code></pre>
        </details>
    </div>

    <h3>Private Members</h3>
    <details>
        <summary>Private Member Variables</summary>
        <ul>
        <li><code>static CollisionManager* instance</code>: Singleton instance of the collision manager.</li>
        <li><code>std::vector&lt;CollisionComponent*&gt; m_collisionComponents</code>: List of all registered collision components.</li>
        <li><code>double m_groundedOffset</code>: Offset used for determining grounded state.</li>
        </ul>
    </details>

    <h3>Static Methods</h3>
    <div class="method">
        <details>
        <summary>
            <span class="return-type">static CollisionManager*</span> 
            <span class="method-name">GetInstance</span>()
        </summary>
        <p><strong>Description:</strong> Returns the singleton instance of the <code>CollisionManager</code>.</p>
        <pre><code class="language-cpp">
            CollisionManager* collisionManager = CollisionManager::GetInstance();
        </code></pre>
        </details>
    </div>

    <h3>Example Usage</h3>
    <pre><code class="language-cpp">
        #include "CollisionManager.h"
        #include "CollisionComponent.h"

        int main() {
            // Get the CollisionManager instance
            CollisionManager* collisionManager = CollisionManager::GetInstance();

            // Initialize the collision manager
            collisionManager->Init();

            // Create and add collision components
            CollisionComponent* componentA = collisionManager->CreateCollisionComponent(1, 0xFFFF);
            CollisionComponent* componentB = collisionManager->CreateCollisionComponent(1, 0xFFFF);
            collisionManager->AddCollisionComponent(componentA);
            collisionManager->AddCollisionComponent(componentB);

            // Update the collision manager
            collisionManager->Update();

            // Check collisions
            if (collisionManager->ShapeIsColliding(componentA)) {
                std::cout << "Collision detected!" << std::endl;
            }

            // Shutdown the collision manager
            collisionManager->Shutdown();

            return 0;
        }
    </code></pre>
    </div>
    `,
    eventHandler:`
    <div class="class">
    <h2>EventHandler</h2>
    <p>The <code>EventHandler</code> class manages event-based communication between different parts of the system using an event queue and subscription mechanism.</p>

    <h3>Public Methods</h3>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">static EventHandler*</span> 
            <span class="method-name">GetInstance</span>()
        </summary>
        <p><strong>Description:</strong> Returns the singleton instance of the <code>EventHandler</code>.</p>
        <pre><code class="language-cpp">
            EventHandler* eventHandler = EventHandler::GetInstance();
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">Update</span>()
        </summary>
        <p><strong>Description:</strong> Processes all events in the queue, notifying subscribers for each event.</p>
        <pre><code class="language-cpp">
            eventHandler->Update();
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">AddEvent</span>(Event* event)
        </summary>
        <p><strong>Description:</strong> Adds a new event to the event queue.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>event</code>: Pointer to the event to be added to the queue.</li>
        </ul>
        <pre><code class="language-cpp">
            Event* event = new Event("MyEvent");
            eventHandler->AddEvent(event);
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">AddSubscription</span>(std::string name, EventListener* eventListener)
        </summary>
        <p><strong>Description:</strong> Subscribes an event listener to a specific event type.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>name</code>: The name of the event type to subscribe to.</li>
            <li><code>eventListener</code>: Pointer to the event listener to subscribe.</li>
        </ul>
        <pre><code class="language-cpp">
            EventListener* listener = new MyEventListener();
            eventHandler->AddSubscription("MyEvent", listener);
        </code></pre>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">RemoveSubscription</span>(std::string name, EventListener* listener)
        </summary>
        <p><strong>Description:</strong> Removes a specific listener's subscription to an event type.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>name</code>: The name of the event type.</li>
            <li><code>listener</code>: Pointer to the event listener to unsubscribe.</li>
        </ul>
        <pre><code class="language-cpp">
            eventHandler->RemoveSubscription("MyEvent", listener);
        </code></pre>
        </details>
    </div>

    <h3>Private Members</h3>
    <details>
        <summary>Private Member Variables</summary>
        <ul>
        <li><code>std::queue&lt;Event*&gt; eventQueue</code>: A queue to hold all pending events.</li>
        <li><code>std::multimap&lt;std::string, EventListener*&gt; subscriptionMap</code>: Maps event types to their subscribed listeners.</li>
        <li><code>static EventHandler* instance</code>: Singleton instance of the <code>EventHandler</code>.</li>
        </ul>
    </details>

    <h3>Static Methods</h3>
    <div class="method">
        <details>
        <summary>
            <span class="return-type">static EventHandler*</span> 
            <span class="method-name">GetInstance</span>()
        </summary>
        <p><strong>Description:</strong> Returns the singleton instance of the <code>EventHandler</code>.</p>
        <pre><code class="language-cpp">
            EventHandler* eventHandler = EventHandler::GetInstance();
        </code></pre>
        </details>
    </div>

    <h3>Example Usage</h3>
    <pre><code class="language-cpp">
        #include "EventHandler.h"
        #include "Event.h"
        #include "EventListener.h"

        int main() {
            // Get the EventHandler instance
            EventHandler* eventHandler = EventHandler::GetInstance();

            // Create a custom event
            Event* myEvent = new Event("TestEvent");

            // Create and register a listener
            EventListener* listener = new MyEventListener();
            eventHandler->AddSubscription("TestEvent", listener);

            // Add the event to the queue
            eventHandler->AddEvent(myEvent);

            // Process all events
            eventHandler->Update();

            // Remove the listener subscription
            eventHandler->RemoveSubscription("TestEvent", listener);

            return 0;
        }
    </code></pre>
    </div>
    `,
    scriptManager:`
    <div class="class">
    <h2>ScriptManager</h2>
    <p>The <code>ScriptManager</code> class is responsible for managing Lua scripting components and exposing C++ classes to Lua.</p>

    <h3>Public Methods</h3>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">Init</span>()
        </summary>
        <p><strong>Description:</strong> Initializes the Lua environment and exposes necessary classes and functions to Lua.</p>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">Update</span>()
        </summary>
        <p><strong>Description:</strong> Updates all registered <code>ScriptComponent</code> instances, allowing them to execute their logic.</p>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">Shutdown</span>()
        </summary>
        <p><strong>Description:</strong> Cleans up and shuts down the Lua environment, releasing all associated resources.</p>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">ExposeClasses</span>()
        </summary>
        <p><strong>Description:</strong> Exposes C++ classes and functions to the Lua environment, enabling interaction between Lua scripts and the engine.</p>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">AddScriptComponent</span>(ScriptComponent* component)
        </summary>
        <p><strong>Description:</strong> Adds a <code>ScriptComponent</code> to the manager for updates.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>component</code>: Pointer to the <code>ScriptComponent</code> to be added.</li>
        </ul>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">RemoveScriptComponent</span>(ScriptComponent* component)
        </summary>
        <p><strong>Description:</strong> Removes a <code>ScriptComponent</code> from the manager, stopping it from being updated.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>component</code>: Pointer to the <code>ScriptComponent</code> to be removed.</li>
        </ul>
        </details>
    </div>

    <h3>Private Members</h3>
    <details>
        <summary>Private Member Variables</summary>
        <ul>
        <li><code>std::vector&lt;ScriptComponent*&gt; m_scriptComponents</code>: A list of all managed <code>ScriptComponent</code> instances.</li>
        <li><code>sol::state m_lua</code>: The Lua scripting environment.</li>
        <li><code>static ScriptManager* instance</code>: Singleton instance of the <code>ScriptManager</code>.</li>
        </ul>
    </details>

    <h3>Static Methods</h3>
    <div class="method">
        <details>
        <summary>
            <span class="return-type">static ScriptManager*</span> 
            <span class="method-name">GetInstance</span>()
        </summary>
        <p><strong>Description:</strong> Returns the singleton instance of the <code>ScriptManager</code>.</p>
        </details>
    </div>

    <h3>Example Usage</h3>
    <pre><code class="language-cpp">
        #include "ScriptManager.h"

        int main() {
            // Get the ScriptManager instance
            ScriptManager* scriptManager = ScriptManager::GetInstance();

            // Initialize the script manager
            scriptManager->Init();

            // Update scripts (typically called in the game loop)
            scriptManager->Update();

            // Shut down the script manager
            scriptManager->Shutdown();

            return 0;
        }
    </code></pre>
    </div>
    `,
    sceneManager:`
    <div class="class">
    <h2>SceneManager</h2>
    <p>The <code>SceneManager</code> class is responsible for managing scenes in the engine, including scene addition, removal, and exporting to JSON.</p>

    <h3>Public Methods</h3>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">ExportScene</span>(Scene* scene)
        </summary>
        <p><strong>Description:</strong> Exports the specified scene to a JSON file.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>scene</code>: Pointer to the <code>Scene</code> object to export.</li>
        </ul>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">AddScene</span>(std::string name = "", const std::source_location& location = std::source_location::current())
        </summary>
        <p><strong>Description:</strong> Adds a new scene to the manager. If no name is provided, a default name will be assigned.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>name</code>: Name of the scene (optional).</li>
            <li><code>location</code>: Source location information (optional).</li>
        </ul>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">RemoveScene</span>(const Scene* scene)
        </summary>
        <p><strong>Description:</strong> Removes a scene from the manager.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>scene</code>: Pointer to the <code>Scene</code> object to remove.</li>
        </ul>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">RemoveScene</span>(const std::string& name)
        </summary>
        <p><strong>Description:</strong> Removes a scene from the manager by its name.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>name</code>: Name of the scene to remove.</li>
        </ul>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">SetSceneName</span>(const Scene* scene, const std::string& name)
        </summary>
        <p><strong>Description:</strong> Renames an existing scene and updates the associated file paths.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>scene</code>: Pointer to the <code>Scene</code> object.</li>
            <li><code>name</code>: New name for the scene.</li>
        </ul>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">MoveToNextScene</span>()
        </summary>
        <p><strong>Description:</strong> Switches to the next scene in the manager.</p>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">MoveToPreviousScene</span>()
        </summary>
        <p><strong>Description:</strong> Switches to the previous scene in the manager.</p>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">SetCurrentScene</span>(const Scene* scene)
        </summary>
        <p><strong>Description:</strong> Sets the specified scene as the current active scene.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>scene</code>: Pointer to the <code>Scene</code> object to activate.</li>
        </ul>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">SetCurrentScene</span>(const std::string& name)
        </summary>
        <p><strong>Description:</strong> Sets the scene with the specified name as the current active scene.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>name</code>: Name of the scene to activate.</li>
        </ul>
        </details>
    </div>

    <h3>Example Usage</h3>
    <pre><code class="language-cpp">
        #include "SceneManager.h"

        int main() {
            // Get the SceneManager instance
            SceneManager* sceneManager = SceneManager::GetInstance();

            // Add a new scene
            sceneManager->AddScene("MyScene");

            // Set the current scene
            sceneManager->SetCurrentScene("MyScene");

            // Export the current scene
            Scene* currentScene = sceneManager->GetCurrentScene();
            sceneManager->ExportScene(currentScene);

            // Remove the scene
            sceneManager->RemoveScene("MyScene");

            return 0;
        }
    </code></pre>
    </div>
    `,
    resourceFactory:`
        <div class="class">
        <h2>ResourceFactory</h2>
        <p>The <code>ResourceFactory</code> class is responsible for creating and managing game resources such as textures, shaders, materials, and geometries from a JSON source file.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">CreateAllResources</span>(const char* source)
            </summary>
            <p><strong>Description:</strong> Loads and creates all resources from a specified JSON file.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>source</code>: Path to the JSON file containing resource data.</li>
            </ul>
            </details>
        </div>

        <h3>Private Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">createResource</span>(rapidjson::Value::ConstMemberIterator member, ResourceType type = ResourceType::UNKNOWN)
            </summary>
            <p><strong>Description:</strong> Creates a specific type of resource based on the provided data and resource type.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>member</code>: JSON member containing resource data.</li>
                <li><code>type</code>: Type of resource to create (e.g., <code>TEXTURE</code>, <code>SHADER</code>, etc.).</li>
            </ul>
            </details>
        </div>

        <h3>Private Members</h3>
        <ul>
            <li><code>ResourceFactory* instance</code>: Singleton instance of the class.</li>
        </ul>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
            #include "ResourceFactory.h"

            int main() {
                // Get the ResourceFactory instance
                ResourceFactory* resourceFactory = ResourceFactory::GetInstance();

                // Load resources from a JSON file
                resourceFactory->CreateAllResources("resources.json");

                return 0;
            }
        </code></pre>
        </div>
    `,
    resourceManager:`
    <div class="class">
    <h2>ResourceManager</h2>
    <p>The <code>ResourceManager</code> class is responsible for managing various game resources such as textures, shaders, materials, and geometries. It provides functionality to add, remove, and retrieve resources by name.</p>

    <h3>Public Methods</h3>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">AddTexture</span>(const std::string&amp; name, Texture* texture)
        </summary>
        <p><strong>Description:</strong> Adds a texture to the resource manager.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>name</code>: The name associated with the texture.</li>
            <li><code>texture</code>: The texture to add.</li>
        </ul>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">AddShader</span>(const std::string&amp; name, Shader* shader)
        </summary>
        <p><strong>Description:</strong> Adds a shader to the resource manager.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>name</code>: The name associated with the shader.</li>
            <li><code>shader</code>: The shader to add.</li>
        </ul>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">AddMaterial</span>(const std::string&amp; name, Material* material)
        </summary>
        <p><strong>Description:</strong> Adds a material to the resource manager.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>name</code>: The name associated with the material.</li>
            <li><code>material</code>: The material to add.</li>
        </ul>
        </details>
    </div>

    <div class="method">
        <details>
        <summary>
            <span class="return-type">void</span> 
            <span class="method-name">AddGeometry</span>(const std::string&amp; name, Geometry* geometry)
        </summary>
        <p><strong>Description:</strong> Adds a geometry to the resource manager.</p>
        <p><strong>Parameters:</strong></p>
        <ul>
            <li><code>name</code>: The name associated with the geometry.</li>
            <li><code>geometry</code>: The geometry to add.</li>
        </ul>
        </details>
    </div>

    <h3>Private Members</h3>
    <ul>
        <li><code>std::unordered_map&lt;std::string, Texture*&gt; m_textures</code>: Stores textures by name.</li>
        <li><code>std::unordered_map&lt;std::string, Shader*&gt; m_shaders</code>: Stores shaders by name.</li>
        <li><code>std::unordered_map&lt;std::string, Material*&gt; m_materials</code>: Stores materials by name.</li>
        <li><code>std::unordered_map&lt;std::string, Geometry*&gt; m_geometries</code>: Stores geometries by name.</li>
    </ul>

    <h3>Example Usage</h3>
    <pre><code class="language-cpp">
        #include "ResourceManager.h"

        int main() {
            // Get the ResourceManager instance
            ResourceManager* resourceManager = ResourceManager::GetInstance();

            // Add a texture
            Texture* myTexture = new Texture("path/to/texture.png");
            resourceManager->AddTexture("myTexture", myTexture);

            // Retrieve the texture by name
            Texture* retrievedTexture = resourceManager->GetTexture("myTexture");

            // Remove the texture
            resourceManager->RemoveTexture("myTexture");

            return 0;
        }
    </code></pre>
    </div>
    `,
    gameObjectFactory:`
        <div class="class">
        <h2>GameObjectFactory</h2>
        <p>The <code>GameObjectFactory</code> class is responsible for creating and configuring game objects from JSON data. It supports hierarchical game object structures and component-based architecture.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">CreateAllGameObjects</span>(const rapidjson::Value&amp; gameObjects)
            </summary>
            <p><strong>Description:</strong> Loads and creates all game objects from a JSON structure.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>gameObjects</code>: A JSON object containing game object definitions.</li>
            </ul>
            </details>
        </div>

        <h3>Private Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">createGameObject</span>(rapidjson::Value::ConstMemberIterator member, GameObject* pParent = nullptr)
            </summary>
            <p><strong>Description:</strong> Creates a game object with its components and children recursively.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>member</code>: An iterator to a JSON member representing the game object.</li>
                <li><code>pParent</code>: (Optional) The parent game object to which this game object will be added.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">deserialize</span>(const rapidjson::Value&amp; components, GameObject* gameObject)
            </summary>
            <p><strong>Description:</strong> Processes the components JSON and attaches them to the game object.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>components</code>: A JSON value containing component definitions.</li>
                <li><code>gameObject</code>: The game object to which components will be added.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">CollisionShape*</span> 
                <span class="method-name">parseCollisionShape</span>(const rapidjson::Value&amp; collisionShapeData)
            </summary>
            <p><strong>Description:</strong> Parses collision shape data and creates a corresponding collision shape object.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>collisionShapeData</code>: A JSON value containing collision shape properties.</li>
            </ul>
            <p><strong>Returns:</strong> A pointer to the created <code>CollisionShape</code>.</p>
            </details>
        </div>

        <h3>Private Members</h3>
        <ul>
            <li><code>static GameObjectFactory* instance</code>: Singleton instance of the factory.</li>
            <li><code>void createGameObject</code>: Creates game objects recursively.</li>
            <li><code>void deserialize</code>: Attaches components to game objects.</li>
            <li><code>CollisionShape* parseCollisionShape</code>: Parses collision shapes for physics or collision components.</li>
        </ul>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
            #include "GameObjectFactory.h"

            int main() {
                // Example JSON representation of game objects
                rapidjson::Document gameObjectsJson;
                gameObjectsJson.Parse(R"({
                    "Player": {
                        "Components": {
                            "TransformComponent": {
                                "Position": ["glm::vec3", [0, 1, 0]],
                                "Scale": ["glm::vec3", [1, 1, 1]]
                            }
                        },
                        "Children": {
                            "Weapon": {
                                "Components": {
                                    "RenderComponent": {
                                        "Color": ["glm::vec3", [1, 0, 0]]
                                    }
                                }
                            }
                        }
                    }
                })");

                // Create game objects using the factory
                GameObjectFactory* factory = GameObjectFactory::GetInstance();
                factory->CreateAllGameObjects(gameObjectsJson);

                return 0;
            }
        </code></pre>
        </div>
    `,
    gameObjectManager:`
        <div class="class">
        <h2>GameObjectManager</h2>
        <p>The <code>GameObjectManager</code> class manages all game objects in the application. It handles their lifecycle, updates, rendering, and provides utility functions for efficient management and retrieval.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Init</span>()
            </summary>
            <p><strong>Description:</strong> Initializes the game objects and the manager itself, including creating necessary framebuffers.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Update</span>()
            </summary>
            <p><strong>Description:</strong> Updates all game objects managed by the manager.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Render</span>()
            </summary>
            <p><strong>Description:</strong> Renders all game objects, handling shadow mapping and deferred rendering setups.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Shutdown</span>()
            </summary>
            <p><strong>Description:</strong> Destroys all game objects and cleans up resources.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">AddGameObject</span>(GameObject* object)
            </summary>
            <p><strong>Description:</strong> Adds a game object to the manager and the current scene.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>object</code>: The game object to add.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">DeleteGameObject</span>(GameObject* object)
            </summary>
            <p><strong>Description:</strong> Deletes a game object from the manager and cleans up its resources.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>object</code>: The game object to delete.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">const GameObject*</span> 
                <span class="method-name">GetGameObject</span>(const std::string name)
            </summary>
            <p><strong>Description:</strong> Retrieves a game object by its name.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>name</code>: The name of the game object to retrieve.</li>
            </ul>
            <p><strong>Returns:</strong> A pointer to the game object if found.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">const std::vector&lt;GameObject*&gt;</span> 
                <span class="method-name">GetGameObjects</span>()
            </summary>
            <p><strong>Description:</strong> Returns the list of all game objects managed by the manager.</p>
            <p><strong>Returns:</strong> A vector containing pointers to all game objects.</p>
            </details>
        </div>

        <h3>Private Members</h3>
        <ul>
            <li><code>static GameObjectManager* instance</code>: Singleton instance of the manager.</li>
            <li><code>std::unordered_map&lt;std::string, size_t&gt; m_gameObjectMap</code>: Maps game object names to their indices in the vector.</li>
            <li><code>std::vector&lt;GameObject*&gt; m_gameObjects</code>: Stores all managed game objects.</li>
            <li><code>FBO* gbuffer</code>: Framebuffer object for deferred rendering.</li>
            <li><code>FBO* m_shadowBuffer</code>: Framebuffer object for shadow mapping.</li>
            <li><code>glm::mat4 lightView</code>: Matrix for light view transformations.</li>
        </ul>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
            #include "GameObjectManager.h"
            
            int main() {
                // Get the singleton instance
                GameObjectManager* manager = GameObjectManager::GetInstance();

                // Initialize the manager
                manager->Init();

                // Create and add a new game object
                GameObject* player = new GameObject();
                player->SetName("Player");
                manager->AddGameObject(player);

                // Update and render the game objects
                manager->Update();
                manager->Render();

                // Shutdown and clean up
                manager->Shutdown();

                return 0;
            }
        </code></pre>
        </div>
    `,
    shader: `
        <div class="class">
        <h2>Shader</h2>
        <p>The <code>Shader</code> class encapsulates the creation and management of OpenGL shaders. It provides utility methods for loading, using, and setting uniform variables within the shader programs.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">Shader</span> 
                <span class="method-name">Shader</span>(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
            </summary>
            <p><strong>Description:</strong> Constructor that loads the shader program from vertex, fragment, and optionally geometry shader files.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>vertexPath</code>: Path to the vertex shader file.</li>
                <li><code>fragmentPath</code>: Path to the fragment shader file.</li>
                <li><code>geometryPath</code>: Path to the geometry shader file (optional).</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Use</span>()
            </summary>
            <p><strong>Description:</strong> Activates the shader program for use in rendering.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Unuse</span>()
            </summary>
            <p><strong>Description:</strong> Deactivates the shader program.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">GLint</span> 
                <span class="method-name">GetAttributeLocation</span>(const std::string& name)
            </summary>
            <p><strong>Description:</strong> Retrieves the location of an attribute variable within the shader program.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>name</code>: The name of the attribute variable.</li>
            </ul>
            <p><strong>Returns:</strong> The location ID of the attribute.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">ClearUniformCache</span>()
            </summary>
            <p><strong>Description:</strong> Clears the cached uniform variable locations.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">GLint</span> 
                <span class="method-name">GetUniformLocation</span>(const std::string& name)
            </summary>
            <p><strong>Description:</strong> Retrieves the location of a uniform variable within the shader program, using a cache to avoid redundant OpenGL calls.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>name</code>: The name of the uniform variable.</li>
            </ul>
            <p><strong>Returns:</strong> The location ID of the uniform variable.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetUniform</span>&lt;T&gt;(const std::string& name, const T& value)
            </summary>
            <p><strong>Description:</strong> Sets the value of a uniform variable in the shader program.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>name</code>: The name of the uniform variable.</li>
                <li><code>value</code>: The value to assign to the uniform variable.</li>
            </ul>
            <p><strong>Template Parameters:</strong></p>
            <ul>
                <li><code>T</code>: The type of the value (e.g., <code>bool</code>, <code>int</code>, <code>float</code>, or GLM types).</li>
            </ul>
            </details>
        </div>

        <h3>Private Members</h3>
        <ul>
            <li><code>GLuint m_id</code>: OpenGL ID of the shader program.</li>
            <li><code>std::unordered_map&lt;std::string, GLint&gt; m_uniformCache</code>: Cache for uniform variable locations.</li>
        </ul>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
            #include "Shader.h"

            int main() {
                // Create a Shader object
                Shader shader("vertex.glsl", "fragment.glsl");

                // Use the shader program
                shader.Use();

                // Set a uniform variable
                glm::vec3 color(1.0f, 0.5f, 0.2f);
                shader.SetUniform("u_Color", color);

                // Render your objects...

                // Deactivate the shader program
                shader.Unuse();

                return 0;
            }
        </code></pre>
        </div>
    `,
    texture: `
        <div class="class">
        <h2>Texture</h2>
        <p>The <code>Texture</code> class is designed for loading and managing 2D textures. It provides utilities for reading texture files, assigning texture data, and accessing texture properties such as dimensions and image buffers.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">Texture</span> 
                <span class="method-name">Texture</span>(const char* filename)
            </summary>
            <p><strong>Description:</strong> Constructor that loads a texture file upon creation.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>filename</code>: Path to the texture file.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">AssignTextureToDest</span>(glm::vec3*& dest)
            </summary>
            <p><strong>Description:</strong> Assigns the texture image buffer to the provided destination pointer.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>dest</code>: Pointer to store the texture image buffer.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">LoadTexture</span>(const char* filename)
            </summary>
            <p><strong>Description:</strong> Loads a texture file and updates the texture data.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>filename</code>: Path to the texture file.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">unsigned int</span> 
                <span class="method-name">GetWidth</span>()
            </summary>
            <p><strong>Description:</strong> Returns the width of the texture.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">unsigned int</span> 
                <span class="method-name">GetHeight</span>()
            </summary>
            <p><strong>Description:</strong> Returns the height of the texture.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">glm::vec3*</span> 
                <span class="method-name">GetImage</span>()
            </summary>
            <p><strong>Description:</strong> Returns the image buffer containing the texture data.</p>
            </details>
        </div>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
            // Load a texture from file
            Texture texture("path/to/texture.png");

            // Retrieve texture properties
            unsigned int width = texture.GetWidth();
            unsigned int height = texture.GetHeight();
            glm::vec3* imageBuffer = texture.GetImage();

            // Assign texture to a destination buffer
            glm::vec3* destBuffer;
            texture.AssignTextureToDest(destBuffer);
        </code></pre>
        </div>
    `,
    material: `
        <div class="class">
        <h2>Material</h2>
        <p>The <code>Material</code> class encapsulates the management of material properties for rendering. It integrates with shaders and textures to control how surfaces are displayed in the graphics pipeline.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">Material</span> 
                <span class="method-name">Material</span>()
            </summary>
            <p><strong>Description:</strong> Default constructor initializing material with default shader and properties.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">Material</span> 
                <span class="method-name">Material</span>(Shader* pShader)
            </summary>
            <p><strong>Description:</strong> Constructor initializing material with a specified shader.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>pShader</code>: Pointer to a custom shader object.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetShader</span>(Shader* shader)
            </summary>
            <p><strong>Description:</strong> Sets a custom shader for the material.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>shader</code>: Pointer to the new shader.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetShader</span>(const std::string name)
            </summary>
            <p><strong>Description:</strong> Sets a shader by name using the resource manager.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>name</code>: Name of the shader to retrieve and set.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetTextureDiffuse</span>(Texture* texture)
            </summary>
            <p><strong>Description:</strong> Applies a diffuse texture to the material.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>texture</code>: Pointer to the diffuse texture.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetTextureSpecular</span>(Texture* texture)
            </summary>
            <p><strong>Description:</strong> Applies a specular texture to the material.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>texture</code>: Pointer to the specular texture.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetColor</span>(glm::vec3 color)
            </summary>
            <p><strong>Description:</strong> Sets the material color using a <code>glm::vec3</code>.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>color</code>: RGB color values.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetShininess</span>(float shininess)
            </summary>
            <p><strong>Description:</strong> Sets the shininess property of the material, controlling specular highlights.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>shininess</code>: Shininess factor.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetupUniformData</span>()
            </summary>
            <p><strong>Description:</strong> Passes material properties to the shader as uniform variables.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Bind</span>()
            </summary>
            <p><strong>Description:</strong> Binds textures to their respective texture units for rendering.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Unbind</span>()
            </summary>
            <p><strong>Description:</strong> Unbinds textures from texture units.</p>
            </details>
        </div>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
            Material material;
            material.SetColor(glm::vec3(1.0f, 0.5f, 0.2f));
            material.SetShininess(32.0f);
            material.SetShader("CustomShader");
            material.SetTextureDiffuse(diffuseTexture);
            material.SetTextureSpecular(specularTexture);
            
            material.SetupUniformData();
            material.Bind();

            // Render your object...

            material.Unbind();
        </code></pre>
        </div>
  `,
    geometry: `
        <div class="class">
        <h2>Geometry</h2>
        <p>The <code>Geometry</code> class manages 3D geometric data, including vertices, normals, and texture coordinates. It supports loading data from OBJ files, binding buffers for rendering, and setting UV mapping types.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">Geometry</span> 
                <span class="method-name">Geometry</span>()
            </summary>
            <p><strong>Description:</strong> Default constructor. Initializes buffers and sets default UV mapping to <code>PLANAR</code>.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">Geometry</span> 
                <span class="method-name">Geometry</span>(const char* path)
            </summary>
            <p><strong>Description:</strong> Constructor that loads geometry data from an OBJ file.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>path</code>: Path to the OBJ file.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">bool</span> 
                <span class="method-name">LoadGeometry</span>(const char* path)
            </summary>
            <p><strong>Description:</strong> Loads geometric data from an OBJ file.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>path</code>: Path to the OBJ file.</li>
            </ul>
            <p><strong>Returns:</strong> <code>true</code> if successful, <code>false</code> otherwise.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Bind</span>(Shader* shader)
            </summary>
            <p><strong>Description:</strong> Binds vertex, normal, UV, and index data to buffers, linking attributes to the provided shader.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>shader</code>: Pointer to the shader program.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Unbind</span>()
            </summary>
            <p><strong>Description:</strong> Unbinds all buffers and vertex arrays.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">CleanUpBuffers</span>()
            </summary>
            <p><strong>Description:</strong> Deletes all buffers and vertex arrays to free GPU memory.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Render</span>()
            </summary>
            <p><strong>Description:</strong> Renders the geometry using the bound buffers.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetUVType</span>(UV_TYPE type)
            </summary>
            <p><strong>Description:</strong> Sets the type of UV mapping to use for texture coordinates.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>type</code>: The UV mapping type (e.g., <code>PLANAR</code>, <code>CYLINDRICAL</code>, etc.).</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">UV_TYPE</span> 
                <span class="method-name">GetUVType</span>()
            </summary>
            <p><strong>Description:</strong> Retrieves the current UV mapping type.</p>
            <p><strong>Returns:</strong> The current UV mapping type.</p>
            </details>
        </div>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
            // Create a geometry object and load data from an OBJ file
            Geometry geometry("path/to/model.obj");

            // Bind the geometry to a shader
            Shader shader("vertex.glsl", "fragment.glsl");
            geometry.Bind(&shader);

            // Render the geometry
            geometry.Render();

            // Unbind after rendering
            geometry.Unbind();
        </code></pre>
        </div>
  `,
    transform: `
        <div class="class">
        <h2>Transform</h2>
        <p>The <code>Transform</code> class manages spatial properties such as position, rotation, and scale. It calculates and maintains model, view, and projection matrices for rendering in a 3D environment.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetPosition</span>(glm::vec3 position)
            </summary>
            <p><strong>Description:</strong> Sets the position of the transform and updates the model matrix.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>position</code>: The new position of the transform.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetRotation</span>(glm::vec3 rotation)
            </summary>
            <p><strong>Description:</strong> Sets the rotation of the transform and updates the model matrix.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>rotation</code>: The new rotation of the transform in degrees.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetScale</span>(glm::vec3 scale)
            </summary>
            <p><strong>Description:</strong> Sets the scale of the transform and updates the model matrix.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>scale</code>: The new scale of the transform.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetProjection</span>(glm::mat4 projection)
            </summary>
            <p><strong>Description:</strong> Assigns a projection matrix to the transform.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>projection</code>: The projection matrix.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetView</span>(glm::mat4 view)
            </summary>
            <p><strong>Description:</strong> Assigns a view matrix to the transform.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>view</code>: The view matrix.</li>
            </ul>
            </details>
        </div>

        <h3>Getters</h3>
        <ul>
            <li><code>glm::vec3 GetPosition()</code>: Returns the position of the transform.</li>
            <li><code>glm::vec3 GetRotation()</code>: Returns the rotation of the transform in degrees.</li>
            <li><code>glm::vec3 GetScale()</code>: Returns the scale of the transform.</li>
            <li><code>glm::mat4 GetTranslationMatrix()</code>: Returns the translation matrix of the transform.</li>
            <li><code>glm::mat4 GetRotationMatrix()</code>: Returns the rotation matrix of the transform.</li>
            <li><code>glm::mat4 GetScaleMatrix()</code>: Returns the scale matrix of the transform.</li>
            <li><code>glm::mat4 GetModel()</code>: Returns the model matrix of the transform.</li>
            <li><code>glm::mat4 GetProjection()</code>: Returns the projection matrix of the transform.</li>
            <li><code>glm::mat4 GetView()</code>: Returns the view matrix of the transform.</li>
        </ul>

        <h3>Private Methods</h3>
        <ul>
            <li><code>void updateModelMatrix()</code>: Helper function to update the model matrix based on the position, rotation, and scale.</li>
        </ul>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
            // Create a Transform object
            Transform transform;

            // Set position, rotation, and scale
            transform.SetPosition(glm::vec3(1.0f, 2.0f, 3.0f));
            transform.SetRotation(glm::vec3(45.0f, 30.0f, 90.0f));
            transform.SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

            // Retrieve matrices for rendering
            glm::mat4 modelMatrix = transform.GetModel();
            glm::mat4 viewMatrix = transform.GetView();
            glm::mat4 projectionMatrix = transform.GetProjection();
        </code></pre>
        </div>
  `,
    scene: `
        <div class="class">
        <h2>Scene</h2>
        <p>The <code>Scene</code> class represents a collection of game objects (nodes) and manages their lifecycle. It supports initialization, updating, rendering, and shutdown of nodes and other scene-specific resources like skyboxes.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Init</span>()
            </summary>
            <p><strong>Description:</strong> Initializes the scene by loading game objects and resources from a JSON file. Also initializes the skybox if specified.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Update</span>()
            </summary>
            <p><strong>Description:</strong> Updates all nodes in the scene.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Render</span>()
            </summary>
            <p><strong>Description:</strong> Renders all nodes in the scene.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">PostUpdate</span>()
            </summary>
            <p><strong>Description:</strong> Flushes updates to all nodes in the scene.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Shutdown</span>()
            </summary>
            <p><strong>Description:</strong> Cleans up and destroys all nodes and scene resources.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">AddNode</span>(Node* node)
            </summary>
            <p><strong>Description:</strong> Adds a new node to the scene and assigns it a unique ID.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>node</code>: The node to add.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">DeleteNode</span>(Node* node)
            </summary>
            <p><strong>Description:</strong> Deletes a node from the scene and destroys it.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>node</code>: The node to delete.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">Node*</span> 
                <span class="method-name">RemoveNode</span>(Node* node)
            </summary>
            <p><strong>Description:</strong> Removes a node from the scene without destroying it. Only root nodes can be removed.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>node</code>: The node to remove.</li>
            </ul>
            <p><strong>Returns:</strong> The removed node or <code>nullptr</code> if the node is not a root node.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">std::vector&lt;Node*&gt;</span> 
                <span class="method-name">GetNodes</span>()
            </summary>
            <p><strong>Description:</strong> Retrieves a list of all nodes in the scene.</p>
            <p><strong>Returns:</strong> A vector of pointers to nodes.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">std::string</span> 
                <span class="method-name">GetName</span>()
            </summary>
            <p><strong>Description:</strong> Retrieves the name of the scene.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">std::string</span> 
                <span class="method-name">GetSceneSource</span>()
            </summary>
            <p><strong>Description:</strong> Retrieves the source file path of the scene.</p>
            </details>
        </div>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
            // Create a scene
            Scene scene;
            scene.SetName("SampleScene");
            scene.SetSceneSource("path/to/scene.json");

            // Initialize the scene
            scene.Init();

            // Add a node
            Node* newNode = new Node();
            scene.AddNode(newNode);

            // Update and render the scene
            scene.Update();
            scene.Render();

            // Clean up the scene
            scene.Shutdown();
        </code></pre>
        </div>
  `,
    node: `
        <div class="class">
        <h2>Node</h2>
        <p>The <code>Node</code> class is a core structure representing an entity in the scene graph. Nodes can have parent-child relationships, components, and manage their lifecycle within the scene.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">AddChild</span>(Node* node)
            </summary>
            <p><strong>Description:</strong> Adds a child node to the current node.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>node</code>: Pointer to the child node to be added.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">RemoveChild</span>(Node* node)
            </summary>
            <p><strong>Description:</strong> Removes a child node from the current node.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>node</code>: Pointer to the child node to be removed.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetName</span>(std::string name)
            </summary>
            <p><strong>Description:</strong> Sets the name of the current node.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>name</code>: The name to assign to the node.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Destroy</span>()
            </summary>
            <p><strong>Description:</strong> Marks the node for deletion, allowing it to be safely removed from memory during the next update cycle.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">template &lt;typename DataType&gt; DataType*</span> 
                <span class="method-name">AddComponent&lt;/span>()
            </summary>
            <p><strong>Description:</strong> Adds a component of the specified type to the node.</p>
            <p><strong>Template Parameters:</strong></p>
            <ul>
                <li><code>DataType</code>: A class derived from <code>Component</code>.</li>
            </ul>
            <p><strong>Returns:</strong> A pointer to the newly created component.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">template &lt;typename DataType&gt; DataType*</span> 
                <span class="method-name">GetComponent</span>()
            </summary>
            <p><strong>Description:</strong> Retrieves a component of the specified type from the node.</p>
            <p><strong>Template Parameters:</strong></p>
            <ul>
                <li><code>DataType</code>: A class derived from <code>Component</code>.</li>
            </ul>
            <p><strong>Returns:</strong> A pointer to the component if it exists, otherwise <code>nullptr</code>.</p>
            </details>
        </div>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
            // Create a node
            Node* rootNode = new Node();
            rootNode-&gt;SetName("RootNode");

            // Add a child node
            Node* childNode = new Node();
            rootNode-&gt;AddChild(childNode);
            childNode-&gt;SetName("ChildNode");

            // Add a TransformComponent to the child node
            TransformComponent* transform = childNode-&gt;AddComponent&lt;TransformComponent&gt;();
            transform-&gt;SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));

            // Update and render
            rootNode-&gt;Update();
            rootNode-&gt;Render();

            // Mark the child node for deletion
            childNode-&gt;Destroy();

            // Clean up
            delete rootNode;
        </code></pre>
        </div>
  `,
    gameObject: `
        <div class="class">
        <h2>GameObject</h2>
        <p>The <code>GameObject</code> class extends the <code>Node</code> class and represents an entity in the game. Each <code>GameObject</code> can have various components, update its state, render itself, and manage its lifecycle.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Init</span>()
            </summary>
            <p><strong>Description:</strong> Initializes the game object and its components.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Update</span>()
            </summary>
            <p><strong>Description:</strong> Updates the game object's state by invoking the <code>Update</code> method of its components.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Render</span>()
            </summary>
            <p><strong>Description:</strong> Renders the game object using its <code>RenderComponent</code>. Updates the projection and view matrices using the current camera.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Render</span>(Shader* shader)
            </summary>
            <p><strong>Description:</strong> Renders the game object using a specified shader and its <code>RenderComponent</code>.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>shader</code>: Pointer to the shader to be used for rendering.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetDead</span>(bool isDead)
            </summary>
            <p><strong>Description:</strong> Marks the game object as dead or alive.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>isDead</code>: Boolean flag indicating whether the object is dead (<code>true</code>) or alive (<code>false</code>).</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">bool</span> 
                <span class="method-name">IsDead</span>() <code>const</code>
            </summary>
            <p><strong>Description:</strong> Checks if the game object is marked as dead.</p>
            <p><strong>Returns:</strong> <code>true</code> if the object is dead, otherwise <code>false</code>.</p>
            </details>
        </div>

        <h3>Private Members</h3>
        <ul>
            <li><code>bool m_isAlive</code>: Indicates whether the game object is alive (<code>true</code>) or dead (<code>false</code>).</li>
        </ul>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
            // Create a new GameObject
            GameObject* gameObject = new GameObject();

            // Initialize the object
            gameObject->Init();

            // Update and render in the game loop
            gameObject->Update();
            gameObject->Render();

            // Mark the object as dead
            gameObject->SetDead(true);

            // Check if the object is dead
            if (gameObject->IsDead())
            {
                // Cleanup
                delete gameObject;
            }
        </code></pre>
        </div>
    `,
    component: `
        <div class="class">
        <h2>Component</h2>
        <p>The <code>Component</code> class serves as a base class for all components that can be attached to <code>Node</code> objects, such as <code>GameObject</code>. It provides essential functionality for initialization, updating, and managing the ownership of components.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Init</span>() <code>= 0</code>
            </summary>
            <p><strong>Description:</strong> Initializes the component. Must be implemented by derived classes.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Update</span>() <code>= 0</code>
            </summary>
            <p><strong>Description:</strong> Updates the component's state. Must be implemented by derived classes.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Shutdown</span>() <code>= 0</code>
            </summary>
            <p><strong>Description:</strong> Cleans up and shuts down the component. Must be implemented by derived classes.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetOwner</span>(Node* owner)
            </summary>
            <p><strong>Description:</strong> Assigns a <code>Node</code> as the owner of the component.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>owner</code>: Pointer to the <code>Node</code> that owns this component.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">Node*</span> 
                <span class="method-name">GetOwner</span>()
            </summary>
            <p><strong>Description:</strong> Retrieves the <code>Node</code> that owns this component.</p>
            <p><strong>Returns:</strong> Pointer to the owner <code>Node</code>.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">GameObject*</span> 
                <span class="method-name">GetOwnerAsGameObject</span>()
            </summary>
            <p><strong>Description:</strong> Retrieves the owner of this component cast as a <code>GameObject</code>.</p>
            <p><strong>Returns:</strong> Pointer to the owner <code>GameObject</code>, or <code>nullptr</code> if the cast is invalid.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">template <typename DataType> DataType*</span> 
                <span class="method-name">GetComponent</span>()
            </summary>
            <p><strong>Description:</strong> Retrieves a component of type <code>DataType</code> from the owner.</p>
            <p><strong>Returns:</strong> Pointer to the component if found, otherwise <code>nullptr</code>.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">template <typename DataType> bool</span> 
                <span class="method-name">HasComponent</span>()
            </summary>
            <p><strong>Description:</strong> Checks if the owner has a component of type <code>DataType</code>.</p>
            <p><strong>Returns:</strong> <code>true</code> if the component exists, otherwise <code>false</code>.</p>
            </details>
        </div>

        <h3>Protected Members</h3>
        <ul>
            <li><code>Node* pOwner</code>: Pointer to the <code>Node</code> that owns this component.</li>
        </ul>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
            class RenderComponent : public Component
            {
            public:
                void Init() override { /* Initialization code */ }
                void Update() override { /* Update code */ }
                void Shutdown() override { /* Shutdown code */ }
            };

            // Create a new RenderComponent and assign an owner
            RenderComponent* renderComponent = new RenderComponent();
            Node* node = new GameObject();
            renderComponent->SetOwner(node);

            // Update the component
            renderComponent->Update();

            // Clean up
            renderComponent->Shutdown();
            delete renderComponent;
            delete node;
        </code></pre>
        </div>
    `,
    renderComponent: `
        <div class="class">
        <h2>RenderComponent</h2>
        <p>The <code>RenderComponent</code> class handles the rendering process for <code>GameObject</code>s by managing materials, geometry, and shaders. It enables developers to set up and customize how objects appear in the game world.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Init</span>() <code>override</code>
            </summary>
            <p><strong>Description:</strong> Initializes the component with default material and geometry.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Update</span>() <code>override</code>
            </summary>
            <p><strong>Description:</strong> Updates the component state. Currently, this method is a placeholder.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Shutdown</span>() <code>override</code>
            </summary>
            <p><strong>Description:</strong> Cleans up and unbinds any allocated resources.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Render</span>()
            </summary>
            <p><strong>Description:</strong> Renders the object using the assigned geometry and material.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Render</span>(Shader* shader)
            </summary>
            <p><strong>Description:</strong> Renders the object with a custom shader.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetColor</span>(glm::vec3 color)
            </summary>
            <p><strong>Description:</strong> Sets the color of the material.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>color</code>: The RGB color values in the range [0, 1].</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetMaterial</span>(Material* pMaterial)
            </summary>
            <p><strong>Description:</strong> Sets the material for the component.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>pMaterial</code>: Pointer to the material to be assigned.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetGeometry</span>(Geometry* pGeometry)
            </summary>
            <p><strong>Description:</strong> Sets the geometry for the component.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>pGeometry</code>: Pointer to the geometry to be assigned.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetUVType</span>(UV_TYPE type)
            </summary>
            <p><strong>Description:</strong> Sets the UV mapping type for the geometry.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>type</code>: The UV mapping type (e.g., Planar, Cylindrical, etc.).</li>
            </ul>
            </details>
        </div>

        <h3>Private Members</h3>
        <ul>
            <li><code>Geometry* m_pGeometry</code>: The geometry of the object.</li>
            <li><code>Material* m_pMaterial</code>: The material used for rendering.</li>
            <li><code>Model* m_pModel</code>: Reserved for future model integration.</li>
        </ul>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
        GameObject* obj = new GameObject();
        RenderComponent* renderComp = obj->AddComponent&lt;RenderComponent&gt;();
        renderComp->SetMaterial("MyMaterial");
        renderComp->SetGeometry("MyGeometry");
        renderComp->SetColor(0.5f, 0.7f, 0.2f);
        obj->Update();
        obj->Render();
        </code></pre>
        </div>
    `,
    transformComponent: `
        <div class="class">
        <h2>TransformComponent</h2>
        <p>The <code>TransformComponent</code> manages the position, rotation, and scale of a <code>GameObject</code>. It provides a convenient interface for manipulating the transform of an object and synchronizing it with its parent <code>Transform</code>.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Init</span>() <code>override</code>
            </summary>
            <p><strong>Description:</strong> Initializes the component by linking it to the <code>Transform</code> of its owning <code>GameObject</code>.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Update</span>() <code>override</code>
            </summary>
            <p><strong>Description:</strong> Updates the component. Currently, this method is a placeholder.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Shutdown</span>() <code>override</code>
            </summary>
            <p><strong>Description:</strong> Cleans up the component's resources. Currently, this method is a placeholder.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetPosition</span>(glm::vec3 pos)
            </summary>
            <p><strong>Description:</strong> Sets the position of the owning <code>GameObject</code>.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>pos</code>: The new position.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetRotation</span>(glm::vec3 angle)
            </summary>
            <p><strong>Description:</strong> Sets the rotation of the owning <code>GameObject</code>.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>angle</code>: The new rotation in degrees.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetScale</span>(glm::vec3 scale)
            </summary>
            <p><strong>Description:</strong> Sets the scale of the owning <code>GameObject</code>.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>scale</code>: The new scale vector.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetScale</span>(float scale)
            </summary>
            <p><strong>Description:</strong> Sets a uniform scale for the owning <code>GameObject</code>.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>scale</code>: The uniform scale value.</li>
            </ul>
            </details>
        </div>

        <h3>Private Members</h3>
        <ul>
            <li><code>Transform* m_pTransform</code>: Pointer to the <code>Transform</code> object associated with this component.</li>
        </ul>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
        GameObject* obj = new GameObject();
        TransformComponent* transformComp = obj->AddComponent&lt;TransformComponent&gt;();
        transformComp->SetPosition(glm::vec3(1.0f, 2.0f, 3.0f));
        transformComp->SetRotation(glm::vec3(45.0f, 0.0f, 0.0f));
        transformComp->SetScale(2.0f);
        obj->Update();
        </code></pre>
        </div>
    `,
    physicsComponent: `
        <div class="class">
        <h2>PhysicsComponent</h2>
        <p>The <code>PhysicsComponent</code> simulates physical properties like velocity, acceleration, gravity, and drag for a <code>GameObject</code>. It integrates with the collision and physics systems to enable realistic object behaviors such as grounding, force application, and bouncing.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Init</span>() <code>override</code>
            </summary>
            <p><strong>Description:</strong> Initializes the component. Currently, this method is a placeholder.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Update</span>() <code>override</code>
            </summary>
            <p><strong>Description:</strong> Updates the component based on the elapsed time and applies forces, velocity, and gravity to the <code>GameObject</code>.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">ApplyForce</span>(glm::dvec3 force)
            </summary>
            <p><strong>Description:</strong> Applies a force to the object, affecting its acceleration.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>force</code>: The force vector to apply.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">ApplyTorque</span>(glm::dvec3 torque)
            </summary>
            <p><strong>Description:</strong> Applies a torque to the object, affecting its rotational acceleration.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>torque</code>: The torque vector to apply.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">std::optional&lt;std::pair&lt;CollisionComponent, CollisionComponent&gt;&gt;</span> 
                <span class="method-name">CheckForGround</span>()
            </summary>
            <p><strong>Description:</strong> Checks whether the object is grounded by detecting collisions below it.</p>
            <p><strong>Returns:</strong> An optional pair of <code>CollisionComponent</code> objects representing the ground collision, if detected.</p>
            </details>
        </div>

        <h3>Private Members</h3>
        <ul>
            <li><code>glm::dvec3 m_velocity</code>: The linear velocity of the object.</li>
            <li><code>glm::dvec3 m_acceleration</code>: The linear acceleration of the object.</li>
            <li><code>double m_mass</code>: The mass of the object.</li>
            <li><code>double m_gravityMultiplier</code>: Multiplier for gravity affecting the object.</li>
            <li><code>bool m_grounded</code>: Indicates if the object is grounded.</li>
        </ul>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
        GameObject* obj = new GameObject();
        PhysicsComponent* physicsComp = obj->AddComponent&lt;PhysicsComponent&gt;();
        physicsComp->ApplyForce(glm::dvec3(10.0, 0.0, 0.0));
        obj->Update();
        </code></pre>
        </div>
    `,
    collisionComponent: `
        <div class="class">
        <h2>CollisionComponent</h2>
        <p>The <code>CollisionComponent</code> manages collision detection for a <code>GameObject</code>. It defines collision layers and masks for filtering collisions and provides methods for collision detection and shape casting.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Init</span>() <code>override</code>
            </summary>
            <p><strong>Description:</strong> Initializes the collision component. Currently, this method is a placeholder.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Update</span>() <code>override</code>
            </summary>
            <p><strong>Description:</strong> Updates the collision component's shape position, rotation, and scale based on the owning <code>GameObject</code>'s transform.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Shutdown</span>() <code>override</code>
            </summary>
            <p><strong>Description:</strong> Cleans up the collision component, releasing its collision shape.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">bool</span> 
                <span class="method-name">CanCollideWith</span>(const CollisionComponent* other) const
            </summary>
            <p><strong>Description:</strong> Determines if this object can collide with another object based on their collision layers and masks.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>other</code>: The other collision component to check against.</li>
            </ul>
            <p><strong>Returns:</strong> <code>true</code> if the objects can collide; otherwise, <code>false</code>.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">glm::dvec3</span> 
                <span class="method-name">Cast_LastAvailablePosition</span>(glm::dvec3 startPosition, glm::dvec3 endPosition, glm::dvec3 startRotation, glm::dvec3 endRotation, int iterations)
            </summary>
            <p><strong>Description:</strong> Casts the collision shape from the start position to the end position, stopping at the last non-colliding position.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>startPosition</code>: The starting position of the cast.</li>
                <li><code>endPosition</code>: The target position of the cast.</li>
                <li><code>startRotation</code>: The starting rotation of the shape.</li>
                <li><code>endRotation</code>: The target rotation of the shape.</li>
                <li><code>iterations</code>: The number of steps in the cast.</li>
            </ul>
            <p><strong>Returns:</strong> The last available position before a collision.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">glm::dvec3</span> 
                <span class="method-name">Cast_FirstAvailablePosition</span>(glm::dvec3 startPosition, glm::dvec3 endPosition, int iterations)
            </summary>
            <p><strong>Description:</strong> Casts the collision shape from the start position to the end position, stopping at the first non-colliding position.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>startPosition</code>: The starting position of the cast.</li>
                <li><code>endPosition</code>: The target position of the cast.</li>
                <li><code>iterations</code>: The number of steps in the cast.</li>
            </ul>
            <p><strong>Returns:</strong> The first available position where no collision occurs.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">std::pair&lt;CollisionComponent, CollisionComponent*&gt;</span> 
                <span class="method-name">Cast_FirstCollision</span>(glm::dvec3 startPosition, glm::dvec3 endPosition, glm::dvec3 startRotation, glm::dvec3 endRotation, int iterations)
            </summary>
            <p><strong>Description:</strong> Casts the collision shape between two points and detects the first collision, if any.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>startPosition</code>: The starting position of the cast.</li>
                <li><code>endPosition</code>: The target position of the cast.</li>
                <li><code>startRotation</code>: The starting rotation of the shape.</li>
                <li><code>endRotation</code>: The target rotation of the shape.</li>
                <li><code>iterations</code>: The number of steps in the cast.</li>
            </ul>
            <p><strong>Returns:</strong> A pair containing the cast collision component and the collided component, or <code>nullptr</code> if no collision occurs.</p>
            </details>
        </div>

        <h3>Private Members</h3>
        <ul>
            <li><code>CollisionShape* m_collisionShape</code>: Pointer to the collision shape of this component.</li>
            <li><code>int m_collisionLayer</code>: The layer this component belongs to.</li>
            <li><code>int m_collisionMask</code>: The mask specifying which layers this component can collide with.</li>
        </ul>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
        GameObject* obj = new GameObject();
        CollisionComponent* collisionComp = obj->AddComponent&lt;CollisionComponent&gt;();
        collisionComp->SetCollisionLayer(1);
        collisionComp->SetCollisionMask(2);
        collisionComp->SetCollisionShape(new CollisionShape_Sphere());
        obj->Update();
        </code></pre>
        </div>
    `,
    controllerComponent: `
        <div class="class">
        <h2>ControllerComponent</h2>
        <p>The <code>ControllerComponent</code> manages user input to control the movement and behavior of a <code>GameObject</code>. It supports keyboard and gamepad input for navigation and interaction.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Init</span>() <code>override</code>
            </summary>
            <p><strong>Description:</strong> Initializes the component. Currently, this method is a placeholder.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Update</span>() <code>override</code>
            </summary>
            <p><strong>Description:</strong> Processes user input to update the position and state of the owning <code>GameObject</code>. Supports both keyboard and gamepad input.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Shutdown</span>() <code>override</code>
            </summary>
            <p><strong>Description:</strong> Cleans up the component's resources. Currently, this method is a placeholder.</p>
            </details>
        </div>

        <h3>Private Members</h3>
        <ul>
            <li><code>const float m_speed</code>: The movement speed of the <code>GameObject</code>.</li>
            <li><code>const Input* m_InputHandler</code>: Pointer to the input handler service for managing input events.</li>
        </ul>

        <h3>Details</h3>
        <p>The <code>Update</code> method handles input from:</p>
        <ul>
            <li><strong>Keyboard:</strong> <code>W</code>, <code>A</code>, <code>S</code>, <code>D</code>, and <code>Space</code> keys for movement and jumping.</li>
            <li><strong>Gamepad:</strong> Supports directional movement via D-pad and camera rotation via the right stick. Includes support for an inverted Y-axis toggle.</li>
        </ul>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
        GameObject* player = new GameObject();
        ControllerComponent* controller = player->AddComponent&lt;ControllerComponent&gt;();
        player->Update();  // Processes input and updates position
        </code></pre>
        </div>
    `,
    scriptComponent: `
        <div class="class">
        <h2>ScriptComponent</h2>
        <p>The <code>ScriptComponent</code> allows scripting functionality using Lua. It integrates Lua scripts into the engine, enabling custom behaviors and interactions for <code>GameObject</code>s.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span>
                <span class="method-name">Init</span>() <code>override</code>
            </summary>
            <p><strong>Description:</strong> Initializes the component by loading and running the associated Lua script. Also exposes engine classes and functions to Lua.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span>
                <span class="method-name">Update</span>(double deltaTime)
            </summary>
            <p><strong>Description:</strong> Updates the script by invoking the <code>Update</code> function defined in Lua, passing the delta time.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>deltaTime</code>: The time elapsed since the last update.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span>
                <span class="method-name">Update</span>() <code>override</code>
            </summary>
            <p><strong>Description:</strong> Updates the script with a default delta time of <code>1/60.0</code> seconds.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span>
                <span class="method-name">Shutdown</span>() <code>override</code>
            </summary>
            <p><strong>Description:</strong> Runs the <code>Shutdown</code> function in Lua and cleans up resources.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">bool</span>
                <span class="method-name">LoadScript</span>(std::string filepath)
            </summary>
            <p><strong>Description:</strong> Loads a Lua script from the specified file path.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>filepath</code>: Path to the Lua script file.</li>
            </ul>
            <p><strong>Returns:</strong> <code>true</code> if the script is loaded successfully.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span>
                <span class="method-name">ExposeClasses</span>()
            </summary>
            <p><strong>Description:</strong> Exposes engine classes and methods to Lua for scripting purposes.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span>
                <span class="method-name">TestFunc</span>()
            </summary>
            <p><strong>Description:</strong> A test function to validate script integration. Prints a message to the console.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">TransformComponent*</span>
                <span class="method-name">GetTransformComponent</span>(Node* parent)
            </summary>
            <p><strong>Description:</strong> Retrieves the <code>TransformComponent</code> of the given parent <code>Node</code>.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>parent</code>: The parent <code>Node</code>.</li>
            </ul>
            <p><strong>Returns:</strong> A pointer to the <code>TransformComponent</code>.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">RenderComponent*</span>
                <span class="method-name">GetRenderComponent</span>(Node* parent)
            </summary>
            <p><strong>Description:</strong> Retrieves the <code>RenderComponent</code> of the given parent <code>Node</code>.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>parent</code>: The parent <code>Node</code>.</li>
            </ul>
            <p><strong>Returns:</strong> A pointer to the <code>RenderComponent</code>.</p>
            </details>
        </div>

        <h3>Private Members</h3>
        <ul>
            <li><code>sol::state m_lua</code>: Lua state for managing scripts and their execution.</li>
            <li><code>std::string m_scriptFilepath</code>: Path to the Lua script file associated with the component.</li>
        </ul>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
        GameObject* obj = new GameObject();
        ScriptComponent* scriptComp = obj->AddComponent&lt;ScriptComponent&gt;();
        scriptComp->LoadScript("path/to/script.lua");
        scriptComp->Init();
        scriptComp->Update(0.016); // Update with 16 ms delta time
        scriptComp->Shutdown();
        </code></pre>
        </div>
    `,
    game:`
        <div class="class">
        <h2>Game</h2>
        <p>The <code>Game</code> class provides a base structure for creating a game application. It defines the core lifecycle methods and properties required for initializing, updating, rendering, and shutting down the game.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type"></span> 
                <span class="method-name">Game</span>() <code>constructor</code>
            </summary>
            <p><strong>Description:</strong> Default constructor that initializes the game with default settings.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type"></span> 
                <span class="method-name">Game</span>(WINDOW_PROPS props) <code>constructor</code>
            </summary>
            <p><strong>Description:</strong> Constructor that initializes the game with specified window properties.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>props</code>: Window properties.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type"></span> 
                <span class="method-name">Game</span>(int width, int height, std::string title) <code>constructor</code>
            </summary>
            <p><strong>Description:</strong> Constructor that initializes the game with specified width, height, and title.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>width</code>: Width of the game window.</li>
                <li><code>height</code>: Height of the game window.</li>
                <li><code>title</code>: Title of the game window.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">virtual void</span> 
                <span class="method-name">Init</span>() = 0
            </summary>
            <p><strong>Description:</strong> Initializes the game. This method should be implemented in derived classes to set up resources, load assets, and prepare the game state.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">virtual void</span> 
                <span class="method-name">Update</span>() = 0
            </summary>
            <p><strong>Description:</strong> Updates the game logic. This method should be implemented in derived classes to process game logic, physics, and input handling.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">virtual void</span> 
                <span class="method-name">Render</span>() = 0
            </summary>
            <p><strong>Description:</strong> Renders the game. This method should be implemented in derived classes to draw the game objects and UI to the screen.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">virtual void</span> 
                <span class="method-name">PostUpdate</span>() = 0
            </summary>
            <p><strong>Description:</strong> Handles any post-update operations. This method should be implemented in derived classes to finalize frame updates, such as physics synchronization or event handling.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">virtual void</span> 
                <span class="method-name">Shutdown</span>() = 0
            </summary>
            <p><strong>Description:</strong> Shuts down the game. This method should be implemented in derived classes to release resources, save state, and clean up the game environment.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">SetRunning</span>(bool running)
            </summary>
            <p><strong>Description:</strong> Sets the running state of the game.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>running</code>: <code>true</code> to run the game; <code>false</code> to stop it.</li>
            </ul>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">bool</span> 
                <span class="method-name">IsRunning</span>() const
            </summary>
            <p><strong>Description:</strong> Returns whether the game is currently running.</p>
            <p><strong>Returns:</strong> <code>true</code> if the game is running; otherwise, <code>false</code>.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">const char*</span> 
                <span class="method-name">GetTitle</span>() const
            </summary>
            <p><strong>Description:</strong> Returns the title of the game.</p>
            <p><strong>Returns:</strong> The title of the game as a C-string.</p>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">WINDOW_PROPS</span> 
                <span class="method-name">GetWindowProps</span>() const
            </summary>
            <p><strong>Description:</strong> Returns the properties of the game window.</p>
            <p><strong>Returns:</strong> A <code>WINDOW_PROPS</code> structure containing window properties.</p>
            </details>
        </div>

        <h3>Protected Members</h3>
        <ul>
            <li><code>bool m_isRunning</code>: Indicates whether the game is currently running.</li>
            <li><code>std::string m_title</code>: The title of the game.</li>
            <li><code>WINDOW_PROPS m_windowProps</code>: Properties of the game window.</li>
        </ul>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
        class MyGame : public Game {
        public:
            MyGame() : Game(800, 600, "My Game Title") {}
            void Init() override {
                // Initialize resources
            }
            void Update() override {
                // Game logic
            }
            void Render() override {
                // Rendering code
            }
            void PostUpdate() override {
                // Post-update operations
            }
            void Shutdown() override {
                // Cleanup resources
            }
        };

        int main() {
            MyGame game;
            game.Init();
            while (game.IsRunning()) {
                game.Update();
                game.Render();
                game.PostUpdate();
            }
            game.Shutdown();
            return 0;
        }
        </code></pre>
        </div>
    ,`,
    utils:`
        <div class="class">
        <h2>Utils</h2>
        <p>The <code>Utils</code> class provides utility functions for various tasks, including extracting class names and handling OpenGL errors.</p>

        <h3>Static Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">static std::string</span> 
                <span class="method-name">GetClassName</span>&lt;typename DataType&gt;()
            </summary>
            <p><strong>Description:</strong> Extracts the class name from the data type.</p>
            <p><strong>Returns:</strong> A string containing the class name of the provided data type.</p>
            <p><strong>Template Parameters:</strong></p>
            <ul>
                <li><code>DataType</code>: The type whose class name is to be extracted.</li>
            </ul>
            <h4>Example Usage:</h4>
            <pre><code class="language-cpp">
            std::string className = Utils::GetClassName<MyClass>();
            std::cout << "Class Name: " << className << std::endl;
            </code></pre>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">static void</span> 
                <span class="method-name">GetGLError</span>()
            </summary>
            <p><strong>Description:</strong> Checks for OpenGL errors and logs them to the console.</p>
            <p><strong>Details:</strong> This method retrieves the most recent OpenGL error, translates it into a human-readable string, and prints it. If no error is present, it does nothing.</p>
            <h4>Example Usage:</h4>
            <pre><code class="language-cpp">
            Utils::GetGLError();
            </code></pre>
            </details>
        </div>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
        // Extract class name example
        std::string className = Utils::GetClassName&lt;MyClass&gt;();
        std::cout << "Class Name: " << className << std::endl;

        // OpenGL error check example
        Utils::GetGLError();
        </code></pre>
        </div>
    `,
    time:`
        <div class="class">
        <h2>Time</h2>
        <p>The <code>Time</code> class manages time-related operations such as delta time calculations and fixed update intervals. It is designed to track elapsed time and provide consistent updates for fixed-timestep simulations.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Init</span>(double fixedDT)
            </summary>
            <p><strong>Description:</strong> Initializes the time system with a fixed delta time.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>fixedDT</code>: The fixed timestep interval in seconds.</li>
            </ul>
            <h4>Example Usage:</h4>
            <pre><code class="language-cpp">
            Time* time = ServiceLocator::GetService&lt;Time&gt;();
            time->Init(1.0 / 60.0); // Set fixed timestep to 60 FPS
            </code></pre>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">void</span> 
                <span class="method-name">Update</span>()
            </summary>
            <p><strong>Description:</strong> Updates the time system by calculating the time elapsed since the last update.</p>
            <h4>Example Usage:</h4>
            <pre><code class="language-cpp">
            Time* time = ServiceLocator::GetService&lt;Time&gt;();
            time->Update();
            </code></pre>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">const double</span> 
                <span class="method-name">GetFixedDeltaTime</span>() <code>const</code>
            </summary>
            <p><strong>Description:</strong> Returns the fixed delta time set during initialization.</p>
            <p><strong>Returns:</strong> The fixed timestep interval in seconds.</p>
            <h4>Example Usage:</h4>
            <pre><code class="language-cpp">
            double fixedDT = time->GetFixedDeltaTime();
            std::cout << "Fixed Delta Time: " << fixedDT << " seconds" << std::endl;
            </code></pre>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">const double</span> 
                <span class="method-name">GetDeltaTime</span>() <code>const</code>
            </summary>
            <p><strong>Description:</strong> Returns the time elapsed since the last frame update.</p>
            <p><strong>Returns:</strong> The time difference in seconds.</p>
            <h4>Example Usage:</h4>
            <pre><code class="language-cpp">
            double deltaTime = time->GetDeltaTime();
            std::cout << "Delta Time: " << deltaTime << " seconds" << std::endl;
            </code></pre>
            </details>
        </div>

        <h3>Private Members</h3>
        <ul>
            <li><code>double m_fixedDeltaTime</code>: Fixed timestep interval for updates.</li>
            <li><code>double m_deltaTime</code>: Elapsed time since the last frame.</li>
            <li><code>std::chrono::time_point&lt;std::chrono::high_resolution_clock&gt; m_lastTime</code>: Timestamp of the last frame.</li>
        </ul>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
        Time* time = ServiceLocator::GetService&lt;Time&gt;();
        time->Init(1.0 / 60.0); // Initialize with 60 FPS fixed timestep

        while (game->IsRunning())
        {
            time->Update(); // Update time each frame
            double deltaTime = time->GetDeltaTime();
            std::cout << "Frame time: " << deltaTime << " seconds" << std::endl;
        }
        </code></pre>
        </div>
    `,
    vectorCalculations:`
        <div class="class">
        <h2>VectorCalculation</h2>
        <p>The <code>VectorCalculation</code> class provides utility functions to compute directional vectors (forward, right, up) based on an object's rotation. These vectors are commonly used for object movement, camera controls, and orientation in 3D space.</p>

        <h3>Public Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">static glm::vec3</span> 
                <span class="method-name">GetForwardVec</span>(glm::vec3 rotation)
            </summary>
            <p><strong>Description:</strong> Calculates the forward vector for a given rotation.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>rotation</code>: A <code>glm::vec3</code> representing the rotation in degrees (pitch, yaw, roll).</li>
            </ul>
            <p><strong>Returns:</strong> A normalized forward vector.</p>
            <h4>Example Usage:</h4>
            <pre><code class="language-cpp">
            glm::vec3 rotation = glm::vec3(30.0f, 45.0f, 0.0f);
            glm::vec3 forward = VectorCalculation::GetForwardVec(rotation);
            </code></pre>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">static glm::vec3</span> 
                <span class="method-name">GetRightVec</span>(glm::vec3 rotation)
            </summary>
            <p><strong>Description:</strong> Calculates the right vector for a given rotation.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>rotation</code>: A <code>glm::vec3</code> representing the rotation in degrees (pitch, yaw, roll).</li>
            </ul>
            <p><strong>Returns:</strong> A normalized right vector.</p>
            <h4>Example Usage:</h4>
            <pre><code class="language-cpp">
            glm::vec3 rotation = glm::vec3(30.0f, 45.0f, 0.0f);
            glm::vec3 right = VectorCalculation::GetRightVec(rotation);
            </code></pre>
            </details>
        </div>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">static glm::vec3</span> 
                <span class="method-name">GetUpVec</span>(glm::vec3 rotation)
            </summary>
            <p><strong>Description:</strong> Calculates the up vector for a given rotation.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>rotation</code>: A <code>glm::vec3</code> representing the rotation in degrees (pitch, yaw, roll).</li>
            </ul>
            <p><strong>Returns:</strong> A normalized up vector.</p>
            <h4>Example Usage:</h4>
            <pre><code class="language-cpp">
            glm::vec3 rotation = glm::vec3(30.0f, 45.0f, 0.0f);
            glm::vec3 up = VectorCalculation::GetUpVec(rotation);
            </code></pre>
            </details>
        </div>

        <h3>Private Methods</h3>

        <div class="method">
            <details>
            <summary>
                <span class="return-type">static glm::mat3</span> 
                <span class="method-name">calculateRotationMatrix</span>(glm::vec3 rotation)
            </summary>
            <p><strong>Description:</strong> Computes the rotation matrix based on pitch, yaw, and roll.</p>
            <p><strong>Parameters:</strong></p>
            <ul>
                <li><code>rotation</code>: A <code>glm::vec3</code> representing the rotation in degrees (pitch, yaw, roll).</li>
            </ul>
            <p><strong>Returns:</strong> A <code>glm::mat3</code> representing the rotation matrix.</p>
            </details>
        </div>

        <h3>Example Usage</h3>
        <pre><code class="language-cpp">
        glm::vec3 rotation = glm::vec3(30.0f, 45.0f, 0.0f);

        glm::vec3 forward = VectorCalculation::GetForwardVec(rotation);
        glm::vec3 right = VectorCalculation::GetRightVec(rotation);
        glm::vec3 up = VectorCalculation::GetUpVec(rotation);

        std::cout << "Forward: " << forward.x << ", " << forward.y << ", " << forward.z << std::endl;
        std::cout << "Right: " << right.x << ", " << right.y << ", " << right.z << std::endl;
        std::cout << "Up: " << up.x << ", " << up.y << ", " << up.z << std::endl;
        </code></pre>
        </div>
    `,
    definecomponents: `
        <h3>Defining New Components to the Engine</h3>
        <p>Components are the building blocks of game objects in the engine. Each component encapsulates specific functionality or behavior that can be attached to game objects. Here are some steps for defining new components:</p>

        <h4>Create a New Component Class</h4>
        <pre class="language-cpp"><code>

        class CustomType : public IHasGettersSetters
        {
        public:
            //base class for custom type
        protected:
            void defineMember() override {}
        };

        class MyAwsomeType : public CustomType
        {
        public:
            MyAwsomeType() { defineMember(); }

            void SetSomeValue(float value) { m_someValue = value; }

            float GetSomeValue() const { return m_someValue; }

        private:
            float m_someValue;

            void defineMember() override {
                m_setters["someValue"] = [this](std::any value) { SetSomeValue(std::any_cast&lt;float&gt;(value)); };
                m_getters["someValue"] = [this]() -> std::any { return GetSomeValue(); };
            }
        };

        class MyNewComponent : public Component
        {
        public:
            MyNewComponent() { defineMember(); }

            void Init() override;
            void Update() override;
            void Shutdown() override;

            void SetMemberA(float value) { m_memberA = value; }
            void SetMemberB(int value) { m_memberB = value; }
            void SetCustomType(CustomType* value) { m_customType = value; }

            float GetMemberA() const { return m_memberA; }
            int GetMemberB() const { return m_memberB; }
            CustomType* GetCustomType() const { return m_customType; }

        private:
            float m_memberA;
            int m_memberB;
            CustomType* m_customType;

            void defineMember() override {
                m_setters["memberA"] = [this](std::any value) { SetMemberA(std::any_cast&lt;float&gt;(value)); };
                m_setters["memberB"] = [this](std::any value) { SetMemberB(std::any_cast&lt;int&gt;(value)); };
                m_setters["customType"] = [this](std::any value) { SetCustomType(std::any_cast&lt;CustomType*&gt;(value)); };

                m_getters["memberA"] = [this]() -> std::any { return GetMemberA(); };
                m_getters["memberB"] = [this]() -> std::any { return GetMemberB(); };
                m_getters["customType"] = [this]() -> std::any { return GetCustomType(); };
            }
        };
        </code></pre>

        <h4>(Optional) Add new parameter datatype</h4>
        <div style="display: flex; gap: 15px;">
            <img src="img/component-3.png" alt="Component Example 3" style="width: 780; height: 378;">
        </div>
        <p style="color: #888; font-style: italic;">Add new datatype to load from the file</p>

        <pre class="language-c"><code>
            // SceneManager.cpp in handleGetters function
            else if constexpr (std::is_same_v&lt;T, CustomType*&gt;)
            {
				rapidjson::Value subVariable(rapidjson::kObjectType);
				auto obj = std::any_cast<T>(value);
				auto subGetters = obj->GetGetters();
				handleGetters(subGetters, subVariable, allocator);
				variable.PushBack(subVariable, allocator);
            }
        </code></pre>
        `,
    importingcomponents: `
      <h3>Importing Components to the Engine</h3>
      <p>Components can be created by defining constants in the engine. Depending on the requirements of your component, 
         you may need to manually compose the input settings. Here are some steps for new components:</p>
      <h4>GameObjectSystemComponentConstants.h</h4>
      <div style="display: flex; gap: 15px;">
        <img src="img/component-1.png" alt="Component Example 1" style="width: 840px; height: 220px;">
      </div>
      <p style="color: #888; font-style: italic;">Examples of defined component constants</p>

      <h4>GameObjectFactory.cpp in deserialize function</h4>
      <div style="display: flex; gap: 15px;">
        <img src="img/component-2.png" alt="Component Example 2" style="width: 780; height: 378;">
      </div>
      <p style="color: #888; font-style: italic;">Examples of loading component from file</p>
     
    <pre class="language-c"><code>
        // GameObjectFactory.cpp in deserialize function
        else if constexpr (std::is_same_v&lt;T, CustomType*&gt;)
        {
            // need to parse the custom type if parameter is also a custom type
            auto customType = parseCustomType(member.value[1].GetObject());
            setters.at(variableName)(static_cast<CustomType*>(customType));
        }
    </code></pre>
    `,
    addinggame: `
      <h3>Adding a New Game</h3>
      <p>Adding a new game to the engine involves creating a class that inherits from the <code>Game</code> base class.
        The game revolves around the core lifecycle methods of initialization, updating, rendering, post-update, and shutdown.
        Here is an example of a simple game class:</p>

      <pre class="language-c"><code>
        class sample : public Game
        {
        public:
            //@brief Constructor
            sample() : Game() {}

            //@brief Constructor
            sample(WINDOW_PROPS props) : Game(props) {}

            //@brief Constructor
            sample(int width, int height, std::string title) :Game(width, height, title) {}

            void Init() override;

            void Update() override;

            void PostUpdate() override;

            void Render() override;

            void Shutdown() override;
        private:
        };

        void sample::Init()
        {
            ServiceLocator::GetService&lt;ResourceFactory&gt;()->CreateAllResources("path/to/resource.json");
            ServiceLocator::GetService&lt;SceneManager&gt;()->AddScene("myScene1");
            ServiceLocator::GetService&lt;SceneManager&gt;()->AddScene("myScene1");


            Camera* cam = Camera::GetInstance();
            cam->rot.z = 6.29f;
            cam->rot.x = 33.48f;
            //TestCamera::GetInstance()->Init();

            DeserializeJSON::LoadImGui("../../content/code/json_files/UI.json");

            // Setting the starting scene
            ServiceLocator::GetService&lt;SceneManager&gt;()->SetCurrentScene("myScene1");
            
            std::cout << "sample::Init()" << std::endl;
        }

        void sample::Update()
        {
            Camera::GetInstance()->Update();
            ServiceLocator::GetService&lt;SceneManager&gt;()->GetCurrentScene()->Update();
        }

        void sample::PostUpdate()
        {
            ServiceLocator::GetService&lt;SceneManager&gt;()->GetCurrentScene()->PostUpdate();
        }

        void sample::Render()
        {
            ServiceLocator::GetService&lt;SceneManager&gt;()->GetCurrentScene()->Render();
        }

        void sample::Shutdown()
        {
            ServiceLocator::GetService&lt;SceneManager&gt;()->GetCurrentScene()->Shutdown();
            std::cout << "sample::Shutdown()" << std::endl;
        }
      </code></pre>
    `,
};

// Function to display content dynamically
function showContent(sectionId) {
    const content = document.getElementById("content");

    // Clear current content
    content.innerHTML = "";

    // Load the selected content
    content.innerHTML = sections[sectionId] || `
      <h3>Section Not Found</h3>
      <p>The selected section does not exist.</p>
    `;

    // Highlight code blocks using Prism
    Prism.highlightAll();
}
