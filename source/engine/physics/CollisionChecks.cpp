#include "../pch.h"
#include "../Quaternion.h"

namespace {

	bool OverlapOnAxis(const CollisionShape_Cuboid* cuboid1, const CollisionShape_Cuboid* cuboid2, const glm::dvec3& axis) {

		auto getInterval = [](const CollisionShape_Cuboid* cuboid, const glm::dvec3& axis) {
			std::array<glm::dvec3, 8> vertices = {
				cuboid->GetPosition() + cuboid->GetHalfWidth() * glm::dvec3(1,  1,  1),
				cuboid->GetPosition() + cuboid->GetHalfWidth() * glm::dvec3(1,  1, -1),
				cuboid->GetPosition() + cuboid->GetHalfWidth() * glm::dvec3(1, -1,  1),
				cuboid->GetPosition() + cuboid->GetHalfWidth() * glm::dvec3(1, -1, -1),
				cuboid->GetPosition() + cuboid->GetHalfWidth() * glm::dvec3(-1,  1,  1),
				cuboid->GetPosition() + cuboid->GetHalfWidth() * glm::dvec3(-1,  1, -1),
				cuboid->GetPosition() + cuboid->GetHalfWidth() * glm::dvec3(-1, -1,  1),
				cuboid->GetPosition() + cuboid->GetHalfWidth() * glm::dvec3(-1, -1, -1)
			};

			double min = glm::dot(vertices[0], axis);
			double max = min;
			for (const auto& vertex : vertices) {
				double projection = glm::dot(vertex, axis);
				min = std::min(min, projection);
				max = std::max(max, projection);
			}
			return std::make_pair(min, max);
		};

		auto [min1, max1] = getInterval(cuboid1, axis);
		auto [min2, max2] = getInterval(cuboid2, axis);

		return !(max1 < min2 || max2 < min1);
	}

    std::vector<glm::dvec3> ComputeAxes(const CollisionShape_Cuboid* cuboid1, const CollisionShape_Cuboid* cuboid2) {
        std::vector<glm::dvec3> axes;

		// Define axis-aligned face normals for a cuboid (local space)
		std::vector<glm::dvec3> cuboidAANormals = {
			//glm::dvec3(0, 1, 0),  // Top face
			glm::dvec3(0, -1, 0), // Bottom face
			//glm::dvec3(1, 0, 0),  // Front face
			glm::dvec3(-1, 0, 0), // Back face
			//glm::dvec3(0, 0, 1),  // Left face
			glm::dvec3(0, 0, -1)  // Right face
		};

        // Get face normals for cuboid1
		std::vector<glm::dvec3> cuboid1Normals;
		for (const glm::dvec3& normal : cuboidAANormals) {
			cuboid1Normals.push_back(glm::rotate(glm::dquat(cuboid1->GetRotation()), normal));
		}


        // Get face normals for cuboid2
		std::vector<glm::dvec3> cuboid2Normals;
		for (const glm::dvec3& normal : cuboidAANormals) {
			cuboid2Normals.push_back(glm::rotate(glm::dquat(cuboid2->GetRotation()), normal));
		}

        // Add face normals
        axes.insert(axes.end(), std::begin(cuboid1Normals), std::end(cuboid1Normals));
        axes.insert(axes.end(), std::begin(cuboid2Normals), std::end(cuboid2Normals));

        // Add cross products of edges
        for (const auto& axis1 : cuboid1Normals) {
            for (const auto& axis2 : cuboid2Normals) {
                glm::dvec3 crossAxis = glm::cross(axis1, axis2);
                if (glm::length(crossAxis) > 1e-6) { // Skip small axes
                    axes.push_back(glm::normalize(crossAxis));
                }
            }
        }

        return axes;
    }

	bool SAT(const CollisionShape_Cuboid* cuboid1, const CollisionShape_Cuboid* cuboid2)
	{
		auto axes = ComputeAxes(cuboid1, cuboid2);

		for (const auto& axis : axes) {
			if (!OverlapOnAxis(cuboid1, cuboid2, axis)) 
				{ return false; }
		}
		return true;
	}

	bool AABB(const CollisionShape_Cuboid* cuboid1, const CollisionShape_Cuboid* cuboid2)
	{
		glm::dvec3 thisHalfWidth = cuboid1->GetHalfWidth();
		glm::dvec3 otherHalfWidth = cuboid2->GetHalfWidth();

		glm::dvec3 thisMin = cuboid1->GetPosition() - thisHalfWidth;
		glm::dvec3 thisMax = cuboid1->GetPosition() + thisHalfWidth;

		glm::dvec3 otherMin = cuboid2->GetPosition() - otherHalfWidth;
		glm::dvec3 otherMax = cuboid2->GetPosition() + otherHalfWidth;

		if (thisMax.x < otherMin.x || thisMin.x > otherMax.x) { return false; }
		if (thisMax.y < otherMin.y || thisMin.y > otherMax.y) { return false; }
		if (thisMax.z < otherMin.z || thisMin.z > otherMax.z) { return false; }

		return true;
	}
}

