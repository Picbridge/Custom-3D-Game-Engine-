#include "pch.h"
#include "Input.h"
#include "ServiceLocator.h"

WindowHandler* WindowHandler::instance = nullptr;

WindowHandler::WindowHandler() : glfwSuccess(false), m_pWindow(nullptr), m_pMonitor(nullptr), shouldClose(false)
{
	Props.Height = 0;
	Props.Width = 0;
	Props.Title = "";

	FrameBuffer.Width = 0;
	FrameBuffer.Height = 0;
}

WindowHandler& WindowHandler::GetInstance()
{
	static WindowHandler instance;
	return instance;
}

void WindowHandler::SwapBuffers()
{
	glfwSwapBuffers(m_pWindow);
}

GLFWwindow* WindowHandler::GetCurrentContext()
{
	return glfwGetCurrentContext();
}

void WindowHandler::Init()
{
	// set default props if not initialized
	if (Props.Title.empty())
		Props.Title = "Untitled";

	if (Props.Height == 0)
		Props.Height = 480;

	if (Props.Width == 0)
		Props.Width = 640;

	// initialize glfw
	if (glfwSuccess)
	{
		std::cout << "GLFW is already initialized!";
		exit(EXIT_FAILURE);
	}

	glfwSetErrorCallback(this->GLFWErrorCallback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwSuccess = true;

	// Initialize OpenGL Context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);
	// Create window
	m_pWindow = glfwCreateWindow(Props.Width, Props.Height, Props.Title.c_str(), NULL, NULL);
	if (!m_pWindow)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(m_pWindow);

	// Initialize glew (or other gl functions loader)
	GLenum glewError = glewInit();
	if (GLEW_OK != glewError)
	{
		std::cout << "Terminating program. Reason:" << std::endl;
		std::cout << "Error in GLEW: " << glewGetErrorString(glewError) << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwSetWindowSizeCallback(m_pWindow, this->GLFWWindowSizeCallback);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDebugMessageCallback(MessageCallback, 0);

	std::cout << "Window Initialized" << std::endl;
}

void WindowHandler::Update()
{

	if (!m_pWindow)
	{
		std::cout << "Window has not been initialized yet. Closing program..." << std::endl;
		exit(EXIT_FAILURE);
	}

	// this should not be responsibility of Window. Maybe something like a BufferDraw
	glfwGetFramebufferSize(m_pWindow, &FrameBuffer.Width, &FrameBuffer.Height);
	glViewport(0, 0, FrameBuffer.Width, FrameBuffer.Height);

	glClear(GL_COLOR_BUFFER_BIT);

	glfwPollEvents();

	if (SERVICE_LOCATOR.GetInput()->GetInstance().IsKeyJustPressed(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(this->m_pWindow, GLFW_TRUE);

	if (glfwWindowShouldClose(m_pWindow))
		shouldClose = true;
}

void WindowHandler::Shutdown()
{
	if (!m_pWindow)
	{
		std::cout << "Window can't shutdown because it has not been initialized yet. Closing program..." << std::endl;
		exit(EXIT_FAILURE);
	}
	if (!glfwSuccess)
	{
		std::cout << "GLFW  can't shutdown because it has not been initialized yet. Closing program..." << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
	std::cout << "Window Shutdown" << std::endl;
}

bool WindowHandler::ShouldClose()
{
	return shouldClose;
}



void WindowHandler::GLFWErrorCallback(int error, const char* description)
{
	std::cout << "Error in GLFW: " << description << std::endl;
}

void WindowHandler::GLFWWindowSizeCallback(GLFWwindow* window, int width, int height)
{
	std::cout << "Window being resized... ";
	std::cout << "Width: " << width << " | Height: " << height << std::endl;
}

void GLAPIENTRY WindowHandler::MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::cout << "OpenGL Debug: " << message << std::endl;
}