// Copyright 2024 DigiPen (USA) Corporation
// File Author(s): Marco Aguilar Gallardo (Keyboard + Mouse), Greyson Biggs (Gamepad), 
// Cameron Allen (Callback adjustments, refactor/cleanup)

#include "pch.h"
#include "Input.h"

float Input::m_zoom = 0.0f;
float Input::m_scrollDiff = 0.0f;

// Input contructor
Input::Input() 
{
    for (int i = 0; i < 1024; i++) 
    {
        m_keys[i] = false;
        m_keysLastFrame[i] = false;
    }
    for (int i = 0; i < 8; i++) 
    {
        m_mouseButtons[i] = false;
        m_mouseButtonsLastFrame[i] = false;
    }
}

// Gets Input instance
Input& Input::GetInstance() 
{
    static Input instance;
    return instance;
}

// Initializes Input
void Input::Init(GLFWwindow* _window) 
{
    mp_window = _window;
    glfwSetKeyCallback(mp_window, this->KeyCallback);
    glfwSetCursorPosCallback(mp_window, this->MousePositionCallback);
    glfwSetMouseButtonCallback(mp_window, this->MouseButtonCallback);
    glfwSetScrollCallback(mp_window, this->MouseScrollCallback);
    glfwSetJoystickCallback(this->JoystickCallback);
    glfwSetInputMode(mp_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    m_lastRegisteredAnalogInputTime = std::chrono::high_resolution_clock::now();
}

// Updates Input states
void Input::Update()
{
    for (int i = 0; i <= GLFW_JOYSTICK_LAST; i++)
    {
        glfwGetGamepadState(GLFW_JOYSTICK_1 + i, &m_gamepadsCurrFrame[i]);
    }
}

// Updates last frame's Input states after the frame
void Input::PostUpdate()
{
    for (int i = 0; i < 1024; i++)
    {
        m_keysLastFrame[i] = m_keys[i];
    }
    for (int i = 0; i < 8; i++)
    {
        m_mouseButtonsLastFrame[i] = m_mouseButtons[i];
    }
    for (int i = 0; i <= GLFW_JOYSTICK_LAST; i++)
    {
        m_gamepadsLastFrame[i] = m_gamepadsCurrFrame[i];
    }
    m_scrollDiff = 0.0f;
}

// Returns true if a key is pressed
bool Input::IsKeyPressed(int key) const 
{
    return m_keys[key];
}

// Returns true if a key was just pressed
bool Input::IsKeyJustPressed(int key) const 
{
    return m_keys[key] && !m_keysLastFrame[key];
}

// Returns true if a mouse button is pressed
bool Input::IsMouseButtonPressed(int button) const 
{
    return m_mouseButtons[button];
}

// Returns true if a mouse button was just pressed
bool Input::IsMouseButtonJustPressed(int button) const 
{
    return m_mouseButtons[button] && !m_mouseButtonsLastFrame[button];
}

// Returns the mouse x position
const double& Input::GetMouseX() const
{
    return m_mousePos.x;
}

// Returns the mouse y position
const double& Input::GetMouseY() const 
{
    return m_mousePos.y;
}

// Returns the mouse position
const glm::dvec2 Input::GetMouseCoords() const 
{
    return m_mousePos;
}

// gets the mouse position in normalized device coordinates
glm::vec2 Input::GetMouseNormalizedCoords() const 
{
    int w, h;
    glfwGetWindowSize(mp_window, &w, &h);
    double x_ndc = 2.0f * (m_mousePos.x + 0.5f) / w - 1.0f;
    double y_ndc = 2.0f * (m_mousePos.y + 0.5f) / h - 1.0f;
    return glm::vec2(x_ndc, y_ndc);
}

// Returns the scroll zoom value
const float& Input::GetScrollZoom() const
{
    return m_zoom;
}

// Sets the scroll zoom value
void Input::SetScrollZoom(const float& _zoom)
{
    m_zoom = _zoom;
}

// Returns how many gamepads are connected
int Input::GetGamepadCount() const 
{
    return m_gamepadCount;
}

// GLFWgamepadstate is a struct with two arrays - buttons and axes.
GLFWgamepadstate Input::GetGamepadState(int jid) const 
{
    return m_gamepadsLastFrame[jid];
}

// Returns the state of a specific mouse button (true means it is currently pressed)
const bool& Input::GetMouseButtonState(int jid) const
{
    return m_mouseButtons[jid];
}

// Returns true if a specific gamepad button is pressed
bool Input::IsGamepadButtonPressed(int jid, int button) const
{
    if (!IsGamepadPresent(jid))
        return false;

    return (m_gamepadsCurrFrame[jid].buttons[button] == GLFW_PRESS);
}

// Returns true if a specific gamepad button was just pressed
bool Input::IsGamepadButtonJustPressed(int jid, int button) const
{
    if (!IsGamepadPresent(jid))
        return false;
    
    return ((m_gamepadsCurrFrame[jid].buttons[button] == GLFW_PRESS) && (m_gamepadsLastFrame[jid].buttons[button] == GLFW_RELEASE));
}

// Returns true if a gamepad with a certain id is present
bool Input::IsGamepadPresent(int jid) const
{
    return glfwJoystickPresent(jid);
}

// Key callback
void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    Input& input = GetInstance();
    if (action == GLFW_PRESS) 
    {
        input.m_keys[key] = true;
    }
    else if (action == GLFW_RELEASE) 
    {
        input.m_keys[key] = false;
    }
}

// Mouse position callback
void Input::MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    Input& input = GetInstance();
    input.m_mousePos.x = xpos;
    input.m_mousePos.y = ypos;
}

// Mouse button callback
void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
{
    Input& input = GetInstance();

    if (action == GLFW_PRESS) 
    {
        input.m_mouseButtons[button] = true;
    }
    else if (action == GLFW_RELEASE) 
    {
        input.m_mouseButtons[button] = false;
    }
}

// Mouse scroll callback
void Input::MouseScrollCallback(GLFWwindow* window, double x, double y) 
{
    float val = 0.8f;
    if (y > 0.0) {
        m_zoom -= val;
        m_scrollDiff = -val;
    }
    else if (y < 0.0) {
        m_zoom += val;
        m_scrollDiff = val;
    }
}

// GLFW only calls this when a joystick is dis/connected.
void Input::JoystickCallback(int jid, int event) 
{
    Input& input = GetInstance();
    if (event == GLFW_CONNECTED) ++input.m_gamepadCount;
    if (event == GLFW_DISCONNECTED) --input.m_gamepadCount;
}