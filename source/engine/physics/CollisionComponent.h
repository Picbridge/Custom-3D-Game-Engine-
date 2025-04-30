#pragma once
class Component;
#include "PhysicsComponent.h"

class CollisionComponent : public Component
{
public:
	// ****** CollisionComponent ****** //

	CollisionComponent() : CollisionComponent(0,0,false)
	{
		defineMember();
	}
	CollisionComponent(int layer, int mask, bool isTrigger = false) : m_collisionLayer(layer), m_collisionMask(mask), m_isTrigger(isTrigger) {}
	CollisionComponent(const CollisionComponent& other)
        : m_collisionLayer(other.m_collisionLayer), m_collisionMask(other.m_collisionMask), m_isTrigger(other.m_isTrigger)
	{ 
		m_collisionShape = other.m_collisionShape->Clone();
	}
	virtual ~CollisionComponent() {}

	CollisionComponent& operator=(const CollisionComponent& other)
	{
		if (this == &other) { return *this; }
		m_collisionLayer = other.m_collisionLayer;
		m_collisionMask = other.m_collisionMask;
		m_collisionShape = other.m_collisionShape->Clone();
        m_isTrigger = other.m_isTrigger;
		return *this;
	}

	//@brief Initialize the CollisionComponent
	virtual void Init() override;
	//@brief Update the CollisionComponent
	virtual void Update() override;
	void MatchTransform();
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

    //@brief Set this CollisionComponent as a trigger
    //@param isTrigger : The trigger state to set
    inline void SetTrigger(const bool isTrigger) { m_isTrigger = isTrigger; }
    //@brief Get this CollisionComponent's trigger state
    //@return This CollisionComponent's trigger state
    inline const bool IsTrigger() const { return m_isTrigger; }

    inline const bool IsDynamic() const { return pOwner->HasComponent<PhysicsComponent>(); }

	// ****** Collision Detection ****** //

	//@brief Check if this object has matching collision layer/mask with another object 
	//@param other : The other object to check
	//@return bool True if the objects can collide
	bool CanCollideWith(const CollisionComponent* other) const;


	// ****** Casting ****** //
	//@brief Cast a shape between two points and wait until a collision occurs
    //@param startPosition : The start position of the cast
    //@param endPosition : The end position of the cast
    //@param startRotation : The start rotation of the cast
    //@param endRotation : The end rotation of the cast
    //@param iterations : The number of iterations to cast
    //@return glm::dvec3 The last available position
	CollisionComponent Cast_LastAvailablePosition(glm::dvec3 startPosition, glm::dvec3 endPosition, glm::dvec3 startRotation, glm::dvec3 endRotation, int iterations);

	//@brief Cast a shape between two points and return the first available position
    //@param startPosition : The start position of the cast
    //@param endPosition : The end position of the cast
    //@param iterations : The number of iterations to cast
    //@return glm::dvec3 The first available position
	glm::dvec3 Cast_FirstAvailablePosition(glm::dvec3 startPosition, glm::dvec3 endPosition, int iterations);

    //@brief Cast a shape between two points and return the first collision
    //@param startPosition : The start position of the cast
    //@param endPosition : The end position of the cast
    //@param startRotation : The start rotation of the cast
    //@param endRotation : The end rotation of the cast
    //@param iterations : The number of iterations to cast
    //@return first: A copy of this collision component moved into the first collision
    //@return second: A pointer to the collision component that was collided with
	std::pair<CollisionComponent, CollisionComponent*> Cast_FirstCollision(glm::dvec3 startPosition, glm::dvec3 endPosition, glm::dvec3 startRotation, glm::dvec3 endRotation, int iterations = 1) const;
    std::pair<CollisionComponent, CollisionComponent*> Cast_FirstCollision(glm::dvec3 endPosition, glm::dvec3 endRotation, int iterations = 1) const;
private:
	std::unique_ptr<CollisionShape> m_collisionShape;

	int m_collisionLayer;	// The layer this objects exists on
	int m_collisionMask;	// The mask of objects this object can collide with

    bool m_isTrigger;		// Is this object a trigger

	void defineMember() override
	{
		m_setters["collisionLayer"] = [this](std::any val) { this->SetCollisionLayer(std::any_cast<int>(val)); };
		m_setters["collisionMask"] = [this](std::any val) { this->SetCollisionMask(std::any_cast<int>(val)); };
		m_setters["collisionShape"] = [this](std::any val) { this->SetCollisionShape(std::any_cast<CollisionShape*>(val)); };
		m_setters["IsTrigger"] = [this](std::any val) { this->SetTrigger(std::any_cast<bool>(val)); };

		m_getters["collisionLayer"] = [this]() -> std::any { return this->GetCollisionLayer(); };
		m_getters["collisionMask"] = [this]() -> std::any { return this->GetCollisionMask(); };
		m_getters["collisionShape"] = [this]() -> std::any { return this->GetCollisionShape(); };
		m_getters["IsTrigger"] = [this]() -> std::any { return this->IsTrigger(); };

	}
};
