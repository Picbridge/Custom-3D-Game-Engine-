#pragma once

class CollisionComponent;

class PhysicsComponent : public Component
{
public:

	PhysicsComponent(double mass = 1, double gravityMultiplyer = 1, double bounciness = 0, 
		double drag = 0, double rotationalDrag = 0);

	~PhysicsComponent();

	//-------------------
	//Component essentials
	//-------------------

	//@brief Initialize the component
	void Init() override;
	//@brief Update the component
	void Update(double deltaTime);
	//@brief Update the component
	void Update() override;
	//@brief Shutdown the component
	void Shutdown() override;

	//-------------------
	// PhysicsComponent essentials
	//-------------------
	//@brief Manage object reaction to being grounded
	void GroundedResponse();
	//@brief Expensive check if the object is grounded
	std::optional < std::pair<CollisionComponent, CollisionComponent>> CheckForGround();
	//@brief Cheap check if the object is still grounded
	bool IsStillGrounded();
	//@brief Apply a force to the object
	//@param force : The force to apply
	void ApplyForce(const glm::dvec3& force)
	{
		m_acceleration += force * m_inverseMass;
	}
	//@brief Apply a force to the object
	//@param x : The x component of the force
	//@param y : The y component of the force
	//@param z : The z component of the force
	void ApplyForce(double x, double y, double z)
	{
		m_acceleration.x += x * m_inverseMass;
		m_acceleration.y += y * m_inverseMass;
		m_acceleration.z += z * m_inverseMass;
	}
	//@brief Apply a torque to the object
	// @param torque : The torque to apply
	void ApplyTorque(const glm::dvec3& torque)
	{
		m_rotationalAcceleration += torque;
	}
	//@brief Apply a torque to the object
	//@param x : The x component of the torque
	//@param y : The y component of the torque
	//@param z : The z component of the torque
	void ApplyTorque(double x, double y, double z)
	{
		m_rotationalAcceleration.x += x;
		m_rotationalAcceleration.y += y;
		m_rotationalAcceleration.z += z;
	}

	//--------------------------------
	// Setters
	//--------------------------------

	//@brief Set the grounded state of the object
	//@param grounded : The grounded state to set
	inline void SetGrounded(bool grounded) { m_grounded = grounded; }
	//@brief Set the velocity of the object
	//@param velocity : The velocity to set
	inline void SetVelocity(const glm::dvec3& velocity) { m_velocity = velocity; }
	//@brief Set the velocity of the object
	//@param x : The x component of the velocity
	//@param y : The y component of the velocity
	//@param z : The z component of the velocity
	inline void SetVelocity(double x, double y, double z)
	{
		m_velocity.x = x;
		m_velocity.y = y;
		m_velocity.z = z;
	}
	//@brief Set the mass of the object
	//@param mass : The mass to set
	inline void SetMass(double mass) { m_mass = mass; m_inverseMass = 1 / mass; }
	//@brief Set the drag of the object
	//@param drag : The drag to set
	inline void SetDrag(double drag) { m_drag = drag; }
	//@brief Set the gravity of the object
	//@param gravity : The gravity to set
	inline void SetGravityMultiplyer(double gravity) { m_gravityMultiplier = gravity; }

	//--------------------------------
	// Getters
	//--------------------------------

	//@brief Get the grounded state of the object
	//@return bool The grounded state of the object
	inline bool Grounded() { return m_grounded; }
	//@brief Get the velocity of the object
	//@return glm::dvec3 The velocity of the object
	inline glm::dvec3 GetVelocity() { return m_velocity; }
	//@brief Get the mass of the object
	//@return double The mass of the object
	inline double GetMass() { return m_mass; }
	//@brief Get the inverse mass of the object
	// @return double The inverse mass of the object
	inline double GetInverseMass() { return m_inverseMass; }
	//@brief Get the drag of the object
	//@return double The drag of the object
	inline double GetDrag() { return m_drag; }
	//@brief Get the gravity of the object
	//@return double The gravity of the object
	inline double GetGravityMultiplyer() { return m_gravityMultiplier; }


	static inline const double s_offset { 0.01 }; // Offset for grounded check

private:
	glm::dvec3 ApplyDrag();

	glm::dvec3 ApplyRotationalDrag();

	glm::dvec3 m_velocity;						// Component's velocity
	glm::dvec3 m_rotationalVelocity;			// Component's rotational velocity
	glm::dvec3 m_acceleration;					// Component's acceleration
	glm::dvec3 m_rotationalAcceleration;		// Component's rotational acceleration
	double m_mass;								// Component's mass
	double m_inverseMass;						// (1/ mass)	avoids devision in calculations
	double m_drag;								// (0-1) 0 being no drag, 1 being full drag
	double m_rotationalDrag;					// (0-1) 0 being no drag, 1 being full drag
	double m_gravityMultiplier;					// Multiplier for gravity
	double m_bounciness;						// (0-1) 0 being no bounce, 1 being full bounce
	bool m_grounded;							// Is the object grounded

	void defineMember() override
	{
		m_setters["velocity"] = [this](std::any value) { SetVelocity(std::any_cast<glm::dvec3>(value)); };
		m_setters["mass"] = [this](std::any value) { SetMass(std::any_cast<double>(value)); };
		m_setters["drag"] = [this](std::any value) { SetDrag(std::any_cast<double>(value)); };
		m_setters["gravityMultiplier"] = [this](std::any value) { SetGravityMultiplyer(std::any_cast<double>(value)); };
		m_setters["grounded"] = [this](std::any value) { SetGrounded(std::any_cast<bool>(value)); };

		m_getters["velocity"] = [this]() -> std::any { return GetVelocity(); };
		m_getters["mass"] = [this]() -> std::any { return GetMass(); };
		m_getters["drag"] = [this]() -> std::any { return GetDrag(); };
		m_getters["gravityMultiplier"] = [this]() -> std::any { return GetGravityMultiplyer(); };
		m_getters["grounded"] = [this]() -> std::any { return Grounded(); };
	}
};
