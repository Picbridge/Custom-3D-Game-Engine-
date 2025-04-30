#include "../pch.h"

std::optional<glm::dvec3> CollisionShape_Sphere::GetHitPoint(const glm::dvec3 origin, const glm::dvec3 direction) const
{
    // move into local space
    glm::dvec3 localOrigin = origin - GetPosition();
    double radius = GetRadius().x; // Assuming uniform radius in all directions
    // solve for discriminant
    glm::dvec3 normDir = glm::normalize(direction);
    double a = glm::dot(normDir, normDir);
    double b = 2.0 * glm::dot(localOrigin, normDir);
    double c = glm::dot(localOrigin, localOrigin) - pow(radius,2);
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        return std::nullopt;
    }
    double t0 = (-b - glm::sqrt(discriminant)) / (2.0 * a);
    double t1 = (-b + glm::sqrt(discriminant)) / (2.0 * a);
    double t = (t0 > 0) ? t0 : t1; // take the first positive root
    if (t < 0) { return std::nullopt; } // no hit
    // return to world space
    return origin + normDir * t;
}

const glm::dvec3 CollisionShape_Sphere::GetEdgePoint(const glm::dvec3 direction) const
{
    return GetPosition() + (glm::normalize(direction) * GetRadius().x);
}

std::vector<glm::dvec3> CollisionShape_Sphere::GetAxes() const
{
    return std::vector<glm::dvec3>();
}

std::vector<glm::dvec3> CollisionShape_Sphere::GetVertices() const
{
    return std::vector<glm::dvec3> { GetPosition() };
}

std::pair<double, double> CollisionShape_Sphere::GetAxisInterval(const glm::dvec3 axis) const
{
    assert(axis != glm::dvec3(0.0) && "Axis cannot be zero vector");
    // Normalize the axis to ensure accurate projection
    glm::dvec3 normalizedAxis = glm::normalize(axis);

    // Project the center of the sphere onto the axis
    double centerProjection = glm::dot(GetPosition(), normalizedAxis);

    // Extend the projection by the radius in both directions
    double radius = GetRadius().x; // TODO Lane: Assuming uniform radius in all directions
    double minInterval = centerProjection - radius;
    double maxInterval = centerProjection + radius;

    return std::make_pair(minInterval, maxInterval);
}

