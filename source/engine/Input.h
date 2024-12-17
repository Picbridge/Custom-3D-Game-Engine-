// Copyright 2024 DigiPen (USA) Corporation
// File Author(s): Marco Aguilar Gallardo (Keyboard + Mouse), Greyson Biggs (Gamepad), 
// Cameron Allen (Callback adjustments, refactor/cleanup)

// This class abstracts away certain GLFW input functions, and provides needed support for gamepad input. The following are the GLFW gamepad functions you should know that are NOT covered by this class.
// https://www.glfw.org/docs/3.3/input_guide.html#joystick
// glfwJoystickPresent(GLFW_JOYSTICK_1); // Tests if joystick/gamepad #1 is present (the maximum is GLFW_JOYSTICK_16).
// GLFWgamepadstate state; // Declares a struct which has two members - buttons[15], and axes[6]. Access with GLFW_GAMEPAD_BUTTON_... and GLFW_GAMEPAD_AXIS_... respectively. Buttons are either GLFW_PRESS or GLFW_RELEASE. Axes range from -1.0f to 1.0f inclusive.
// glfwGetGamepadState(int jid, GLFWgamepadstate &state); // Updates the state variable with the given gamepad's current state.

#pragma once

#define INPUT_MAX_GAMEPADS 16
#define INPUT_MAX_BUTTONS 128
#define INPUT_MAX_AXES 32

class Input 
{
public:
    static Input& GetInstance();
    void Init(GLFWwindow* window);
    void Update();
    void PostUpdate();

    // Keys
    bool IsKeyPressed(int key) const;
    bool IsKeyJustPressed(int key) const;

    // Mouse
    bool IsMouseButtonPressed(int button) const;
    bool IsMouseButtonJustPressed(int button) const;
    const double& GetMouseX() const;
    const double& GetMouseY() const;
    const glm::dvec2 GetMouseCoords() const;
    glm::vec2 GetMouseNormalizedCoords() const;

    // returns scalar representing how far the mouse
    // scroll wheel has moved
    const float& GetScrollZoom() const;
    
    // gets scroll difference between frames
    float GetScrollZoomDelta() const { return m_scrollDiff; };

    void SetScrollZoom(const float& _zoom);

    // Gamepads
    int GetGamepadCount() const;

    // States from last frame
    GLFWgamepadstate GetGamepadState(int jid) const;
    const bool& GetMouseButtonState(int jid) const;

    bool IsGamepadButtonJustPressed(int jid, int button) const;
    bool IsGamepadButtonPressed(int jid, int button) const;
    bool IsGamepadPresent(int jid) const;

private:
    Input();
    Input& operator=(const Input&) = delete;
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void MouseScrollCallback(GLFWwindow* window, double x, double y);
    static void JoystickCallback(int jid, int event);
    static float m_zoom;
    static float m_scrollDiff;

    // Keyboard + Mouse
    bool m_keys[1024];
    bool m_keysLastFrame[1024];
    bool m_mouseButtons[8];
    bool m_mouseButtonsLastFrame[8];
    glm::dvec2 m_mousePos;

    // Gamepad 
    int m_gamepadCount = 0;
    std::chrono::high_resolution_clock::time_point m_lastRegisteredAnalogInputTime;
    GLFWgamepadstate m_gamepadsCurrFrame[GLFW_JOYSTICK_LAST + 1] = {};
    GLFWgamepadstate m_gamepadsLastFrame[GLFW_JOYSTICK_LAST + 1] = {};

    // Window(s)
    GLFWwindow* mp_window = nullptr; // Set on Initialize(). Rework this if we end up needing multiple windows!
    friend class ServiceLocator;
};

