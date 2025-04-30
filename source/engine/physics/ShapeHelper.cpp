#include "../pch.h"
#include "ShapeHelper.h"

namespace ShapeHelper
{
	bool IfAxisAligned (const CollisionShape* shape)
	{
		glm::dvec3 rot = shape->GetRotation();
		int x = glm::round(rot.x);
		int y = glm::round(rot.y);
		int z = glm::round(rot.z);
		return x % 90 == 0 && y % 90 == 0 && z % 90 == 0;
	}
	
	bool IfCuboid(const CollisionShape* shape)
	{
		return dynamic_cast<const CollisionShape_Cuboid*>(shape) != nullptr;
	}

	bool IfSphere(const CollisionShape* shape)
	{
		return dynamic_cast<const CollisionShape_Sphere*>(shape) != nullptr;
	}
};
