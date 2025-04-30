#pragma once
#include "Raycast.h"

class CollisionComponent;
class Collision;

class CollisionManager
{
public:
	~CollisionManager();

	//@brief Initializes the collision manager
	void Init();
	//@brief Updates the collision manager
	void Update();
	//@brief Shuts down the collision manager
	void Shutdown();
	
// ****** Collision Detection ****** //
	
	//@brief Checks if a collision component is colliding with any other collision component
	CollisionComponent* ShapeIsColliding(CollisionComponent* component) const;
	//@brief Checks if a collision component is colliding with another collision component
	bool ShapeIsCollidingWith(CollisionComponent* component, CollisionComponent* other) const;

// ****** CollisionComponent Management ****** //

	// @brief Creates a collision component
	// @return CollisionComponent* The created collision component
	// @param layer : The layer of the collision component
	// @param mask : The mask of the collision component
	CollisionComponent* CreateCollisionComponent(int layer, int mask);
	//@brief Adds a collision component to the collision manager
	//@param component : The collision component to add
	void AddCollisionComponent(CollisionComponent* component);
	//@brief Removes a collision component from the collision manager
	//@param component : The collision component to remove
	void RemoveCollisionComponent(CollisionComponent* component);

    // ****** Raycast ****** //
    //@brief Cast a ray and return the first collision
    //@param origin : The origin of the ray
    //@param direction : The direction of the ray
    //@param collisionMask : The collision mask of the ray  (default = -1: hit everything
    //@param max_distance : The maximum distance of the ray (default = -1: no max)
    //@return Raycast The raycast result
    Raycast CastRay(glm::dvec3 origin, glm::dvec3 direction, int collisionMask = -1, double max_distance = -1);


	EventListener* m_eventListener; // Event handler for the component
	static CollisionManager* GetInstance();
private:
	CollisionManager();

	//@brief Returns the instance of the collision manager
	static std::unique_ptr<CollisionManager> instance;

	std::vector<CollisionComponent*> m_collisionComponents;

	friend class ServiceLocator;
};