namespace CollisionChecks {

	bool CheckCollisionBetween(const CollisionShape_Cuboid* cuboid1, const CollisionShape_Cuboid* cuboid2)
	{
		// if rot1 has x%90 == 0 and y%90 == 0 and z%90==0 then this is axis aligned
		glm::dvec3 rot1 = cuboid1->GetRotation();
		int x1 = glm::round(rot1.x);
		int y1 = glm::round(rot1.y);
		int z1 = glm::round(rot1.z);
		bool axis_aligned1 = x1 % 90 == 0 && y1 % 90 == 0 && z1 % 90 == 0;
		// if rot2 has x%90 == 0 and y%90 == 0 and z%90==0 then this is axis aligned
		glm::dvec3 rot2 = cuboid2->GetRotation();
		int x2 = glm::round(rot2.x);
		int y2 = glm::round(rot2.y);
		int z2 = glm::round(rot2.z);
		bool axis_aligned2 = x2 % 90 == 0 && y2 % 90 == 0 && z2 % 90 == 0;
		if (axis_aligned1 && axis_aligned2)	// If both are axis aligned
		{
			return AABB(cuboid1, cuboid2);
		}
		else
		{
			return SAT(cuboid1, cuboid2);
		}
	}


	/// <summary>
	/// Check if a CollisionShape_Sphere collides with a CollisionShape_Cuboid.
	/// </summary>
	/// <returns>True if the two shapes are colliding.</returns>
	bool CheckCollisionBetween(const CollisionShape_Cuboid* cuboid, const CollisionShape_Sphere* sphere)
	{
		glm::dvec3 thisHalfWidth = cuboid->GetHalfWidth();
		glm::dvec3 otherPosition = sphere->GetPosition();
		glm::dvec3 thisMin = cuboid->GetPosition() - thisHalfWidth;
		glm::dvec3 thisMax = cuboid->GetPosition() + thisHalfWidth;

		glm::dvec3 closestPoint = glm::dvec3(
			glm::clamp(otherPosition.x, thisMin.x, thisMax.x),
			glm::clamp(otherPosition.y, thisMin.y, thisMax.y),
			glm::clamp(otherPosition.z, thisMin.z, thisMax.z)
		);

		double distance = glm::distance(otherPosition, closestPoint);
		if (distance < sphere->GetRadius().x)						// TODO: Assuming the sphere is a perfect sphere
		{
			return true;
		}
		return false;
	}

	/// <summary>
	/// Check if a CollisionShape_Sphere collides with a CollisionShape_Cuboid.
	/// </summary>
	/// <returns>True if the two shapes are colliding.</returns>
	bool CheckCollisionBetween(const CollisionShape_Sphere* sphere, const CollisionShape_Cuboid* cuboid)
	{
		return CheckCollisionBetween(cuboid, sphere);
	}


	bool CheckCollisionBetween(const CollisionShape_Sphere* sphere1, const CollisionShape_Sphere* sphere2)
	{
		// Check if the distance between the two spheres is less than the sum of their radii
		double distance = glm::distance(sphere1->GetPosition(), sphere2->GetPosition());
		double sumRadii = sphere1->GetRadius().x + sphere2->GetRadius().x;	// TODO: Assuming the spheres are perfect spheres
		if (distance < sumRadii)
		{
			return true;
		}
		return false;
	}

	bool CheckCollisionBetween(const CollisionShape* shape1, const CollisionShape* shape2)
	{
		assert(shape1 != nullptr && shape2 != nullptr);
		if (auto cuboid1 = dynamic_cast<const CollisionShape_Cuboid*>(shape1))
		{
			if (auto cuboid2 = dynamic_cast<const CollisionShape_Cuboid*>(shape2))
			{
				return CheckCollisionBetween(cuboid1, cuboid2);
			}
			else if (auto sphere2 = dynamic_cast<const CollisionShape_Sphere*>(shape2))
			{
				return CheckCollisionBetween(cuboid1, sphere2);
			}
		}
		else if (const CollisionShape_Sphere* sphere1 = dynamic_cast<const CollisionShape_Sphere*>(shape1))
		{
			if (auto cuboid2 = dynamic_cast<const CollisionShape_Cuboid*>(shape2))
			{
				return CheckCollisionBetween(sphere1, cuboid2);
			}
			else if (const CollisionShape_Sphere* sphere2 = dynamic_cast<const CollisionShape_Sphere*>(shape2))
			{
				return CheckCollisionBetween(sphere1, sphere2);
			}
		}
		return false;
	}
}