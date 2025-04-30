#include "../pch.h"
#include "../VectorCalculations.h"

std::optional<const glm::dvec3> CollisionShape::GetHitNormal(const glm::dvec3 origin, const glm::dvec3 direction) const
{
    std::optional<const glm::dvec3> hitPoint = GetHitPoint(origin, direction);
    if (!hitPoint.has_value()) { return std::nullopt; }
    return GetHitNormal(hitPoint.value());
}

std::optional<const glm::dvec3> CollisionShape::GetHitNormal(const glm::dvec3 hitPoint) const
{
    glm::dvec3 worldPoint = hitPoint;
    glm::dvec3 localPoint = worldPoint - GetPosition(); // Get the point in local space
    glm::dvec3 dirToPoint = glm::normalize(localPoint); // Get the direction to the point
    return GetNormal(dirToPoint);
}

// Use this to replace "glm::dquat(glm::radians(GetRotation()))"
const glm::dquat CollisionShape::GetShapeRotationQuat() const
{
    // Get the rotation quaternion (Using Garrett's weird thing)
    glm::dquat rotationQuat = VectorCalculation::CalculateRotationQuat(GetRotation());
    return rotationQuat;
}

