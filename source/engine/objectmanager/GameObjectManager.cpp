#include "../pch.h"
#include "GameObjectManager.h"
#include "../resourcemanager/ResourceManager.h"
#include "../scenemanager/SceneManager.h"
#include "../ComponentIncludes.h"

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

void GameObjectManager::AddGameObject(GameObject* object, GameObject* parent)
{
	SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->AddNode(object, parent);
	m_gameObjectMap[object->GetName()] = m_gameObjects.size();
	m_gameObjects.push_back(object);
}

void GameObjectManager::DeleteGameObject(GameObject* object)
{
	if (object == nullptr || m_gameObjectMap.find(object->GetName()) == m_gameObjectMap.end())
	{
		std::cerr << "GameObject not found" << std::endl;
		return;
	}

	SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->DeleteNode(object);

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

Node* GameObjectManager::CloneNode(Node* node)  
{  
Node* copy;  
if (dynamic_cast<GameObject*>(node))  
	copy = new GameObject();  
else   
	copy = new Node();  

for (auto& component : node->GetComponents())   
{  
	ComponentDictionary::ComponentTypes type = ComponentDictionary::componentStore.at(component.first);  
	using ComponentType = std::remove_pointer_t<std::decay_t<decltype(type)>>;  
	std::visit([&](auto value)  
		{  
			using ComponentType = std::remove_pointer_t<std::decay_t<decltype(value)>>;  
			if constexpr (std::is_same_v<ComponentType, TransformComponent>)  
			{  
				// Skip processing for TransformComponent  
				return;  
			}  
			try  
			{  
				ComponentType* newComp = copy->AddComponent<ComponentType>(true);  
				auto& getters = component.second->GetGetters();  
				auto& setters = newComp->GetSetters();  

				for (auto setter = setters.begin(); setter != setters.end(); setter++)  
				{  
					std::any val = getters.at(setter->first)();  
					setter->second(val);  
				}  
			}  
			catch (const std::bad_variant_access& e)  
			{  
				std::cout << "Component likely not properly defined in GameObjectSystemComponentConstants.cpp" << std::endl;  
			}  
		}, type);  
}
copy->GetComponent<TransformComponent>()->SetPosition(node->GetTransform()->GetPosition());
copy->GetComponent<TransformComponent>()->SetRotation(node->GetTransform()->GetRotation());
copy->GetComponent<TransformComponent>()->SetScale(node->GetTransform()->GetScale());
auto pos = node->GetName().find('\0');  
std::string nodeName = node->GetName();  
nodeName = (pos != std::string::npos) ? nodeName.substr(0, pos) : nodeName;  
return copy;  
}


