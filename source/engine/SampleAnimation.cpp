#include "pch.h"
#include "SampleAnimation.h"
#include "Model.h"
#include "ServiceLocator.h"
#include "Camera.h"

void SampleAnimation::Init()
{
	root = std::unique_ptr<Model>(new Model("../../content/art/fbx/Body Block.fbx"));
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void SampleAnimation::Update()
{
	for (auto& go : m_gameObjects)
		go->Update();
	root->Update(static_cast<float>(SERVICE_LOCATOR.GetTime()->GetDeltaTime()));
}

void SampleAnimation::PostUpdate()
{
	for (auto& go : m_gameObjects)
		go->Flush();
}

void SampleAnimation::Render()
{
	root->DrawSkeleton(Camera::GetInstance()->m_worldProjection, Camera::GetInstance()->m_worldView, root->origin, glm::scale(glm::mat4(1), glm::vec3(0.05, 0.05, 0.05)));
	//m_pSkybox->Render();
	for (auto& go : m_gameObjects)
		go->Render();
}

void SampleAnimation::Shutdown()
{
	for (auto& go : m_gameObjects)
		go->Destroy();
	std::cout << "sample::Shutdown()" << std::endl;
}
