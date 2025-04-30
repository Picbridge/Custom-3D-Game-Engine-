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
	//@brief Return the normlaized gravity vector
	inline glm::dvec3 GetGravityDir() const { return m_gravity != glm::dvec3(0) ? glm::normalize(m_gravity) : glm::dvec3(0); }
	//@brief Sets the gravity
	//@param g : The gravity to set
	inline void SetGravity(glm::vec3 g) { m_gravity = g; }

	// ****** Collision Response ****** //
	//@brief Loops through all collisions and handles them
	void CollisionResponse(std::vector<Collision*> collisions);
	//@brief Handles a collision between a static and dynamic physics component
	void StaticDynamicResponse(Collision* collision);
	//@brief Handles a collision between two dynamic physics components
	//void DynamicDynamicCollision(Collision* collision);


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

