class CollisionComponent : public Component
{
public:
	// ****** CollisionComponent ****** //

	CollisionComponent() : m_collisionLayer(0), m_collisionMask(0) { Init(); }
	CollisionComponent(int layer, int mask) : m_collisionLayer(layer), m_collisionMask(mask) { Init(); }
	virtual ~CollisionComponent() { Shutdown(); }
	CollisionComponent(const CollisionComponent& other)
		: m_collisionLayer(other.m_collisionLayer), m_collisionMask(other.m_collisionMask)
	{ 
		m_collisionShape = other.m_collisionShape->Clone();
		Init();
	}

	//@brief Initialize the CollisionComponent
	virtual void Init() override;
	//@brief Update the CollisionComponent
	virtual void Update() override;
	//@brief Shutdown the CollisionComponent
	virtual void Shutdown() override;

	// ****** CollisionComponent Management ****** //
	
	//@brief Set the collision shape
	//@param shape : The collision shape to set
	void SetCollisionShape(CollisionShape* shape) { m_collisionShape = std::unique_ptr<CollisionShape>(shape); }
	//@brief Get the collision shape
	//@return CollisionShape* The collision shape
	inline CollisionShape* GetCollisionShape() { return m_collisionShape.get(); }

	//@brief Set the collision layer
	//@param layer : The layer to set
	inline void SetCollisionLayer(const int layer) { m_collisionLayer = layer; }
	//@brief Get the collision layer
	//@return int The collision layer
	inline int GetCollisionLayer() const { return m_collisionLayer; }

	//@brief Set this CollisionComponent's collision mask
	//@param mask : The mask to set
	inline void SetCollisionMask(const int mask) { m_collisionMask = mask; }
	//@brief Get this CollisionComponent's collision mask
	//@return This CollisionComponent's collision mask
	inline const int GetCollisionMask() const { return m_collisionMask; }

	// ****** Collision Detection ****** //

	//@brief Check if this object has matching collision layer/mask with another object 
	//@param other : The other object to check
	//@return bool True if the objects can collide
	bool CanCollideWith(const CollisionComponent* other) const;


	// ****** Casting ****** //
	//@brief Cast a shape between two points and wait until a collision occurs
	glm::dvec3 Cast_LastAvailablePosition(glm::dvec3 startPosition, glm::dvec3 endPosition, glm::dvec3 startRotation, glm::dvec3 endRotation, int iterations);

	//@brief Cast a shape between two points and return the first available position
	glm::dvec3 Cast_FirstAvailablePosition(glm::dvec3 startPosition, glm::dvec3 endPosition, int iterations);

	std::pair<CollisionComponent, CollisionComponent*> Cast_FirstCollision(glm::dvec3 startPosition, glm::dvec3 endPosition, glm::dvec3 startRotation, glm::dvec3 endRotation, int iterations = 1);
private:
	std::unique_ptr<CollisionShape> m_collisionShape;

	int m_collisionLayer;	// The layer this objects exists on
	int m_collisionMask;	// The mask of objects this object can collide with

	void defineMember() override
	{
		m_setters["collisionLayer"] = [this](std::any val) { this->SetCollisionLayer(std::any_cast<int>(val)); };
		m_setters["collisionMask"] = [this](std::any val) { this->SetCollisionMask(std::any_cast<int>(val)); };
		m_setters["collisionShape"] = [this](std::any val) { this->SetCollisionShape(std::any_cast<CollisionShape*>(val)); };

		m_getters["collisionLayer"] = [this]() -> std::any { return this->GetCollisionLayer(); };
		m_getters["collisionMask"] = [this]() -> std::any { return this->GetCollisionMask(); };
		m_getters["collisionShape"] = [this]() -> std::any { return this->GetCollisionShape(); };
	}
};
