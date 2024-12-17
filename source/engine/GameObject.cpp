#include "pch.h"
#include "RenderComponent.h"
#include "Camera.h"
#include "ServiceLocator.h"

GameObject::~GameObject()
{
    for (auto it = m_components.begin(); it != m_components.end();)
    {
        if (it->second != nullptr)
        {
            it->second->SetOwner(nullptr);
            delete it->second;
            it->second = nullptr;
        }
        it = m_components.erase(it);
    }

}

void GameObject::Update()
{
	if (m_needsDeletion) return;

    for (auto component : m_components)
    {
        component.second->Update();
    }
}

void GameObject::Init()
{
}

void GameObject::Render()
{
    auto renderComponent = GetComponent<RenderComponent>();

    if (renderComponent != nullptr)
    {
        //TODO:---------Need to update project matrix later on
        auto frameBuffer = SERVICE_LOCATOR.GetWindowHandler()->FrameBuffer;
        //auto proj = glm::perspective(glm::radians(TestCamera::GetInstance()->GetZoom()), (float)frameBuffer.Width / (float)frameBuffer.Height, TestCamera::GetInstance()->nearPlane, TestCamera::GetInstance()->farPlane);
        GetTransform()->SetProjection(Camera::GetInstance()->m_worldProjection);
		//TODO: Remove TestCamera and use CameraComponent
		GetTransform()->SetView(Camera::GetInstance()->m_worldView);
        renderComponent->Render();
    }
}

void GameObject::Render(Shader* shader)
{
    auto renderComponent = GetComponent<RenderComponent>();

    if (renderComponent != nullptr)
    {
        //TODO:---------Need to update project matrix later on
        auto frameBuffer = SERVICE_LOCATOR.GetWindowHandler()->FrameBuffer;
        //auto proj = glm::perspective(glm::radians(TestCamera::GetInstance()->GetZoom()), (float)frameBuffer.Width / (float)frameBuffer.Height, TestCamera::GetInstance()->nearPlane, TestCamera::GetInstance()->farPlane);
        GetTransform()->SetProjection(Camera::GetInstance()->m_worldProjection);
        //TODO: Remove TestCamera and use CameraComponent
        GetTransform()->SetView(Camera::GetInstance()->m_worldView);
        renderComponent->Render(shader);
    }
}

void GameObject::SetDead(bool isDead)
{
    m_isAlive = !isDead;
}