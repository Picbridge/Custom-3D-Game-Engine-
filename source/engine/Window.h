#pragma once

struct GLFWwindow;
struct GLFWmonitor;

class WindowHandler
{
public:
	WINDOW_PROPS Props;
	FRAME_BUFFER_PROPS FrameBuffer;
	GLFWwindow* m_pWindow;

	WindowHandler();

	//@brief Initializes the window
	void Init();
	//@brief Updates the window
	void Update();
	//@brief Shuts down the window
	void Shutdown();
	//@brief Returns if the window should close
	bool ShouldClose();
	//@brief Swaps the buffers
	void SwapBuffers();
	//@brief Returns the current context
	//@return GLFWwindow* the current context
	GLFWwindow* GetCurrentContext();

private:
	bool glfwSuccess;
	bool shouldClose;

	// window pointers
	GLFWmonitor* m_pMonitor;

	static WindowHandler* instance;
	static WindowHandler& GetInstance();

	//@brief GLFW error handling
	static void GLFWErrorCallback(int error, const char* description);
	//@brief Gets called when the window is resized
	static void GLFWWindowSizeCallback(GLFWwindow* window, int width, int height);
	//@brief Gets called when a key is pressed
	static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

	friend class ServiceLocator;
};
