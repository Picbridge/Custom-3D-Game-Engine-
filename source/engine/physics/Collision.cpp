#include "../pch.h"
#include "Collision.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "TransformComponent.h"

Collision::Collision(GameObject* object1, GameObject* object2)
    :   m_object1(object1), m_object2(object2), m_collisionNormal(glm::dvec3()),
        m_contactPoints(std::vector<glm::dvec3>()), m_penetrationDepth(INFINITY),
        m_colliding(false), m_resolved(false), m_dynamicCollision(false)
{

    // First check if these are colliding
    CheckForCollision();
    if (!IsColliding()) { return; }

    // Calculate the Data to help resolve the collision
    FillCollisionData();
}

void Collision::ResolveCollision()
{
    // If the collision is already resolved, return
    assert(IsColliding() && "Objects not colliding");
    assert(!IsResolved() && "Collision already resolved");
    if (IsResolved()) { return; }
    if (this->IsDynamicCollision())
    {
        // TODO Lane: Implement Dynamic Collision
        std::cerr << "@@ Dynamic Collision not coded yet @@" << std::endl;
    }
    //If this is a static/dynamic collision, only move the dynamic object
    else
    {
        CollisionComponent* dynamicCollider = GetDynamicComponent();
        CollisionComponent* staticCollider = GetStaticComponent();

        // Ensure the normal is pointing from static to dynamic collider
        glm::dvec3 direction = dynamicCollider->GetCollisionShape()->GetPosition() - staticCollider->GetCollisionShape()->GetPosition();
        if (glm::dot(GetCollisionNormal(), direction) < 0) { SetCollisionNormal(-GetCollisionNormal()); } // if the dot product is negative, the normal is facing towards the static collider. Flip it

        glm::dvec3 newPosition = dynamicCollider->GetCollisionShape()->GetPosition();
        newPosition += GetCollisionNormal() * GetPenetrationDepth();
        dynamicCollider->GetCollisionShape()->SetPosition(newPosition);
        GetDynamicObject()->GetComponent<TransformComponent>()->SetPosition(newPosition);
        SetResolved(true);
    }
}

CollisionComponent* Collision::GetDynamicComponent() const
{
    if (GetCollider1()->IsDynamic()) { return GetCollider1(); }
    if (GetCollider2()->IsDynamic()) { return GetCollider2(); }
    return nullptr;
}

CollisionComponent* Collision::GetStaticComponent() const
{
    if (!(GetCollider1()->IsDynamic())) { return GetCollider1(); }
    if (!(GetCollider2()->IsDynamic())) { return GetCollider2(); }
    return nullptr;
}

void Collision::FillCollisionData()
{
    // Set if this is a trigger collision
    bool isTrigger = GetCollider1()->IsTrigger() || GetCollider2()->IsTrigger();
    if (isTrigger)
    {
        SetIsTrigger(isTrigger);
        SetResolved(true);
        return;
    }
    // Set if this is purely a dynamic collision
    bool isDynamic = GetCollider1()->IsDynamic() && GetCollider2()->IsDynamic();
    SetDynamicCollision(isDynamic);
}

bool Collision::CheckForCollision()
{

    // Check if the colliders are colliding
    bool colliding = false;
    CollisionShape* shape1 = GetCollider1()->GetCollisionShape();
    CollisionShape* shape2 = GetCollider2()->GetCollisionShape();
    // Spheres are weird
    if (shape1->GetShapeType() == "CollisionShape_Sphere" && shape2->GetShapeType() == "CollisionShape_Sphere")
    {
        colliding = SphereToSphere();
    }
    else if (shape1->GetShapeType() == "CollisionShape_Sphere" || shape2->GetShapeType() == "CollisionShape_Sphere")
    {
        colliding = SphereToCuboid();
    }
    else
    {
        colliding = SAT();
    }
    SetColliding(colliding);
    return colliding;
}

bool Collision::SAT()
{

    // Get the collision shapes
    CollisionShape* shape1 = GetCollider1()->GetCollisionShape();
    CollisionShape* shape2 = GetCollider2()->GetCollisionShape();
    {
        std::vector<glm::dvec3> axes;
        std::vector<glm::dvec3> axes1 = GetAxes(shape1);
        axes.insert(axes.end(), axes1.begin(), axes1.end());
        std::vector<glm::dvec3> axes2 = GetAxes(shape2);
        axes.insert(axes.end(), axes2.begin(), axes2.end());

        // Add cross products of axes
        for (const auto& axis1 : axes1) {
            for (const auto& axis2 : axes2) {
                glm::dvec3 crossAxis = glm::cross(axis1, axis2);
                if (glm::length(crossAxis) > 1e-6) { // Skip small axes
                    axes.push_back(glm::normalize(crossAxis));
                }
            }
        }

        for (const auto& axis : axes)
        {
            auto [min1, max1] = shape1->GetAxisInterval(axis);
            auto [min2, max2] = shape2->GetAxisInterval(axis);

            // Found a seperating axis
            if (max1 < min2 || max2 < min1) { return false; }

            // Get the overlap
            double overlap = std::min(max1, max2) - std::max(min1, min2);
            if (overlap < GetPenetrationDepth())
            {
                SetPenetrationDepth(overlap);
                SetCollisionNormal(axis);
            }
        }
        return true;
    }
}

