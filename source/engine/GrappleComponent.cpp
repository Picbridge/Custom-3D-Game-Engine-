#include "pch.h"
#include "GrappleComponent.h"
#include "physics/PhysicsComponent.h"
#include <TransformComponent.h>

GrappleComponent::GrappleComponent(double minGrappleLength, double maxGrappleLength, double reelSpeed, int collisionLayer)
    : m_attachPoint(glm::dvec3(0)), m_reelSpeed(reelSpeed), m_attached(false),
      m_grappleLength(0), m_minGrappleLength(minGrappleLength), m_maxGrappleLength(maxGrappleLength),
      m_collisionMask(collisionLayer)
{
    defineMember();
}

GrappleComponent::GrappleComponent() : GrappleComponent(0, 0, 0) {

}

GrappleComponent::~GrappleComponent() {}

void GrappleComponent::Update()
{
    if (!pOwner->HasComponent<PhysicsComponent>())
    {
        printf("Error: GrappleComponent requires a PhysicsComponent to function\n");
        return;
    }
    GrapplePhysics();
}

std::optional<glm::vec3> GrappleComponent::CastGrapple(glm::vec3 grappleDirection)
{
    return CastGrapple(static_cast<glm::vec3>(pOwner->GetComponent<TransformComponent>()->GetPosition()), grappleDirection);
}

std::optional<glm::vec3> GrappleComponent::CastGrapple(glm::vec3 grappleOrigin, glm::vec3 grappleDirection) const
{
    CollisionManager* collisionManager = SERVICE_LOCATOR.GetCollisionManager();
    Raycast ray = collisionManager->CastRay(grappleOrigin, grappleDirection, GetCollisionMask(), GetMaxLength());
    if (!ray.Hit()) { return std::nullopt; }
    if (ray.GetLength() < GetMinLength()) { return std::nullopt; }
    // Check if there is a shape between the grapple and the grapple origin
    Raycast obstructionRay = collisionManager->CastRay(grappleOrigin, grappleDirection, ~1, ray.GetLength());
    if (obstructionRay.Hit() && obstructionRay.GetLength() < ray.GetLength())
    {
        // If there is a shape between the grapple and the grapple origin, return null
        return std::nullopt;
    }
    
    return ray.GetHitPoint();
}

void GrappleComponent::AttachGrapple(glm::vec3 attachPoint)
{
    m_attachPoint = attachPoint;
    m_grappleLength = GetDistanceFromAttachPoint();
    m_attached = true;
}

void GrappleComponent::ReelGrappleIn()
{
    if (!IsAttached()) { return; }
    if (GetGrappleLength() < GetMinLength()) 
    {
        m_grappleLength = GetMinLength();
        return;
    }
    m_grappleLength -= GetReelSpeed() * SERVICE_LOCATOR.GetTime()->GetDeltaTime();
}

void GrappleComponent::ReelGrappleOut()
{
    if (!IsAttached()){ return; }
    if (GetGrappleLength() > GetMaxLength())
    {
        m_grappleLength = GetMaxLength();
        return;
    }
    m_grappleLength += GetReelSpeed() * SERVICE_LOCATOR.GetTime()->GetDeltaTime();
}

glm::dvec3 GrappleComponent::CalculateReelDifference() const
{
    glm::dvec3 grappleDirection = GetGrappleDirection();
    double deltaTime = SERVICE_LOCATOR.GetTime()->GetDeltaTime();
    glm::dvec3 positionDifference = grappleDirection * static_cast<double>(m_reelSpeed) * deltaTime;
    return positionDifference;
}

void GrappleComponent::ReleaseGrapple()
{
    m_attached = false;
}

void GrappleComponent::GrapplePhysics()
{
    if (!this->IsAttached() || !pOwner->HasComponent<PhysicsComponent>()) { return; }
    // If the player is already within the grapple length, don't remove the velocity
    //      The rope shouldn't affect physics if they aren't at the end of the grapple
    //      Since there's slack in the rope
    double distanceFromAttachPoint = GetDistanceFromAttachPoint();
    if (distanceFromAttachPoint < GetGrappleLength()) { return; }
    // If the player is outside the grapple length, move the player to the grapple length
    if (distanceFromAttachPoint > GetGrappleLength())
    {
        glm::dvec3 grappleDirection = GetGrappleDirection();
        glm::dvec3 grapplePosition = static_cast<glm::dvec3>(pOwner->GetComponent<TransformComponent>()->GetPosition());
        glm::dvec3 newPosition = grappleDirection * (distanceFromAttachPoint - GetGrappleLength()) + grapplePosition;
        pOwner->GetComponent<TransformComponent>()->SetPosition(newPosition);
    }

    PhysicsComponent* physicsComponent = pOwner->GetComponent<PhysicsComponent>();
    glm::dvec3 grappleDirection = GetGrappleDirection();

    // A rope removes the velocity in the opposite direction of the grapple
    // Get the player velocity
    glm::dvec3 playerVelocity = physicsComponent->GetVelocity();
    // remove the player velocity opposite the grapple direction
    glm::dvec3 playerVelocityInGrappleDirection = glm::dot(playerVelocity, -grappleDirection) * -grappleDirection;
    glm::dvec3 newPlayerVelocity = playerVelocity - playerVelocityInGrappleDirection;
    // set the player velocity to the new velocity
    physicsComponent->SetVelocity(newPlayerVelocity);
}
