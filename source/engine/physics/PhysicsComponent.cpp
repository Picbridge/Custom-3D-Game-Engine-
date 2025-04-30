#include "../pch.h"
#include "PhysicsComponent.h"
class Component;
#include "CollisionComponent.h"
#include "PhysicsManager.h"

PhysicsComponent::PhysicsComponent(double mass, double gravityMultiplyer, double bounciness, double drag, double rotationalDrag, double friction) : Component(),
	m_velocity(glm::dvec3(0)), m_rotationalVelocity(glm::dvec3(0)),
	m_acceleration(glm::dvec3(0)), m_rotationalAcceleration(glm::dvec3(0)), m_mass(mass), m_inverseMass(1 / m_mass),
    m_frictionCoefficient(friction), m_dragCoefficient(drag), m_rotationalDragCoefficient(rotationalDrag),
	m_gravityMultiplier(gravityMultiplyer), m_bounciness(bounciness), m_groundedOn(nullptr)
{
    defineMember();
}

PhysicsComponent::~PhysicsComponent()
{
}

void PhysicsComponent::Init()
{
}

void PhysicsComponent::Update() { Update(SERVICE_LOCATOR.GetTime()->GetDeltaTime()); }

void PhysicsComponent::Update(double deltaTime)
{
    // Grounded Checks and Responses
    if (CheckForGround()) { TransitionToGround(); GroundedResponse(deltaTime); }
    else { UngroundedResponse(deltaTime); }

    // Apply acceleration to velocity
    glm::dvec3 newVelocity = GetVelocity() + GetAcceleration();
    double minVelocity = 0.001;
    newVelocity = glm::length(newVelocity) < minVelocity ? glm::dvec3(0) : newVelocity;
    SetVelocity(newVelocity);
    SetAcceleration(glm::dvec3(0));
    // Apply rotational acceleration to rotational velocity
    SetRotationalVelocity(GetRotationalVelocity() + GetRotationalAcceleration());
    SetRotationalAcceleration(glm::dvec3(0));

    // Apply velocity to position
    Transform* transform = pOwner->GetTransform();
	glm::dvec3 pos = transform->GetPosition();
	transform->SetPosition(pos + (GetVelocity() * deltaTime));
    // Apply rotational velocity to rotation
	glm::dvec3 rotation = transform->GetRotation();
	transform->SetRotation(rotation + (GetRotationalVelocity() * deltaTime));
}

void PhysicsComponent::Shutdown()
{
}

void PhysicsComponent::GroundedResponse(double deltaTime)
{
    ApplyFriction(deltaTime);
    ApplyDrag(deltaTime);
}

void PhysicsComponent::UngroundedResponse(double deltaTime)
{
    ApplyGravity(deltaTime);
    ApplyDrag(deltaTime);
	//ApplyRotationalDrag(deltaTime);
}

void PhysicsComponent::TransitionToGround()
{
    SetGrounded(true);
}

glm::dvec3 PhysicsComponent::GetGroundedNormal() const
{
    if (!Grounded()) { return glm::dvec3(0); }
    CollisionShape* groundedCollider = GetGroundedCollider();
    glm::dvec3 thisPosition = pOwner->GetComponent<CollisionComponent>()->GetCollisionShape()->GetPosition();
    glm::dvec3 otherPosition = groundedCollider->GetPosition();
    glm::dvec3 diff = thisPosition - otherPosition;
    double dist = glm::length(diff);
    glm::dvec3 dir = glm::normalize(diff);
    glm::dvec3 otherToThis = glm::normalize(thisPosition - otherPosition);

    return groundedCollider->GetNormal(otherToThis);
}

void PhysicsComponent::ApplyGravity(double deltaTime)
{
    ApplyForce(SERVICE_LOCATOR.GetPhysicsManager()->GetGravity() * GetGravityMultiplyer() * deltaTime);
}

void PhysicsComponent::ApplyFriction(double deltaTime)
{
    glm::dvec3 velocity = GetVelocity();
    double frictionCoefficient = GetFrictionCoefficient();
    glm::dvec3 frictionForce = -velocity * frictionCoefficient * deltaTime;
    ApplyForce(frictionForce);
}

void PhysicsComponent::ApplyDrag(double deltaTime)
{
    glm::dvec3 velocity = GetVelocity();
    double dragCoefficient = GetDrag();
    glm::dvec3 dragForce = -velocity * dragCoefficient * deltaTime;
    ApplyForce(dragForce);
}

void PhysicsComponent::ApplyRotationalDrag(double deltaTime)
{
    glm::dvec3 rotationalVelocity = GetRotationalVelocity();
    double dragCoefficient = GetRotationalDrag();
	glm::dvec3 dragForce = -rotationalVelocity * dragCoefficient * deltaTime;
    ApplyTorque(dragForce);
}

bool PhysicsComponent::StillGrounded()
{
    return CheckForGround();
}

bool PhysicsComponent::CheckForGround()
{
    CollisionManager* collisionManager = SERVICE_LOCATOR.GetCollisionManager();
    PhysicsManager* physicsManager = SERVICE_LOCATOR.GetPhysicsManager();
    CollisionComponent* collisionComponent = pOwner->GetComponent<CollisionComponent>();
    CollisionShape* collisionShape = collisionComponent->GetCollisionShape();
    // Check if velocity is moving with or against gravity
    glm::dvec3 gravDir = physicsManager->GetGravityDir();
    // Raycast down from the object to check for the ground
    glm::dvec3 bottomPoint = collisionShape->GetEdgePoint(gravDir);
    int mask = collisionComponent->GetCollisionMask();
    Raycast cast = collisionManager->CastRay(bottomPoint, gravDir, mask, s_grounded_offset);
    if (!cast.Hit()) { m_groundedOn = nullptr;  return false; }
    // Check if the object is moving away from the ground
    glm::dvec3 thisFrameVelocity = GetVelocity() + GetAcceleration();
    glm::dvec3 velocityDir = thisFrameVelocity != glm::dvec3(0) ? glm::normalize(thisFrameVelocity) : glm::dvec3(0);
    glm::dvec3 groundedNormal = cast.GetHitNormal();
    double groundedDot = glm::dot(groundedNormal, velocityDir);
    const double groundedThreshold = 0.002;
    if (groundedDot > groundedThreshold) { m_groundedOn = nullptr; return false; }
    m_groundedOn = cast.GetShapeHit();
    return true;
}
