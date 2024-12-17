#include "../pch.h"
#include "SampleGame.h"
#include "../Camera.h"
#include "../scenemanager/SceneManager.h"
#include "../resourcemanager/ResourceFactory.h"
#include "../objectmanager/GameObjectFactory.h"
#include "../objectmanager/GameObjectManager.h"
#include "../DeserializeJSON.h"
#include "../VectorCalculations.h"
#include "../AudioManager.h"
#include "TransformComponent.h"
#include "../Input.h"

#include "../physics/PhysicsComponent.h"
#include "../physics/CollisionComponent.h"
#include "RenderComponent.h"

void sample::Init()
{
	printf("Initializing sample scene!!\n");
	SERVICE_LOCATOR.GetResourceFactory()->CreateAllResources("SampleGame/SampleGameResource.json");
	SERVICE_LOCATOR.GetSceneManager()->AddScene("scene_01");
	SERVICE_LOCATOR.GetSceneManager()->AddScene("scene_02");
	// Add more scenes here in order...
	// Adding empty scene for testing
	SERVICE_LOCATOR.GetSceneManager()->AddScene();
	SERVICE_LOCATOR.GetAudioManager()->PlaySound("music\\dragon_soul.mp3");

	Camera* cam = Camera::GetInstance();
	cam->m_rot.z = 6.29f;
	cam->m_rot.x = 33.48f;
	//TestCamera::GetInstance()->Init();

	DeserializeJSON::LoadImGui("../../content/code/json_files/UI.json");

	SERVICE_LOCATOR.GetSceneManager()->SetCurrentScene("scene_01");
	SERVICE_LOCATOR.GetSceneManager()->MoveToNextScene();// Empty Scene

	//SERVICE_LOCATOR.GetSceneManager()->ExportScene(SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene());
	std::cout << "sample::Init()" << std::endl;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_Particle.colorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.colorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.sizeBegin = 0.25f, m_Particle.sizeVariation = 0.3f, m_Particle.sizeEnd = 0.0f;
	m_Particle.lifetime = 10.0f;
	m_Particle.velocity = { 0.0f, 0.0f, 0.0f };
	m_Particle.velocityVariation = { 2.0f, 2.0f, 2.0f };
	m_Particle.position = { 0.0f, 0.0f, -15.0f };

}

void sample::Update()
{
	SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->Update();

	// Checking the dt and vector calculation functions
	//auto objTransform = SERVICE_LOCATOR.GetGameObjectManager()->GetGameObjects()[0]->GetComponent<TransformComponent>();
	//auto rot = objTransform->GetRotation();
	//auto forward = VectorCalculation::GetForwardVec(rot);
	//auto right = VectorCalculation::GetRightVec(rot);
	//auto up = VectorCalculation::GetUpVec(rot);

	float dt = static_cast<float>(SERVICE_LOCATOR.GetTime()->GetDeltaTime());
	//objTransform->SetRotation(rot + glm::vec3(0.0f, 0.0f, 5.f * dt));
	const float frequency = 1.0f;  // Oscillations per second
	const float amplitude = 1.0f;  // Maximum displacement in the Y-axis
	const float speed = 19.0f;      // Controls the speed of wave propagation
	if (SERVICE_LOCATOR.GetInput()->IsKeyJustPressed(GLFW_KEY_B))
	{
		glm::vec3 objPos = SERVICE_LOCATOR.GetGameObjectManager()->GetGameObject("GreenBunny")->GetComponent<TransformComponent>()->GetPosition();
		SERVICE_LOCATOR.GetAudioManager()->PlaySound("sound_effects\\banana_bread.mp3", &objPos);
	}
	if (SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->GetName() == "scene_03")
	{
		for (unsigned int i = 0; i < 10; ++i)
		{
			for (unsigned int j = 0; j < 10; ++j)
			{
				float waveHeight = amplitude * std::sin(frequency * glfwGetTime() + speed * (i + j));
				auto transform = SERVICE_LOCATOR.GetGameObjectManager()->GetGameObjects()[i * 10 + j]->GetComponent<TransformComponent>();
				transform->SetPosition(glm::vec3(i, waveHeight, j));
			}
		}
	}

	m_ParticleSystem.Update(dt);
}

void sample::PostUpdate()
{
	SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->PostUpdate();
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//TestCamera::GetInstance()->Update();
	for (int i = 0; i < 1; ++i)
		m_ParticleSystem.Emit(m_Particle);


}

void sample::Render()
{
	SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->Render();
	m_ParticleSystem.Render();
}

void sample::Shutdown()
{
	SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->Shutdown();
	std::cout << "sample::Shutdown()" << std::endl;
}