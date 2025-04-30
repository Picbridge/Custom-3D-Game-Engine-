#include "../pch.h"
#include "CollisionShape_Cuboid.h"

glm::dvec3 CollisionShape_Cuboid::GetNormal(const glm::dvec3& dir) const
{
    // Get the rotation quaternion
    const glm::dquat rotationQuat = GetShapeRotationQuat();
    // Transform the direction vector to local space
    glm::dvec3 localDir = glm::rotate(glm::inverse(rotationQuat), dir);
    glm::dvec3 scaledDir = localDir / GetHalfWidth();
    // Get the absolute value of the local direction
    glm::dvec3 absLocalDir = glm::abs(scaledDir);

    // Return the direction at the axis with the largest absolute value
    int axis = 0;
    if (absLocalDir.y > absLocalDir.x) axis = 1;
    if (absLocalDir.z > absLocalDir[axis]) axis = 2;
    glm::dvec3 localNormal(0.0);
    localNormal[axis] = glm::sign(localDir[axis]);

    // Transform the normal back to world space
    glm::dvec3 worldNormal = glm::rotate(rotationQuat, localNormal);

    return glm::normalize(worldNormal);
}

std::optional<glm::dvec3> CollisionShape_Cuboid::GetHitPoint(const glm::dvec3 origin, const glm::dvec3 direction) const
{
    glm::dquat rotationQuat = GetShapeRotationQuat();
    glm::dquat invRotationQuat = glm::inverse(rotationQuat);
    // Transform the ray origin and direction to local space
    glm::dvec3 localOrigin = glm::rotate(invRotationQuat,(origin - GetPosition()));
    glm::dvec3 localDirection = glm::rotate(invRotationQuat, glm::normalize(direction));

    // Invert the direction
    double epsilonScaler = 10.0;
    glm::dvec3 epsilon = glm::dvec3(glm::epsilon<double>()) * epsilonScaler;    // have to add a tiny buffer to avoid divide by zero
    glm::dvec3 safeLocalDirection = glm::max(glm::abs(localDirection), epsilon) * glm::sign(localDirection);
    glm::dvec3 invLocalDirection = glm::dvec3(1.0) / safeLocalDirection;
    // Get the half width
    glm::dvec3 halfWidth = GetHalfWidth();
    // Get local min and max
    glm::dvec3 minCorner = -halfWidth;
    glm::dvec3 maxCorner = halfWidth;
    // Get the t values
    glm::dvec3 tMin = (minCorner - localOrigin) * invLocalDirection;
    glm::dvec3 tMax = (maxCorner - localOrigin) * invLocalDirection;
    glm::dvec3 t1 = glm::min(tMin, tMax);
    glm::dvec3 t2 = glm::max(tMin, tMax);
    double tNear = glm::compMax(t1);
    double tFar = glm::compMin(t2);
    // Check if the ray intersects the box
    if (tNear > tFar || (tFar < 0.0 && tNear < 0.0))
    {
        return std::nullopt;
    }
    // Get the hit point in local space
    double t = (tNear < 0.0) ? tFar : tNear;
    glm::dvec3 localHitPoint = localOrigin + localDirection * t;
    //glm::dvec3 localHitPoint = localOrigin + localDirection * tNear;
    // Transform the hit point back to world space
    glm::dvec3 worldHitPoint = glm::rotate(rotationQuat,localHitPoint) + GetPosition();
    // Return the hit point
    return worldHitPoint;
}

std::vector<glm::dvec3> CollisionShape_Cuboid::GetAxes() const
{
    // Local space unit axes (X, Y, Z)
    std::vector<glm::dvec3> localAxes = {
        glm::dvec3(1, 0, 0),  // X-axis
        glm::dvec3(0, 1, 0),  // Y-axis
        glm::dvec3(0, 0, 1),  // Z-axis
    };

    // Get rotation
    glm::dquat rotationQuat = GetShapeRotationQuat();

    // Get non-uniform scale (half-widths)
    glm::dvec3 halfWidth = GetHalfWidth();

    // Transform axes considering rotation and scale
    std::vector<glm::dvec3> worldAxes;
    for (const glm::dvec3& axis : localAxes)
    {
        glm::dvec3 rotatedAxis = rotationQuat * axis; // Rotate
        glm::dvec3 scaledAxis = rotatedAxis * halfWidth; // Apply scale
        worldAxes.push_back(glm::normalize(scaledAxis)); // Normalize to maintain unit length
    }

    return worldAxes;
}

std::vector<glm::dvec3> CollisionShape_Cuboid::GetVertices() const
{
    // Get the half width
    glm::dvec3 halfWidth = GetHalfWidth();
    // Get the position
    glm::dvec3 position = GetPosition();
    // Get the rotation
    glm::dquat rotationQuat = GetShapeRotationQuat();
    // Get the vertices
    std::vector<glm::dvec3> vertices = {
        position + glm::rotate(rotationQuat, halfWidth * glm::dvec3(1,  1,  1)),
        position + glm::rotate(rotationQuat, halfWidth * glm::dvec3(1,  1, -1)),
        position + glm::rotate(rotationQuat, halfWidth * glm::dvec3(1, -1,  1)),
        position + glm::rotate(rotationQuat, halfWidth * glm::dvec3(1, -1, -1)),
        position + glm::rotate(rotationQuat, halfWidth * glm::dvec3(-1,  1,  1)),
        position + glm::rotate(rotationQuat, halfWidth * glm::dvec3(-1,  1, -1)),
        position + glm::rotate(rotationQuat, halfWidth * glm::dvec3(-1, -1,  1)),
        position + glm::rotate(rotationQuat, halfWidth * glm::dvec3(-1, -1, -1))
    };
    return vertices;
}

std::pair<double, double> CollisionShape_Cuboid::GetAxisInterval(const glm::dvec3 axis) const
{
    assert(axis != glm::dvec3(0.0) && "Axis cannot be zero vector");
    // Get the vertices
    std::vector<glm::dvec3> vertices = GetVertices();
    double min = glm::dot(vertices[0], axis);
    double max = min;
    // Get the min and max vertices along this axis
    for (const auto& vertex : vertices) {
        double projection = glm::dot(vertex, axis);
        min = std::min(min, projection);
        max = std::max(max, projection);
    }
    return std::make_pair(min, max);
}

const glm::dvec3 CollisionShape_Cuboid::GetEdgePoint(const glm::dvec3 direction) const
{
    const glm::dquat rotationQuat = GetShapeRotationQuat();
    const glm::dvec3 localDirection = glm::inverse(rotationQuat) * glm::normalize(direction);

    glm::dvec3 localEdgePoint = glm::sign(localDirection) * GetHalfWidth();
    glm::dvec3 worldEdgePoint = GetPosition() + (rotationQuat * localEdgePoint);
    return worldEdgePoint;
}
