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

	virtual std::string GetShapeType() const = 0;

protected:
	glm::dvec3 m_position;
	glm::dvec3 m_rotation;
	glm::dvec3 m_scale;

	void defineMember() override {}
};
