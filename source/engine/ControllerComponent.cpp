#include "pch.h"
#include "Camera.h"
#include "ControllerComponent.h"

#include "ui/UI.h"
#include "Input.h"
#include "physics/PhysicsComponent.h"
#include "physics/PhysicsManager.h"
#include "ServiceLocator.h"

void ControllerComponent::Init()
{}

void ControllerComponent::Update()
{
	GLFWwindow* window = SERVICE_LOCATOR.GetWindowHandler()->GetCurrentContext();
	Transform* pTransform = pOwner->GetTransform();
	glm::vec3 pPos = pTransform->GetPosition();
	glm::vec3 pRot = pTransform->GetRotation();

	if (m_InputHandler->IsGamepadPresent(GLFW_JOYSTICK_1))
	{
		bool invert = SERVICE_LOCATOR.GetUI()->GetInstance().GetState("Invert Camera Y-Axis", IMGUI_ELEMENT_TYPE::BUTTON);
		Camera* cam = Camera::GetInstance();
		GLFWgamepadstate state = m_InputHandler->GetGamepadState(GLFW_JOYSTICK_1);
		float rightX = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
		float rightY = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
		if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP]) 
		{
			pPos.y += m_speed;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN])
		{
			pPos.y -= m_speed;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT])
		{
			pPos.x -= m_speed;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT])
		{
			pPos.x += m_speed;
		}
		if (rightY >= 0.5) 
		{
			if (!invert)
				cam->m_rot.x += m_speed;
			else
				cam->m_rot.x -= m_speed;
		}
		if (rightY <= -0.5)
		{
			if (!invert)
				cam->m_rot.x -= m_speed;
			else
				cam->m_rot.x += m_speed;
		}
		if (rightX >= 0.5)
		{
			cam->m_rot.y += m_speed;
		}
		if (rightX <= -0.5)
		{
			cam->m_rot.y -= m_speed;
		}
	}
	if (m_InputHandler->IsKeyPressed(GLFW_KEY_W)) 
	{
		pPos.y += m_speed;
	}
	if (m_InputHandler->IsKeyPressed(GLFW_KEY_S))
	{
		pPos.y -= m_speed;
	}
	if (m_InputHandler->IsKeyPressed(GLFW_KEY_A))
	{
		pPos.x -= m_speed;
	}
	if (m_InputHandler->IsKeyPressed(GLFW_KEY_D))
	{
		pPos.x += m_speed;
	}
	if (m_InputHandler->IsKeyPressed(GLFW_KEY_Q))
	{
		pRot.z += 5;
	}
	if (m_InputHandler->IsKeyPressed(GLFW_KEY_E))
	{
		pRot.z -= 5;
	}
	if (m_InputHandler->IsKeyPressed(GLFW_KEY_SPACE))
	{
		auto* physicsComp = pOwner->GetComponent<PhysicsComponent>();
		if (!physicsComp) { return; }
		if (!physicsComp->Grounded()) { return; }
		glm::dvec3 gravity = SERVICE_LOCATOR.GetPhysicsManager()->GetGravity();
		glm::dvec3 gravNorm = glm::normalize(gravity);
		double jumpForce = 5.0;
		glm::dvec3 jump = -gravNorm * jumpForce;
		physicsComp->ApplyForce(jump);
	}
	pTransform->SetPosition(pPos);
	pTransform->SetRotation(pRot);
}

void ControllerComponent::Shutdown()
{}
