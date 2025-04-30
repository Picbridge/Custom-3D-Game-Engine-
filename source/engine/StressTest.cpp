#include "pch.h"
#include "StressTest.h"
#include "VectorCalculations.h"
#include "TransformComponent.h"
#include "resourcemanager/ResourceFactory.h"
#include "objectmanager/GameObjectManager.h"
#include "objectmanager/GameObjectFactory.h"
#include "scenemanager/SceneManager.h"

#include "ServiceLocator.h"

void StressTest::Init()
{
	SERVICE_LOCATOR.GetResourceFactory()->CreateAllResources();
	SERVICE_LOCATOR.GetSceneManager()->AddScene("GeneratedGameObjectsUnique");

	DeserializeJSON::LoadEngineUI("../../content/code/json_files/EngineUI.json");

	SERVICE_LOCATOR.GetSceneManager()->SetCurrentScene("GeneratedGameObjectsUnique");


	SERVICE_LOCATOR.GetSceneManager()->ExportScene(SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene());
	std::cout << "sample::Init()" << std::endl;
}

void StressTest::Update()
{
	SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->Update();

	// Checking the dt and vector calculation functions
	for (int i = 0; i < 100; i++)
	{
		auto objTransform = SERVICE_LOCATOR.GetGameObjectManager()->GetGameObjects()[i]->GetComponent<TransformComponent>();
		objTransform->SetPosition(glm::vec3(5 * sin(i) + i / 2.0f, 5 * cos(i), 5 * sin(i) * cos(i)));
		float dt = static_cast<float>(SERVICE_LOCATOR.GetTime()->GetDeltaTime());
		auto rot = objTransform->GetRotation();
		objTransform->SetRotation(rot + glm::vec3(8.0f * dt, 8.0f * dt, 0.0f));
	}
	auto objTransform = SERVICE_LOCATOR.GetGameObjectManager()->GetGameObjects()[0]->GetComponent<TransformComponent>();
	auto rot = objTransform->GetRotation();
	auto forward = VectorCalculation::GetForwardVec(rot);
	auto right = VectorCalculation::GetRightVec(rot);
	auto up = VectorCalculation::GetUpVec(rot);

	float dt = static_cast<float>(SERVICE_LOCATOR.GetTime()->GetDeltaTime());
	//objTransform->SetRotation(rot + glm::vec3(0.0f, 0.0f, 5.f * dt));

	//std::cout << "Forward: " << forward.x << " " << forward.y << " " << forward.z << std::endl;
	//std::cout << "Right: " << right.x << " " << right.y << " " << right.z << std::endl;
	//std::cout << "Up: " << up.x << " " << up.y << " " << up.z << std::endl;
}

void StressTest::Render()
{
	SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->Render();
}

void StressTest::Shutdown()
{
	SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->Shutdown();
	std::cout << "sample::Shutdown()" << std::endl;
}