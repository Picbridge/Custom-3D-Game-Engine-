#include "../pch.h"
#include "CollisionManager.h"
#include "CollisionComponent.h"
#include "PhysicsManager.h"
#include "Raycast.h"
#include "Collision.h"
#include "../events/EventListener.h"
#include "../events/CollisionEvent.h"


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

Raycast CollisionManager::CastRay(glm::dvec3 origin, glm::dvec3 direction, int collisionMask, double max_distance)
{
    Raycast bestRaycast;
	for (auto& component : m_collisionComponents)
	{
		// if the component is a trigger, skip it
        if (component->IsTrigger()) { continue; }
		// if mask isn't allowed to hit anything and the layer/mask doesn't line up, continue
		if (collisionMask != -1 && !(component->GetCollisionLayer() & collisionMask) ) { continue; }
        // TODO: check if the shape is within the max distance before doing the raycast
        //       Maybe just see if the farthest point of the shape is within the max distance
        CollisionShape* shape = component->GetCollisionShape();
        Raycast raycast(origin, direction, shape);
		if (raycast.Hit() && (max_distance == -1 || raycast.GetLength() < max_distance))
        {
            if (!bestRaycast.Hit() || raycast.GetLength() < bestRaycast.GetLength())
            {
                bestRaycast = raycast;
            }
        }
    }
    return bestRaycast;
}

//CollisionManager::CollisionManager() : m_eventListener(nullptr)
CollisionManager::CollisionManager() : m_eventListener(nullptr)
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Init()
{
    printf("CollisionManager Init\n");
    m_eventListener = new EventListener();
}

void CollisionManager::Update()
{
	for (auto& component : m_collisionComponents)
	{
		component->Update();
	}
    std::vector<Collision*> objectCollisions;
	std::vector<Collision*> triggerCollisions;
    // Collect the collisions
	for (int i = 0; i < m_collisionComponents.size(); i++)
	{
		CollisionComponent* component1 = m_collisionComponents[i];
		for (int j = i + 1; j < m_collisionComponents.size(); j++)
		{
			CollisionComponent* component2 = m_collisionComponents[j];
			if (component1->CanCollideWith(component2) && component2->CanCollideWith(component1))
			{
                Collision* collision = new Collision(component1, component2);
				if (collision->IsColliding())
				{
					if (collision->IsTrigger())
					{
                        triggerCollisions.push_back(collision);
                    }
					else
					{
						objectCollisions.push_back(collision);
					}
				}
			}
		}
	}
    // Resolve the triggers
    for (auto& trigger : triggerCollisions)
    {
        CollisionEvent* event = new CollisionEvent("Triggered", trigger);
        m_eventListener->BroadcastEvent(event);
    }
    // Resolve the collisions
    for (auto& collision : objectCollisions)
    {
		collision->ResolveCollision();
		CollisionEvent* event = new CollisionEvent("Collided", collision);
		m_eventListener->BroadcastEvent(event);
    }
    // Pass to the physics manager for collision response
	PhysicsManager* physicsManager = SERVICE_LOCATOR.GetPhysicsManager();
	physicsManager->CollisionResponse(objectCollisions);
}

void CollisionManager::Shutdown()
{
	printf("CollisionManager Shutdown\n");
	while (!m_collisionComponents.empty()) 
	{
		auto component = m_collisionComponents.begin();
		m_collisionComponents.erase(component);
	}
    if (m_eventListener)
    {
        delete m_eventListener;
        m_eventListener = nullptr;
    }
}
#pragma endregion

// ****** CollisionDetection ****** //
#pragma region CollisionDetection
//@brief Checks if a collision component is colliding with any other collision component
//@param component : The collision component to check
//@return The collision component that is colliding with the passed component, or nullptr if nothings is colliding
CollisionComponent* CollisionManager::ShapeIsColliding(CollisionComponent* component) const
{
	for (auto& other : m_collisionComponents)
	{
		if (other == component) { continue; }
		if (component->CanCollideWith(other))
		{
			Collision collision{ component,other };
			if (collision.IsColliding()) { return other; }
		}
	}
	return nullptr;
}

bool CollisionManager::ShapeIsCollidingWith(CollisionComponent* component, CollisionComponent* other) const
{
	if (component == other) { return false; }
	if (!component->CanCollideWith(other)) { return false; }
	return Collision(component,other).IsColliding();
}
#pragma endregion

// ****** CollisionComponent Management ****** //
#pragma region CollisionComponent Management
CollisionComponent* CollisionManager::CreateCollisionComponent(int layer, int mask)
{
	std::unique_ptr<CollisionComponent> component = std::make_unique<CollisionComponent> (layer,mask);
	AddCollisionComponent(component.get());
	return component.release();
}

void CollisionManager::AddCollisionComponent(CollisionComponent* component)
{
	assert(component && "Component is null");
	bool was_found = std::find(m_collisionComponents.begin(), m_collisionComponents.end(), component) != m_collisionComponents.end();
	assert(!was_found && "This component already exists in CollisionManager.");
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