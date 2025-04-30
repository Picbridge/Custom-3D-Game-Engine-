#pragma once

class CollisionComponent;

class PhysicsComponent : public Component
{
public:

	PhysicsComponent(double mass = 1, double gravityMultiplyer = 1, double bounciness = 0, 
		double drag = 0, double rotationalDrag = 0, double friction = 0);

	~PhysicsComponent();

	//-------------------
	//Component essentials
	//-------------------

	//@brief Initialize the component
	void Init() override;
	//@brief Update the component
	void Update() override;
	//@brief Update the component
	void Update(double deltaTime);
	//@brief Shutdown the component
	void Shutdown() override;

	//-------------------
	// PhysicsComponent essentials
	//-------------------
	//@brief Manage object reaction to being grounded
	void GroundedResponse(double deltaTime);
    //@brief Manage object reaction to being ungrounded
    void UngroundedResponse(double deltaTime);
    //@brief Manage object transition to being grounded
    void TransitionToGround();
	//@brief Expensive check if the object is grounded
    bool CheckForGround();
	//@brief Cheap check if the object is still grounded
	bool StillGrounded();
	//@brief Apply a force to the object
	//@param force : The force to apply
	inline void ApplyForce(const glm::dvec3 force) { m_acceleration += force * GetInverseMass(); }
	//@brief Apply a force to the object
	//@param x : The x component of the force
	//@param y : The y component of the force
	//@param z : The z component of the force
	inline void ApplyForce(double x, double y, double z) { ApplyForce(glm::dvec3(x, y, z)); }
	//@brief Apply a torque to the object
	// @param torque : The torque to apply
	inline void ApplyTorque(const glm::dvec3& torque) { m_rotationalAcceleration += torque; }
	//@brief Apply a torque to the object
	//@param x : The x component of the torque
	//@param y : The y component of the torque
	//@param z : The z component of the torque
	inline void ApplyTorque(double x, double y, double z) { ApplyTorque(glm::dvec3(x, y, z)); }

	//--------------------------------
	// Setters
	//--------------------------------

	//@brief Set the grounded state of the object
	//@param grounded : The grounded state to set
	inline void SetGrounded(bool grounded) { /*m_grounded = grounded;*/ }
	//@brief Set the velocity of the object
	//@param velocity : The velocity to set
	inline void SetVelocity(const glm::dvec3 velocity) { m_velocity = velocity; }
	//@brief Set the velocity of the object
	//@param x : The x component of the velocity
	//@param y : The y component of the velocity
	//@param z : The z component of the velocity
	inline void SetVelocity(double x, double y, double z) { SetVelocity(glm::dvec3(x, y, z)); }
    //@brief Set the rotational velocity of the object
    inline void SetRotationalVelocity(const glm::dvec3 velocity) { m_rotationalVelocity = velocity; }
	//@brief Set the mass of the object
	//@param mass : The mass to set
	inline void SetMass(double mass) { m_mass = mass; m_inverseMass = 1 / mass; }
    //@brief Set the friction coefficient of the object
	//@param friction : The friction coefficient to set
    inline void SetFrictionCoefficient(double friction) { m_frictionCoefficient = friction; }
	//@brief Set the drag of the object
	//@param drag : The drag to set
	inline void SetDrag(double drag) { m_dragCoefficient = drag; }
	//@brief Set the gravity of the object
	//@param gravity : The gravity to set
	inline void SetGravityMultiplyer(double gravity) { m_gravityMultiplier = gravity; }

	//--------------------------------
	// Getters
	//--------------------------------

