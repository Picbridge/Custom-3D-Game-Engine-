#include "pch.h"
#include "cameramanager/CameraComponent.h"
#include "CameraControllerComponent.h"

#include "ui/UI.h"
#include "Input.h"
#include "physics/PhysicsComponent.h"
#include "physics/PhysicsManager.h"
#include "VectorCalculations.h"

CameraControllerComponent::CameraControllerComponent()
    : m_pInputHandler(SERVICE_LOCATOR.GetInput()), m_zoom(0),
    m_speed(300.0 / 60.0), m_focussedOnUI(SERVICE_LOCATOR.GetUI()->GetInstance().Hovering()),
	m_invert(false), m_leftDown(false), m_rightDown(false), m_middleDown(false),
	m_prevMouseX(0), m_prevMouseY(0),m_rot(0), m_pos(0), m_pCamera(nullptr), m_pTransform(nullptr)
{
    m_pInputHandler->SetScrollZoom(m_zoom);
}

CameraControllerComponent::~CameraControllerComponent() {}

void CameraControllerComponent::Init()
{
	m_pCamera = pOwner->GetComponent<CameraComponent>();
    m_pTransform = pOwner->GetTransform();
    m_rot = m_pTransform->GetRotation();
    m_pos = m_pTransform->GetPosition();
    m_zoom = m_pCamera->GetFOV();
}

void CameraControllerComponent::Update()
{
	auto dt = SERVICE_LOCATOR.GetTime()->GetDeltaTime();
    m_invert = SERVICE_LOCATOR.GetSystemSettings()->GetBoolSetting("Invert Y-Axis");
    //std::cout << "m_invert: " << m_invert << std::endl;
    if (!m_focussedOnUI)
    {
        m_zoom += m_pInputHandler->GetScrollZoomDelta();
        m_pCamera->SetFOV(m_zoom);
        updateMouseClick();
    }

    const float mouseX = static_cast<float>(m_pInputHandler->GetMouseX());
    const float mouseY = static_cast<float>(m_pInputHandler->GetMouseY());

    if (!m_focussedOnUI)
        updateMouseMovement(mouseX, mouseY);
    else
    {
        m_prevMouseX = mouseX;
        m_prevMouseY = mouseY;
    }

    updateKeyboardMovement();
    
    m_pCamera->SetRotation(m_rot);
    m_pCamera->SetPosition(m_pos);
}

void CameraControllerComponent::Shutdown()
{
}

void CameraControllerComponent::updateMouseClick()
{
    m_leftDown = m_pInputHandler->GetMouseButtonState(GLFW_MOUSE_BUTTON_1);
    m_rightDown = m_pInputHandler->GetMouseButtonState(GLFW_MOUSE_BUTTON_2);
    m_middleDown = m_pInputHandler->GetMouseButtonState(GLFW_MOUSE_BUTTON_3);
}

void CameraControllerComponent::updateMouseMovement(const double& x, const double& y)
{
	auto dt = SERVICE_LOCATOR.GetTime()->GetDeltaTime();
    float val = 0.1f; // Adjusted for smoother rotation
	m_rot = m_pTransform->GetRotation();
	m_pos = m_pTransform->GetPosition();

    // Calculate the change in the mouse position
    float dx = static_cast<float>(x - m_prevMouseX);
    float dy = static_cast<float>(y - m_prevMouseY);

    if (m_leftDown) 
    {
        m_rot.y += dx * val;
        m_rot.x += (!m_invert ? dy : -dy) * val;
    }

    if (m_rightDown) 
    {
        m_pos.x += dx * val;
        m_pos.y -= dy * val;
    }

    // Record this position
    m_prevMouseX = static_cast<float>(x);
    m_prevMouseY = static_cast<float>(y);
}

void CameraControllerComponent::updateKeyboardMovement()
{
    if (SERVICE_LOCATOR.GetUI()->GetDebugState() && m_pInputHandler->IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
        return;

	auto speed = m_speed;
	if (m_pInputHandler->IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		speed *= 2;
	auto view = m_pTransform->GetView();
	auto right = VectorCalculation::GetRightVecFromView(view);
	auto forward = VectorCalculation::GetForwardVecFromView(view);
	auto up = VectorCalculation::GetUpVecFromView(view);

    float dt = SERVICE_LOCATOR.GetTime()->GetDeltaTime();

    if (!SERVICE_LOCATOR.GetUI()->Interacting()) 
    {
        if (m_pInputHandler->IsKeyPressed(GLFW_KEY_W))
            m_pos += forward * (speed * dt);
        if (m_pInputHandler->IsKeyPressed(GLFW_KEY_S))
            m_pos -= forward * (speed * dt);
        if (m_pInputHandler->IsKeyPressed(GLFW_KEY_A))
            m_pos -= right * (speed * dt);
        if (m_pInputHandler->IsKeyPressed(GLFW_KEY_D))
            m_pos += right * (speed * dt);
        if (m_pInputHandler->IsKeyPressed(GLFW_KEY_Q))
            m_pos -= up * (speed * dt);
        if (m_pInputHandler->IsKeyPressed(GLFW_KEY_E))
            m_pos += up * (speed * dt);
    }
}