#pragma once
class CollisionShape_Cuboid :
    public CollisionShape
{
public:
	CollisionShape_Cuboid() { defineMember(); }
	~CollisionShape_Cuboid() {}
	std::unique_ptr<CollisionShape> Clone() const override { return std::make_unique<CollisionShape_Cuboid>(*this); }

	// Default size is Scale Width = 1, Height = 1, Depth = 1
	// HalfWidth returns scale
    // Width returns scale * 2
	inline glm::dvec3 GetHalfWidth() const { return GetScale(); }
	inline void SetHalfWidth(glm::dvec3 halfWidth) { SetScale(halfWidth); }
	inline void SetHalfWidth(double x, double y, double z) { SetHalfWidth(glm::dvec3(x, y, z)); }
	inline glm::dvec3 GetWidth() const { return  GetScale() * 2.0; }
	inline void SetWidth(glm::dvec3 width) { SetScale(width / 2.0); }

	glm::dvec3 GetNormal(const glm::dvec3& dir) const override;

	//glm::dvec3 GetHitNormal(const glm::dvec3 origin, const glm::dvec3 direction) const override;

	std::optional<glm::dvec3> GetHitPoint(const glm::dvec3 origin, const glm::dvec3 direction) const override;

	inline std::string GetShapeType() const override {
		return Utils::GetClassName<std::remove_pointer_t<decltype(this)>>();
	}

	std::vector<glm::dvec3> GetAxes() const override;
	std::vector<glm::dvec3> GetVertices() const override;
    std::pair<double, double> GetAxisInterval(const glm::dvec3 axis) const override;

	const glm::dvec3 GetEdgePoint(const glm::dvec3 direction) const override;
private:
};

