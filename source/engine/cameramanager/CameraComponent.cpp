#include "../pch.h"
#include "CameraComponent.h"
#include "CameraManager.h"

CameraComponent::~CameraComponent()
{
	SERVICE_LOCATOR.GetCameraManager()->RemoveCamera(this);
}

void CameraComponent::Init()
{
    m_window = SERVICE_LOCATOR.GetWindowHandler();
    m_dynamicViewport = Viewport(0, 0, m_window->FrameBuffer.Width, m_window->FrameBuffer.Height);
    m_pTransform = GetOwner()->GetTransform();

    SERVICE_LOCATOR.GetCameraManager()->RegisterCameraComponent(this);
}

void CameraComponent::Update()
{
    //if (m_useDynamicClipping)
    //{
    //    m_nearPlane = 0.1f + m_pTransform->GetPosition().z * 0.01f;
    //    m_farPlane = 1000.0f + m_pTransform->GetPosition().z * 10.0f;
    //}
    //m_dynamicViewport.X = m_window->FrameBuffer.Width * m_xRatio;
    //m_dynamicViewport.Y = m_window->FrameBuffer.Height * m_yRatio;
    m_dynamicViewport.W = m_window->FrameBuffer.Width;
    m_dynamicViewport.H = m_window->FrameBuffer.Height;

    updateProjectionMatrix();
    updateViewMatrix();
}

void CameraComponent::Shutdown()
{
}

void CameraComponent::SetPosition(const glm::vec3 position)
{
     m_pTransform->SetPosition(position);
}

void CameraComponent::SetRotation(const glm::vec3 rotation)
{
    m_pTransform->SetRotation(rotation);
}

void CameraComponent::updateProjectionMatrix()
{
    if (m_dynamicViewport.H == 0)
    {
		return;
    }

    float aspectRatio = static_cast<float>(m_dynamicViewport.W) / static_cast<float>(m_dynamicViewport.H);
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(m_fov), aspectRatio, m_nearPlane, m_farPlane);
    m_pTransform->SetProjection(projectionMatrix);
}

void CameraComponent::updateViewMatrix()
{
    auto rotation = m_pTransform->GetRotation();
    auto position = m_pTransform->GetPosition();

    glm::quat pitchQuat = glm::angleAxis(glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat yawQuat = glm::angleAxis(glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat rollQuat = glm::angleAxis(glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::quat rotQuat = pitchQuat * yawQuat * rollQuat;

    glm::mat4 rotationMatrix = glm::toMat4(rotQuat);
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -position + glm::vec3(0,-0.2,0));

    m_pTransform->SetView(rotationMatrix * translationMatrix);
}

void CameraComponent::SetViewport(int x, int y, int width, int height) 
{ 
    m_viewport = Viewport(x, y, width, height);
    m_dynamicViewport = Viewport(x, y, width, height);

    m_xRatio = m_viewport.X / (float)m_window->FrameBuffer.Width;
    m_yRatio = m_viewport.Y / (float)m_window->FrameBuffer.Height;
    m_wRatio = m_viewport.W / (float)m_window->FrameBuffer.Width;
    m_hRatio = m_viewport.H / (float)m_window->FrameBuffer.Height;
}

void CameraComponent::SetViewport(Viewport viewport) 
{
    m_viewport = viewport;
    m_dynamicViewport = viewport;
    if (m_window == nullptr)
    {
        m_window = SERVICE_LOCATOR.GetWindowHandler();
    }
    m_xRatio = m_viewport.X / (float)m_window->FrameBuffer.Width;
    m_yRatio = m_viewport.Y / (float)m_window->FrameBuffer.Height;
    m_wRatio = m_viewport.W / (float)m_window->FrameBuffer.Width;
    m_hRatio = m_viewport.H / (float)m_window->FrameBuffer.Height;
}