#pragma once
class RenderComponent;
class CollisionShape : public IHasGettersSetters
{
public:
	CollisionShape() : m_position(0.0), m_rotation(0.0), m_scale(1.0) {}
	virtual ~CollisionShape() {}
	virtual std::unique_ptr<CollisionShape> Clone() const = 0;

	//@brief Set the position of the collision shape
	//@param position : The position to set
	inline void SetPosition(const glm::dvec3& position) { m_position = position; }
	//@brief Set the position of the collision shape
	//@param x : The x component of the position
	//@param y : The y component of the position
	//@param z : The z component of the position
	inline void SetPosition(double x, double y, double z) { m_position = glm::dvec3(x, y, z); }
	//@brief Get the position of the collision shape
	inline glm::dvec3 GetPosition() const { return m_position; }

	//@brief Set the rotation of the collision shape
	inline void SetRotation(const glm::dvec3& rotation) { m_rotation = rotation; }
	//@brief Set the rotation of the collision shape
	//@param x : The x component of the rotation
	//@param y : The y component of the rotation
	//@param z : The z component of the rotation
	inline void SetRotation(double x, double y, double z) { m_rotation = glm::dvec3(x, y, z); }
	//@brief Get the rotation of the collision shape
	inline glm::dvec3 GetRotation() const { return m_rotation; }

	//@brief Set the scale of the collision shape
	inline void SetScale(const glm::dvec3& scale) { m_scale = scale; }
	//@brief Set the scale of the collision shape
	//@param x : The x component of the scale
	//@param y : The y component of the scale
	//@param z : The z component of the scale
	inline void SetScale(double x, double y, double z) { m_scale = glm::dvec3(x, y, z); }
	//@brief Get the scale of the collision shape
	inline glm::dvec3 GetScale() const { return m_scale; }

	//@brief Get the normal of the collision shape
	//@param dir : The direction from shape center to get the normal from
	//@return The normal of the collision shape at the point intercepted by the direction
	virtual glm::dvec3 GetNormal(const glm::dvec3& dir) const = 0;

    //@brief Get the Normal at the hit point of a ray from origin in direction
    //@param origin : The origin of the ray
    //@param direction : The direction of the ray
    //@return The normal at the hit point
	std::optional<const glm::dvec3> GetHitNormal(const glm::dvec3 origin, const glm::dvec3 direction) const;
	//@brief Get the Normal at the hit point
    //@param hitPoint : The hit point in world space
	//@return The normal at the hit point
	std::optional<const glm::dvec3> GetHitNormal(const glm::dvec3 hitPoint) const;
	
    //@brief Get the hit point of the collision shape of a ray from origin in direction
    //@param origin : The origin of the ray
    //@param direction : The direction of the ray
    //@return The hit point of the collision shape in world space
    virtual std::optional<glm::dvec3> GetHitPoint(const glm::dvec3 origin, const glm::dvec3 direction) const = 0;

	//@brief Get the point on the edge of this shape in world space
	//@param direction: The direction to find the intersection, from origin to out of the shape
	//@return The point in world space
	virtual const glm::dvec3 GetEdgePoint(const glm::dvec3 direction) const = 0;
	virtual std::string GetShapeType() const = 0;

    virtual std::vector<glm::dvec3> GetAxes() const = 0;
    virtual std::vector<glm::dvec3> GetVertices() const = 0;
	virtual std::pair<double, double> GetAxisInterval(const glm::dvec3 axis) const = 0;

	const glm::dquat GetShapeRotationQuat() const;
protected:
	glm::dvec3 m_position;
	glm::dvec3 m_rotation;
	glm::dvec3 m_scale;

	void defineMember() override 
	{
		m_setters["position"] = [this](std::any val) { this->SetPosition(std::any_cast<glm::dvec3>(val)); };
		m_setters["rotation"] = [this](std::any val) { this->SetRotation(std::any_cast<glm::dvec3>(val)); };
		m_setters["scale"] = [this](std::any val) { this->SetScale(std::any_cast<glm::dvec3>(val)); };

		m_getters["position"] = [this]() -> std::any { return std::any(this->GetPosition()); };
		m_getters["rotation"] = [this]() -> std::any { return std::any(this->GetRotation()); };
		m_getters["scale"] = [this]() -> std::any { return std::any(this->GetScale()); };
		m_getters["shapeType"] = [this]() -> std::any { return std::any(this->GetShapeType()); };
	}
};