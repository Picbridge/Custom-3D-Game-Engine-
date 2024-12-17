#include "../pch.h"
#include "GameObjectManager.h"
#include "../resourcemanager/ResourceManager.h"
#include "../scenemanager/SceneManager.h"

std::unique_ptr<GameObjectManager> GameObjectManager::instance = nullptr;

GameObjectManager* GameObjectManager::GetInstance()
{
	if (!instance)
		instance = std::unique_ptr<GameObjectManager>(new GameObjectManager());

	return instance.get();
}

GameObjectManager::~GameObjectManager()
{
	for (auto& go : m_gameObjects)
		go->Destroy();

	m_gameObjects.clear();
	m_gameObjectMap.clear();
}

void GameObjectManager::Init()
{
	for (auto& go : m_gameObjects)
		go->Init();
	std::cout << "GameObjectManager Initialized" << std::endl;
}

void GameObjectManager::Update()
{
	for (auto& go : m_gameObjects)
		go->Update();
}

void GameObjectManager::Render()
{
	for (auto& go : m_gameObjects)
		go->Render();
}

void GameObjectManager::Shutdown()
{
	for (auto& go : m_gameObjects)
		go->Destroy();
	std::cout << "GameObjectManager Shutdown" << std::endl;
}

void GameObjectManager::AddGameObject(GameObject* object)
{
	SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->AddNode(object);
	m_gameObjectMap[object->GetName()] = m_gameObjects.size();
	m_gameObjects.push_back(object);
}

void GameObjectManager::DeleteGameObject(GameObject* object)
{
	object->Destroy();
	size_t index = m_gameObjectMap[object->GetName()];
	auto last = m_gameObjects.back();
	if (index != m_gameObjects.size() - 1)
	{
		m_gameObjects[index] = last;
		m_gameObjectMap[last->GetName()] = index;
	}
	m_gameObjects.pop_back();
	m_gameObjectMap.erase(object->GetName());
}


