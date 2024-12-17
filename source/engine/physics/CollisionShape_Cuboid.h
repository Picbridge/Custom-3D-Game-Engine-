#pragma once
class CollisionShape_Cuboid :
    public CollisionShape
{
public:
	CollisionShape_Cuboid() { defineMember(); }
	~CollisionShape_Cuboid() {}
	std::unique_ptr<CollisionShape> Clone() const override { return std::unique_ptr<CollisionShape_Cuboid>(new CollisionShape_Cuboid(*this)); }

	// Default size is Width = 1, Height = 1, Depth = 1
	// HalfWidth returns scale / 2
	inline glm::dvec3 GetHalfWidth() const { return GetScale() * 0.5; }
	inline void SetHalfWidth(glm::dvec3 halfWidth) { SetScale(halfWidth * 2.0); }
	inline void SetHalfWidth(double x, double y, double z) { SetScale(glm::dvec3(x, y, z) * 0.5); }
	inline glm::dvec3 GetWidth() const { return  GetScale(); }
	inline void SetWidth(glm::dvec3 width) { SetScale(width); }

	glm::dvec3 GetNormal(const glm::dvec3& dir) const override;

	inline std::string GetShapeType() const override {
		return Utils::GetClassName<std::remove_pointer_t<decltype(*this)>>();
	}
private:
	void defineMember() override
	{
		m_setters["halfWidth"] = [this](std::any val) { this->SetHalfWidth(std::any_cast<glm::vec3>(val)); };

		m_getters["halfWidth"] = [this]() -> std::any { return std::any(this->GetHalfWidth()); };
		m_getters["shapeType"] = [this]() -> std::any { return std::any(this->GetShapeType()); };
	}
};

