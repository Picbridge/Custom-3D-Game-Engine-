#include "../pch.h"
#include "PhysicsManager.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "Collision.h"

// ****** Physics Manager ****** //
#pragma region PhysicsManager
std::unique_ptr<PhysicsManager> PhysicsManager::instance = nullptr;

PhysicsManager* PhysicsManager::GetInstance()
{
	if (!instance)
	{
		instance = std::unique_ptr<PhysicsManager>(new PhysicsManager());
	}
	return instance.get();
}

PhysicsManager::PhysicsManager()
{
	Init();
}

PhysicsManager::~PhysicsManager()
{
	Shutdown();
}

void PhysicsManager::Init()
{
	printf("PhysicsManager Init\n");

}

void PhysicsManager::Update(double dt)
{
	for (auto& component : m_physicsComponents)
	{
		component->Update(dt);
	}
}

void PhysicsManager::Shutdown()
{
	printf("PhysicsManager Shutdown\n");
	while (!m_physicsComponents.empty())
	{
		auto component = m_physicsComponents.begin();
		m_physicsComponents.erase(component);
	}
}
#pragma endregion

// ****** Collision Response ****** //
#pragma region CollisionResponse

void PhysicsManager::CollisionResponse(std::vector<Collision*> collisions)
{
	for (auto& collision : collisions)
	{
		if (collision->IsDynamicCollision())
		{
			//DynamicDynamicCollision(collision);
		}
		else
		{
			StaticDynamicResponse(collision);
		}
	}
}

void PhysicsManager::StaticDynamicResponse(Collision* collision)
{
    GameObject* dynamicObject = collision->GetDynamicObject();
    PhysicsComponent* dynamicComponent = dynamicObject->GetComponent<PhysicsComponent>();
    glm::dvec3 normal = collision->GetCollisionNormal();
    glm::dvec3 velocity = dynamicComponent->GetVelocity();
	if (velocity == glm::dvec3(0)) return;
    double restitution = dynamicComponent->GetBounciness();
	glm::dvec3 reflection = velocity - ((1 + restitution) * glm::dot(velocity, normal) * normal);
    dynamicComponent->SetVelocity(reflection);
}
#pragma endregion

// ****** PhysicsComponent Management ****** //
#pragma region PhysicsComponentManagement
void PhysicsManager::AddPhysicsComponent(PhysicsComponent* component)
{
	m_physicsComponents.push_back(component);
}

void PhysicsManager::RemovePhysicsComponent(PhysicsComponent* component)
{
	auto it = std::find(m_physicsComponents.begin(), m_physicsComponents.end(), component);
	if (it != m_physicsComponents.end())
	{
		m_physicsComponents.erase(it);
	}
}
#pragma endregion