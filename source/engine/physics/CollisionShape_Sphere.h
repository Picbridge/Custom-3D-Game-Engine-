#pragma once

class CollisionShape_Sphere : public CollisionShape
{
public:
	CollisionShape_Sphere() { defineMember(); }
	~CollisionShape_Sphere() {}
	std::unique_ptr<CollisionShape> Clone() const override { return std::make_unique<CollisionShape_Sphere>(*this); }

	inline glm::dvec3 GetRadius() const { return GetScale(); }
	inline void SetRadius(glm::dvec3 radius) { SetScale(radius); }
	inline void SetRadius(double x, double y, double z) { SetRadius(glm::dvec3(x, y, z)); }
	inline void SetRadius(double radius) { SetRadius(glm::dvec3(radius)); }

	inline glm::dvec3 GetDiameter() const { return GetScale() * 2.0; }
	inline void SetDiameter(glm::dvec3 diameter) { SetScale(diameter / 2.0); }
	inline void SetDiameter(double x, double y, double z) { SetDiameter(glm::dvec3(x, y, z)); }
	inline void SetDiameter(double diameter) { SetDiameter(glm::dvec3(diameter)); }
	
	inline std::string GetShapeType() const override {
		return Utils::GetClassName<std::remove_pointer_t<decltype(this)>>();
	}

	inline glm::dvec3 GetNormal(const glm::dvec3& dir) const override { return glm::normalize(dir); }
	std::optional<glm::dvec3> GetHitPoint(const glm::dvec3 origin, const glm::dvec3 direction) const override;
	const glm::dvec3 GetEdgePoint(const glm::dvec3 direction) const override;
	std::vector<glm::dvec3> GetAxes() const override;
    std::vector<glm::dvec3> GetVertices() const override;
	std::pair<double, double> GetAxisInterval(const glm::dvec3 axis) const;
private:
};

