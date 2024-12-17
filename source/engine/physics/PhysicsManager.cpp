#include "../pch.h"
#include "PhysicsManager.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"

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
		delete m_physicsComponents[0];
	}
	m_physicsComponents.clear();
}
#pragma endregion

// ****** Collision Response ****** //
#pragma region CollisionResponse
void PhysicsManager::CollisionRouting(std::vector<std::pair<GameObject*, GameObject*>> collisions)
{
	for (auto& collision : collisions)
	{
		CollisionResponse(collision.first, collision.second);
	}
}

void PhysicsManager::CollisionResponse(GameObject* obj1, GameObject* obj2)
{
	bool shape1_isStatic = !obj1->HasComponent<PhysicsComponent>();
	bool shape2_isStatic = !obj2->HasComponent<PhysicsComponent>();

	if (shape1_isStatic && shape2_isStatic)
	{
		// Handle collision with static object
		// Do static objects collide?
	}
	else if (shape1_isStatic)
	{
		//StaticDynamicResponse(obj1, obj2);
	}
	else if (shape2_isStatic)
	{
		//StaticDynamicResponse(obj2, obj1);
	}
	else
	{
		// Handle collision with two dynamic objects
		//DynamicBounce(obj1, obj2);
	}
}

void PhysicsManager::StaticDynamicResponse(GameObject* staticObject, GameObject* dynamicObject)
{
	PhysicsComponent* physicsComponent = dynamicObject->GetComponent<PhysicsComponent>();
	// Calculate the normal of the collision
	glm::dvec3 dir = dynamicObject->GetTransform()->GetPosition() - staticObject->GetTransform()->GetPosition();
	dir = glm::normalize(dir);
	// Calculate static objects normal at the point of collision
	glm::dvec3 normal = staticObject->GetComponent<CollisionComponent>()->GetCollisionShape()->GetNormal(dir);
	// Calculate the reflection of the velocity
	glm::dvec3 velocity = physicsComponent->GetVelocity();
	glm::dvec3 reflection = velocity - 2.0 * glm::dot(velocity, normal) * normal;
	// Apply the reflection
	physicsComponent->SetVelocity(reflection);
}

void PhysicsManager::DynamicDynamicCollision(GameObject* obj1, GameObject* obj2)
{
	// Calculate the relative velocity
	// Calculate the normal of the collision
	// Calculate the reflection of the velocity
	// Apply the reflection
	DynamicBounce(obj1, obj2);
}

void PhysicsManager::DynamicBounce(GameObject* shape1, GameObject* shape2)
{

	// m1*v1 + m2*v2 = m1*v1' + m2*v2'
	// v1' = (v1*(m1-m2) + 2*m2*v2) / (m1+m2)
	// v2' = (v2*(m2-m1) + 2*m1*v1) / (m1+m2)
	PhysicsComponent* physicsComponent1 = shape1->GetComponent<PhysicsComponent>();
	PhysicsComponent* physicsComponent2 = shape2->GetComponent<PhysicsComponent>();
	// Calculate the relative velocity
	glm::vec3 relativeVelocity = physicsComponent1->GetVelocity() - physicsComponent2->GetVelocity();
	double m1 = physicsComponent1->GetMass();
	double m2 = physicsComponent2->GetMass();
	glm::dvec3 v1 = physicsComponent1->GetVelocity();
	glm::dvec3 v2 = physicsComponent2->GetVelocity();
	double inverse_sumedMasses = 1 / (m1 + m2);
	glm::dvec3 v1_prime = (v1 * (m1 - m2) + 2 * m2 * v2) * inverse_sumedMasses;
	glm::dvec3 v2_prime = (v2 * (m2 - m1) + 2 * m1 * v1) * inverse_sumedMasses;

	// Apply the impulse
	physicsComponent1->SetVelocity(v1_prime);
	physicsComponent2->SetVelocity(v2_prime);
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