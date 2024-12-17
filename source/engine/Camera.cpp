// Created by Garrett Grossner (Initial integration and quaterion support) and
// Cameron Allen (Refactor/cleanup and Input compatibility)

#include "pch.h"
#include "Camera.h"
#include "Input.h"
#include "ui/UI.h"
#include "ServiceLocator.h"

std::unique_ptr<Camera> Camera::mp_instance = nullptr;

Camera* Camera::GetInstance()
{
	if (mp_instance == nullptr)
		mp_instance = std::unique_ptr<Camera>(new Camera());
	return mp_instance.get();
}

void Camera::Update()
{
    m_invert = SERVICE_LOCATOR.GetUI()->GetInstance().GetState("Invert Camera Y-Axis", IMGUI_ELEMENT_TYPE::BUTTON);
    
    if (!m_focussedOnUI)
    {
        m_zoom += mp_input->GetScrollZoomDelta();
        UpdateMouseKeyPress();
    }

	const float mouseX = static_cast<float>(mp_input->GetMouseX());
	const float mouseY = static_cast<float>(mp_input->GetMouseY());

    if (!m_focussedOnUI)
        UpdateMouseMovement(mouseX, mouseY);
    else 
    {
        m_prevMouseX = mouseX;
        m_prevMouseY = mouseY;
    }

    auto frameBuffer = SERVICE_LOCATOR.GetWindowHandler()->FrameBuffer;
    float now = static_cast<float>(glfwGetTime());
    float dist = (now - m_lastTime) * m_speed;
    m_lastTime = now;

    glm::quat pitchQuat = glm::angleAxis(m_rot.x - 90, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat yawQuat = glm::angleAxis(m_rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat rollQuat = glm::angleAxis(m_rot.z, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::quat rotQuat = pitchQuat * yawQuat * rollQuat;

    m_worldView = glm::translate(glm::mat4(1.0f), glm::vec3(m_tx, m_ty - 1, -m_zoom)) * glm::toMat4(rotQuat);

    m_worldProjection = glm::perspective(m_ry, (float)frameBuffer.Width / (float)frameBuffer.Height, m_front, m_back);
    //worldInverse = glm::inverse(worldView);
}

Camera::Camera() : mp_input(SERVICE_LOCATOR.GetInput()),
m_speed(300.0 / 60.0), m_lastTime(static_cast<float>(glfwGetTime())),
m_ry(0.4f), m_front(0.5f), m_back(1000.0f), m_tx(0.0f), m_ty(0.0f), m_zoom(25.0f),
m_focussedOnUI(SERVICE_LOCATOR.GetUI()->GetInstance().Hovering())
{
    mp_input->SetScrollZoom(m_zoom);
}


void Camera::UpdateMouseKeyPress()
{
    m_leftDown = mp_input->GetMouseButtonState(GLFW_MOUSE_BUTTON_1);
    m_rightDown = mp_input->GetMouseButtonState(GLFW_MOUSE_BUTTON_2);
    m_middleDown = mp_input->GetMouseButtonState(GLFW_MOUSE_BUTTON_3);
}

void Camera::UpdateMouseMovement(const double& x, const double& y)
{
    float val = 0.0025f;

    // Calculate the change in the mouse position
    int dx = static_cast<int>(x - m_prevMouseX);
    int dy = static_cast<int>(y - m_prevMouseY);

    // @@ Please don't disable this shifted-left-button code. 
    if (m_leftDown) {
        m_rot.y += dx * val;
        if (!m_invert)
            m_rot.x += dy * val;
        else
            m_rot.x -= dy * val;
    }

    if (m_rightDown) {
        m_tx += dx * val;
        m_ty -= dy * val;
    }

    // Record this position
    m_prevMouseX = x;
    m_prevMouseY = y;
}