bool Collision::SphereToCuboid()
{
    // Get the collision shapes
    CollisionShape* shape1 = GetCollider1()->GetCollisionShape();
    CollisionShape* shape2 = GetCollider2()->GetCollisionShape();
    CollisionShape_Sphere* sphere = static_cast<CollisionShape_Sphere*>((shape1->GetShapeType() == "CollisionShape_Sphere") ? shape1 : shape2);
    CollisionShape_Cuboid* cuboid = static_cast<CollisionShape_Cuboid*>((shape1->GetShapeType() == "CollisionShape_Sphere") ? shape2 : shape1);
    double radius = sphere->GetRadius().x; // Assuming uniform radius in all directions
    if (radius <= 0.0) { return false; }

    glm::dvec3 spherePos = sphere->GetPosition();
    glm::dvec3 cuboidPos = cuboid->GetPosition();

    // Inverse Cuboid rotation
    glm::dquat cuboidRotation = cuboid->GetShapeRotationQuat();
    glm::dquat inverseCuboidRotation = glm::inverse(cuboidRotation);
    glm::dvec3 sphereLocalPos = inverseCuboidRotation * (spherePos - cuboidPos);

    glm::dvec3 halfExtents = cuboid->GetHalfWidth();
    glm::dvec3 closestPointLocal;
    closestPointLocal.x = glm::clamp(sphereLocalPos.x, -halfExtents.x, halfExtents.x);
    closestPointLocal.y = glm::clamp(sphereLocalPos.y, -halfExtents.y, halfExtents.y);
    closestPointLocal.z = glm::clamp(sphereLocalPos.z, -halfExtents.z, halfExtents.z);
    glm::dvec3 closestPoint = cuboidPos + cuboidRotation * closestPointLocal;
    glm::dvec3 direction = spherePos - closestPoint;
    
    
    // if sphere is inside the cuboid
    double distanceSquared = glm::length2(direction);
    if (distanceSquared < glm::epsilon<double>())
    {
        glm::dvec3 distancesToFaces = halfExtents - glm::abs(sphereLocalPos);
        int maxAxis = 0;
        double minDistance = distancesToFaces.x;

        if (distancesToFaces.y < minDistance) {
            maxAxis = 1;
            minDistance = distancesToFaces.y;
        }
        if (distancesToFaces.z < minDistance) {
            maxAxis = 2;
            minDistance = distancesToFaces.z;
        }

        glm::dvec3 normal = glm::dvec3(0.0);
        normal[maxAxis] = (sphereLocalPos[maxAxis] >= 0.0) ? 1.0 : -1.0;

        glm::dvec3 normalWorld = cuboidRotation * normal;
        double penetrationDepth = radius  + minDistance;
        if (penetrationDepth < glm::epsilon<double>()) { return false; }
        SetCollisionNormal(normalWorld);
        SetPenetrationDepth(penetrationDepth);
        return true;
    }
    // Check if the distance is less than the radius
    double radiusSquared = radius * radius; // Assuming uniform radius in all directions
    if (distanceSquared < radiusSquared)
    {
        double penetrationDepth = radius - glm::sqrt(distanceSquared);
        if (penetrationDepth < glm::epsilon<double>()) { return false; }
        SetCollisionNormal(cuboid->GetNormal(direction));
        SetPenetrationDepth(penetrationDepth);
        return true;
    }
    
    return false;
}

bool Collision::SphereToSphere()
{
    CollisionShape* shape1 = GetCollider1()->GetCollisionShape();
    CollisionShape* shape2 = GetCollider2()->GetCollisionShape();

    CollisionShape_Sphere* sphere1 = static_cast<CollisionShape_Sphere*>(shape1);
    CollisionShape_Sphere* sphere2 = static_cast<CollisionShape_Sphere*>(shape2);

    glm::dvec3 pos1 = sphere1->GetPosition();
    glm::dvec3 pos2 = sphere2->GetPosition();
    glm::dvec3 delta = pos2 - pos1;
    double distanceSquared = glm::length2(delta);
    double radiusSum = sphere1->GetRadius().x + sphere2->GetRadius().x;

    if (distanceSquared < pow(radiusSum,2))
    {
        double distance = glm::sqrt(distanceSquared);
        glm::dvec3 normal = (distance > 0.0) ? delta / distance : glm::dvec3(0.0, 1.0, 0.0); // Avoid NaN
        SetCollisionNormal(normal);
        SetPenetrationDepth(radiusSum - distance);
        return true;
    }
    return false;
}

std::vector<glm::dvec3> Collision::GetAxes(const CollisionShape* shape) const
{
    //std::vector<glm::dvec3> axes;
    //if (shape->GetShapeType() == "CollisionShape_Sphere")
    //{
    //    // If the shape is a sphere, return the axis between the two shapes
    //    CollisionShape* shape1 = this->GetCollider1()->GetCollisionShape();
    //    CollisionShape* shape2 = this->GetCollider2()->GetCollisionShape();
    //    glm::dvec3 vecBetween = shape2->GetPosition() - shape1->GetPosition();
    //    axes.push_back(glm::normalize(vecBetween));
    //}
    //else
    //{
    //    // If the shape is not a sphere, return the axes of the shape
    //    axes = shape->GetAxes();
    //}
    return shape->GetAxes();
}