	//@brief Get the grounded state of the object
	//@return bool The grounded state of the object
	inline bool		  Grounded()	              const { return m_groundedOn != nullptr; }
	inline CollisionShape* GetGroundedCollider()  const { return m_groundedOn; }
	 glm::dvec3 GetGroundedNormal() const;
	//@brief Get the velocity of the object
	//@return glm::dvec3 The velocity of the object
	inline glm::dvec3 GetVelocity()               const { return m_velocity; }
    //@brief Get the rotational velocity of the object
    //@return glm::dvec3 The rotational velocity of the object
    inline glm::dvec3 GetRotationalVelocity()     const { return m_rotationalVelocity; }
    //@brief Get the acceleration of the object
    //@return glm::dvec3 The acceleration of the object
    inline glm::dvec3 GetAcceleration()           const { return m_acceleration; }
    //@brief Get the rotational acceleration of the object
    //@return glm::dvec3 The rotational acceleration of the object
    inline glm::dvec3 GetRotationalAcceleration() const { return m_rotationalAcceleration; }
	//@brief Get the mass of the object
	//@return double The mass of the object
	inline double	  GetMass()		              const { return m_mass; }
	//@brief Get the inverse mass of the object
	// @return double The inverse mass of the object
	inline double	  GetInverseMass()            const { return m_inverseMass; }
    //@brief Get the friction coefficient of the object
    //@return double The friction coefficient of the object
    inline double	  GetFrictionCoefficient()    const { return m_frictionCoefficient; }
	//@brief Get the drag of the object
	//@return double The drag of the object
	inline double	  GetDrag()		              const { return m_dragCoefficient; }
    //@brief Get the rotational drag of the object
    //@return double The rotational drag of the object
    inline double	  GetRotationalDrag()         const { return m_rotationalDragCoefficient; }
	//@brief Get the gravity of the object
	//@return double The gravity of the object
	inline double	  GetGravityMultiplyer()      const { return m_gravityMultiplier; }
    //@brief Get the bounciness of the object
    //@return double The bounciness of the object
    inline double	  GetBounciness()             const { return m_bounciness; }


	static inline const double s_grounded_offset { 0.15 }; // Offset for grounded check
private:
	void ApplyGravity(double deltaTime);
	void ApplyFriction(double deltaTime);
	void ApplyDrag(double deltaTime);
	void ApplyRotationalDrag(double deltaTime);

    inline void SetAcceleration			  (const glm::dvec3 acceleration) { m_acceleration = acceleration; }
    inline void SetRotationalAcceleration (const glm::dvec3 acceleration) { m_rotationalAcceleration = acceleration; }

	glm::dvec3		m_velocity;					 // Component's velocity
	glm::dvec3		m_rotationalVelocity;		 // Component's rotational velocity
	glm::dvec3		m_acceleration;				 // Component's acceleration
	glm::dvec3		m_rotationalAcceleration;	 // Component's rotational acceleration
	double			m_mass;						 // Component's mass
	double			m_inverseMass;				 // (1/ mass)	avoids devision in calculations
    double			m_frictionCoefficient;		 // (0-1) 0 being no friction, 1 being full friction
	double			m_dragCoefficient;			 // (0-1) 0 being no drag, 1 being full drag in the air
	double			m_rotationalDragCoefficient; // (0-1) 0 being no drag, 1 being full drag
	double			m_gravityMultiplier;		 // Multiplier for gravity
    double			m_bounciness;				 // (0-1) 0 being no bounce, 1 being full bounce || coefficient of restitution
    CollisionShape* m_groundedOn;				 // The shape the component is grounded on

	void defineMember() override
	{
		m_setters["velocity"]			= [this](std::any value) { SetVelocity(std::any_cast<glm::dvec3>(value)); };
		m_setters["mass"]				= [this](std::any value) { SetMass(std::any_cast<double>(value)); };
		m_setters["drag"]				= [this](std::any value) { SetDrag(std::any_cast<double>(value)); };
        m_setters["gravityMultiplier"]  = [this](std::any value) { SetGravityMultiplyer(std::any_cast<double>(value)); };
		m_setters["frictionCoefficient"]= [this](std::any value) { SetFrictionCoefficient(std::any_cast<double>(value)); };

		m_getters["velocity"]			= [this]() -> std::any   { return GetVelocity(); };
		m_getters["mass"]				= [this]() -> std::any   { return GetMass(); };
		m_getters["drag"]				= [this]() -> std::any   { return GetDrag(); };
		m_getters["gravityMultiplier"]  = [this]() -> std::any   { return GetGravityMultiplyer(); };
        m_getters["frictionCoefficient"]= [this]() -> std::any  { return GetFrictionCoefficient(); };
	}
};
