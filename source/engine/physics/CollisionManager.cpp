#include "../pch.h"
#include "CollisionManager.h"
#include "CollisionComponent.h"
#include "PhysicsComponent.h"
#include "CollisionChecks.h"
#include "PhysicsManager.h"


// ****** CollisionManager ****** //
#pragma region CollisionManager
std::unique_ptr<CollisionManager> CollisionManager::instance = nullptr;

CollisionManager* CollisionManager::GetInstance()
{
	if (!instance)
	{
		instance = std::unique_ptr<CollisionManager>(new CollisionManager());
	}
	return instance.get();
}

CollisionManager::CollisionManager()
{
	Init();
}

CollisionManager::~CollisionManager()
{
	Shutdown();
}

void CollisionManager::Init()
{
	printf("CollisionManager Init\n");
}

void CollisionManager::Update()
{

	for (auto& component : m_collisionComponents)
	{
		component->Update();
	}
	std::vector<std::pair<GameObject*, GameObject*>> collisions;
	for (int i = 0; i < m_collisionComponents.size(); i++)
	{
		CollisionComponent* component1 = m_collisionComponents[i];
		for (int j = i + 1; j < m_collisionComponents.size(); j++)
		{
			CollisionComponent* component2 = m_collisionComponents[j];
			if (component1->CanCollideWith(component2))
			{
				if (CollisionChecks::CheckCollisionBetween(component1->GetCollisionShape(), component2->GetCollisionShape()))
				{
					GameObject* obj1 = dynamic_cast<GameObject*>(component1->GetOwner());
					GameObject* obj2 = dynamic_cast<GameObject*>(component2->GetOwner());
					if (obj1 && obj2) std::cout << "Collision detected between " << obj1->GetName() << " and " << obj2->GetName() << std::endl;
					//collisions.push_back(std::make_pair(component1, component2));
					collisions.push_back(std::make_pair(obj1, obj2));
				}
			}
		}
	}
	PhysicsManager* physicsManager = SERVICE_LOCATOR.GetPhysicsManager();
	physicsManager->CollisionRouting(collisions);
}

void CollisionManager::Shutdown()
{
	printf("CollisionManager Shutdown\n");
	while (!m_collisionComponents.empty())
	{
		delete m_collisionComponents[0];
	}
	m_collisionComponents.clear();
}
#pragma endregion

// ****** CollisionDetection ****** //
#pragma region CollisionDetection
CollisionComponent* CollisionManager::ShapeIsColliding(CollisionComponent* component) const
{
	for (auto& other : m_collisionComponents)
	{
		if (other == component) { continue; }
		if (component->CanCollideWith(other))
		{
			if (CollisionChecks::CheckCollisionBetween(component->GetCollisionShape(), other->GetCollisionShape()))
			{
				return other;
			}
		}
	}
	return nullptr;
}

bool CollisionManager::ShapeIsCollidingWith(CollisionComponent* component, CollisionComponent* other) const
{
	if (component == other) { return false; }
	if (component->CanCollideWith(other))
	{
		return CollisionChecks::CheckCollisionBetween(component->GetCollisionShape(), other->GetCollisionShape());
	}
	return false;
}
#pragma endregion

// ****** CollisionComponent Management ****** //
#pragma region CollisionComponent Management
CollisionComponent* CollisionManager::CreateCollisionComponent(int layer, int mask)
{
	std::unique_ptr<CollisionComponent> component = std::unique_ptr<CollisionComponent> (new CollisionComponent);
	component->SetCollisionLayer(layer);
	component->SetCollisionMask(mask);
	AddCollisionComponent(component.get());
	return component.get();
}

void CollisionManager::AddCollisionComponent(CollisionComponent* component)
{
	if (std::find(m_collisionComponents.begin(), m_collisionComponents.end(), component) != m_collisionComponents.end())
		return;
	m_collisionComponents.push_back(component);
}

void CollisionManager::RemoveCollisionComponent(CollisionComponent* component)
{
	auto it = std::find(m_collisionComponents.begin(), m_collisionComponents.end(), component);
	if (it != m_collisionComponents.end())
	{
		m_collisionComponents.erase(it);
	}
}
#pragma endregion