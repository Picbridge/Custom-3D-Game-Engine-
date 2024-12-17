#pragma once
class CollisionShape_Sphere : public CollisionShape
{
public:
	CollisionShape_Sphere() { defineMember(); }
	~CollisionShape_Sphere() {}
	std::unique_ptr<CollisionShape> Clone() const override { return std::unique_ptr<CollisionShape_Sphere>(new CollisionShape_Sphere(*this)); }

	inline glm::dvec3 GetRadius() const { return m_scale * 0.5; }
	inline void SetRadius(glm::dvec3 radius) { m_scale = glm::dvec3(radius * 0.5); }
	inline void SetRadius(double x, double y, double z) { m_scale = glm::dvec3(x, y, z) * 0.5; }
	inline void SetRadius(double radius) { m_scale = glm::dvec3(radius * 0.5); }
	inline glm::dvec3 GetDiameter() const { return m_scale; }
	inline void SetDiameter(glm::dvec3 diameter) { m_scale = diameter; }
	inline void SetDiameter(double x, double y, double z) { m_scale = glm::dvec3(x, y, z); }
	inline void SetDiameter(double diameter) { m_scale = glm::dvec3(diameter); }

	inline std::string GetShapeType() const override {
		return Utils::GetClassName<std::remove_pointer_t<decltype(*this)>>();
	}
	inline glm::dvec3 GetNormal(const glm::dvec3& dir) const override { return glm::normalize(dir); }
private:
	void defineMember() override
	{
		m_setters["radius"] = [this](std::any val) { this->SetRadius(std::any_cast<double>(val)); };

		m_getters["radius"] = [this]() -> std::any { return std::any(this->GetRadius());};
		m_getters["shapeType"] = [this]() -> std::any { return std::any(this->GetShapeType()); };
	}
};

