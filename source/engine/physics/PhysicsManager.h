#pragma once

class PhysicsComponent;

class PhysicsManager
{
public:
	~PhysicsManager();

	// ****** Physics Engine ****** //

	//@brief Initializes the physics engine
	void Init();
	//@brief Updates the physics engine
	void Update(double dt);
	//@brief Shuts down the physics engine
	void Shutdown();

	// ****** PhysicsComponent Management ****** //
	
	//@brief Adds a physics component to the physics engine
	//@param component : The physics component to add
	void AddPhysicsComponent(PhysicsComponent* component);
	//@brief Removes a physics component from the physics engine
	//@param component : The physics component to remove
	void RemovePhysicsComponent(PhysicsComponent* component);

	// ****** Physics Settings ****** //
	// 
	//@brief Returns the gravity
	//@return double : The gravity
	inline glm::dvec3 GetGravity() const { return m_gravity; }
	//@brief Sets the gravity
	//@param g : The gravity to set
	inline void SetGravity(glm::vec3 g) { m_gravity = g; }

	// ****** Collision Response ****** //
	//@brief Loops through all collisions and handles them
	void CollisionRouting(std::vector<std::pair<GameObject*,GameObject*>> collisions);
	//@brief Handles a collision between two physics components
	void CollisionResponse(GameObject* obj1, GameObject* obj2);
	//@brief Handles a collision between a static and dynamic physics component
	void StaticDynamicResponse(GameObject* staticObject, GameObject* dynamicObject);
	//@brief Handles a collision between two dynamic physics components
	void DynamicDynamicCollision(GameObject* obj1, GameObject* obj2);
	//@brief Handles the bounce between two dynamic physics components
	void DynamicBounce(GameObject* shape1, GameObject* shape2);


private:
	PhysicsManager();

	//@brief Defines gravitational force and direction
	glm::dvec3 m_gravity { 0,-9.8,0 };

	//@brief Returns the instance of the physics manager
	static PhysicsManager* GetInstance();
	static std::unique_ptr<PhysicsManager> instance;

	std::vector<PhysicsComponent*> m_physicsComponents;

	friend class ServiceLocator;
};

