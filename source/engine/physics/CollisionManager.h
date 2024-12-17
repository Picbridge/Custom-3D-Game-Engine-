#pragma once

class CollisionComponent;

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

// ****** Collision Response ****** //

	//@brief Handles a collision between two collision components
	void CollisionResponse(std::pair<CollisionComponent*, CollisionComponent*> collision);
	void CollisionResponse(CollisionComponent* shape1, CollisionComponent* shape2);
	//@brief Handles a collision between a static and dynamic collision component
	void StaticDynamicResponse(CollisionComponent* staticObject, CollisionComponent* dynamicObject);
	//@brief Handles a collision between two dynamic collision components
	void DynamicDynamicCollision(CollisionComponent* component1, CollisionComponent* component2);
	//@brief Handles a collision between two dynamic collision components
	void DynamicBounce(CollisionComponent* shape1, CollisionComponent* shape2);

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

private:
	CollisionManager();

	//@brief Returns the instance of the collision manager
	static CollisionManager* GetInstance();
	static std::unique_ptr<CollisionManager> instance;

	std::vector<CollisionComponent*> m_collisionComponents;

	friend class ServiceLocator;
};

