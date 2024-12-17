#include "../pch.h"
#include "CollisionShape_Cuboid.h"

glm::dvec3 CollisionShape_Cuboid::GetNormal(const glm::dvec3& dir) const
{

	glm::dvec3 normal(0.0);
	// Get the absolute direction to compare for the largest component
	glm::dvec3 absDir = glm::abs(dir);

	if (absDir.x > absDir.y && absDir.x > absDir.z)
	{
		normal.x = glm::sign(dir.x);
	}
	else if (absDir.y > absDir.x && absDir.y > absDir.z)
	{
		normal.y = glm::sign(dir.y);
	}
	else
	{
		normal.z = glm::sign(dir.z);
	}

	return normal;
}