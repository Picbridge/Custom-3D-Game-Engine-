#pragma once

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float FAST_SPEED = 5.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	FAST
};

class TestCamera : public Node
{
public:
	static TestCamera* GetInstance();

	float nearPlane;
	float farPlane;
	// Constructor with vectors
	TestCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	// Constructor with scalar values
	TestCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(m_pPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	}

	void Init();
	void Update();

	float GetZoom() { return m_zoom; }
private:
	static TestCamera* instance;

	// Camera Attributes (no longer static)
	glm::vec3 m_pPosition;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	float m_yaw;
	float m_pitch;
	float m_speed;
	float m_sensitivity;
	float m_zoom;
	float m_deltaTime;
	float m_lastFrame;


	// First mouse control
	bool firstMouse;
	float lastX, lastY;

	// glfw: whenever the mouse moves, this callback is called
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	void processInput(GLFWwindow* window);

	// Processes input received from any keyboard-like input system
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	// Processes input received from a mouse input system
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event
	void ProcessMouseScroll(float yoffset);

	// Calculates the front vector from the Camera's Euler Angles
	void updateCameraVectors();
};
