#include "pch.h"
#include "TestCamera.h"

#include "ServiceLocator.h"

TestCamera* TestCamera::instance = nullptr; // Initialize static instance pointer

TestCamera* TestCamera::GetInstance()
{
	if (!instance) {
		instance = new TestCamera(glm::vec3(0.0f, 0.0f, 10.0f));
	}
	return instance;
}

TestCamera::TestCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: m_pPosition(position), m_worldUp(up), m_yaw(yaw), m_pitch(pitch), m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_speed(SPEED), m_sensitivity(SENSITIVITY), m_zoom(ZOOM), firstMouse(true), m_deltaTime(0.0f), m_lastFrame(0.0f), nearPlane(0.1f), farPlane(100.0f)
{
	updateCameraVectors();
}

TestCamera::TestCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: m_pPosition(glm::vec3(posX, posY, posZ)), m_worldUp(glm::vec3(upX, upY, upZ)), m_yaw(yaw), m_pitch(pitch),
	m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_speed(SPEED), m_sensitivity(SENSITIVITY), m_zoom(ZOOM), firstMouse(true), m_deltaTime(0.0f), m_lastFrame(0.0f), nearPlane(0.1f), farPlane(100.0f)
{
	lastX = SERVICE_LOCATOR.GetWindowHandler()->Props.Width / 2.0f;
	lastY = SERVICE_LOCATOR.GetWindowHandler()->Props.Height / 2.0f;
	updateCameraVectors();
}

void TestCamera::Init()
{
	GLFWwindow* window = SERVICE_LOCATOR.GetWindowHandler()->GetCurrentContext();
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

void TestCamera::Update()
{
	float currentFrame = (float)glfwGetTime();
	m_deltaTime = currentFrame - m_lastFrame;
	m_lastFrame = currentFrame;

	GLFWwindow* window = SERVICE_LOCATOR.GetWindowHandler()->GetCurrentContext();
	processInput(window);
}

void TestCamera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	TestCamera* camera = TestCamera::GetInstance();  // Access instance from the static method

	if (camera->firstMouse)
	{
		camera->lastX = (float)xpos;
		camera->lastY = (float)ypos;
		camera->firstMouse = false;
	}

	float xoffset = (float)xpos - camera->lastX;
	float yoffset = camera->lastY - (float)ypos;

	camera->lastX = (float)xpos;
	camera->lastY = (float)ypos;

	camera->ProcessMouseMovement(xoffset, yoffset);
}

void TestCamera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	TestCamera* camera = TestCamera::GetInstance();
	camera->ProcessMouseScroll((float)yoffset);
}

void TestCamera::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		ProcessKeyboard(FORWARD, m_deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		ProcessKeyboard(BACKWARD, m_deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		ProcessKeyboard(LEFT, m_deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		ProcessKeyboard(RIGHT, m_deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		ProcessKeyboard(UP, m_deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		ProcessKeyboard(DOWN, m_deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		ProcessKeyboard(FAST, m_deltaTime);
}

void TestCamera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = m_speed * deltaTime;
	if (direction == FAST)
		m_speed = FAST_SPEED;
	else
		m_speed = SPEED;

	if (direction == FORWARD)
		m_pPosition += m_front * velocity;
	if (direction == BACKWARD)
		m_pPosition -= m_front * velocity;
	if (direction == LEFT)
		m_pPosition -= m_right * velocity;
	if (direction == RIGHT)
		m_pPosition += m_right * velocity;
	if (direction == UP)
		m_pPosition += m_up * velocity;
	if (direction == DOWN)
		m_pPosition -= m_up * velocity;
}

void TestCamera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= m_sensitivity;
	yoffset *= m_sensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	if (constrainPitch)
	{
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;
	}

	updateCameraVectors();
}

void TestCamera::ProcessMouseScroll(float yoffset)
{
	if (m_zoom >= 1.0f && m_zoom <= 45.0f)
		m_zoom -= (float)yoffset;
	if (m_zoom <= 1.0f)
		m_zoom = 1.0f;
	if (m_zoom >= 45.0f)
		m_zoom = 45.0f;
}

void TestCamera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}
