#include "../pch.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "PhysicsManager.h"

PhysicsComponent::PhysicsComponent(double mass, double gravityMultiplyer, double bounciness, double drag, double rotationalDrag) : Component(),
	m_velocity(glm::dvec3(0)), m_rotationalVelocity(glm::dvec3(0)),
	m_acceleration(glm::dvec3(0)), m_rotationalAcceleration(glm::dvec3(0)),
	m_mass(mass), m_inverseMass(1 / m_mass), m_drag(drag), m_rotationalDrag(rotationalDrag),
	m_gravityMultiplier(gravityMultiplyer), m_bounciness(bounciness), m_grounded(false)
{
	SERVICE_LOCATOR.GetPhysicsManager()->AddPhysicsComponent(this);
	Init();
}

PhysicsComponent::~PhysicsComponent()
{
	SERVICE_LOCATOR.GetPhysicsManager()->RemovePhysicsComponent(this);
}

void PhysicsComponent::Init()
{
	defineMember();
}

void PhysicsComponent::Update(double deltaTime)
{
	m_velocity += m_acceleration;
	m_rotationalVelocity += m_rotationalAcceleration;
	m_acceleration = glm::dvec3(0);
	m_rotationalAcceleration = glm::dvec3(0);

	GroundedResponse();

	glm::dvec3 pos = pOwner->GetTransform()->GetPosition();
	pOwner->GetTransform()->SetPosition(pos + (m_velocity * deltaTime));
	glm::dvec3 rotation = pOwner->GetTransform()->GetRotation();
	pOwner->GetTransform()->SetRotation(rotation + (m_rotationalVelocity * deltaTime));
}

void PhysicsComponent::Shutdown()
{
}

void PhysicsComponent::GroundedResponse()
{
	glm::dvec3 gravity = SERVICE_LOCATOR.GetPhysicsManager()->GetGravity();
	glm::dvec3 grav_dir = glm::normalize(gravity);

	if (!m_grounded)
	{
		auto groundCheck = CheckForGround();
		if (groundCheck)	// Transition to grounded state
		{
			if (m_bounciness > 0)	// Bounce off the ground; if velocity still too low, set grounded to true
			{
				// TODO: Implement bouncing off the ground
				// glm::dvec3 normal = groundCheck->first.GetCollisionShape()->GetNormal(grav_dir);
				// m_velocity = m_velocity - 2.0 * glm::dot(m_velocity, normal) * normal * m_bounciness;
			}
			double parallel_to_gravity = glm::dot(glm::normalize(m_velocity), grav_dir);
			if (parallel_to_gravity >= 0)	// If the velocity is aligned with gravity, the object is grounded
			{
				m_grounded = true;
				CollisionComponent* collisionComponent = this->GetComponent<CollisionComponent>();
				CollisionComponent tempComponent = groundCheck->first;
				glm::dvec3 startPos = tempComponent.GetCollisionShape()->GetPosition();
				glm::dvec3 endPos = collisionComponent->GetCollisionShape()->GetPosition();
				glm::dvec3 newPos = tempComponent.Cast_FirstAvailablePosition(startPos, endPos, 10); // TODO maybe change iterations to depend on velocity

				glm::dvec3 rotation = groundCheck->first.GetCollisionShape()->GetRotation();

				glm::dvec3 projection = (glm::dot(m_velocity, gravity) / glm::length2(gravity)) * gravity;
				m_velocity -= projection; // Remove velocity component aligned with gravity
				m_rotationalVelocity = glm::dvec3(0);
				// still need to set position and rotation to the ground
				collisionComponent->GetCollisionShape()->SetPosition(newPos);
				collisionComponent->GetCollisionShape()->SetRotation(rotation);
				pOwner->GetTransform()->SetPosition(newPos);
				pOwner->GetTransform()->SetRotation(rotation);
				m_velocity = ApplyDrag();
			}
		}
	}
	else	// grounded
	{
		m_grounded = IsStillGrounded();
		if (m_grounded)	// continue to be grounded
		{
			return; // TODO: Can move left or right and be inside the ground
		}
	}

	if (!m_grounded)
	{
		m_velocity += gravity * m_gravityMultiplier;
		m_velocity = ApplyDrag();
		m_rotationalVelocity = ApplyRotationalDrag();
	}
}

void PhysicsComponent::Update() 
{ Update(SERVICE_LOCATOR.GetTime()->GetDeltaTime()); }


glm::dvec3 PhysicsComponent::ApplyDrag()
{
	glm::dvec3 drag = m_velocity * (1 - m_drag);
	return drag;
}

glm::dvec3 PhysicsComponent::ApplyRotationalDrag()
{
	glm::dvec3 drag = m_rotationalVelocity * (1 - m_rotationalDrag);
	return drag;
}

std::optional<std::pair<CollisionComponent,CollisionComponent>> PhysicsComponent::CheckForGround()
{
	CollisionComponent* collisionComponent = this->GetComponent<CollisionComponent>();
	if (!collisionComponent) { return std::nullopt; }	// If this has no collision component, this cannot be grounded
	glm::dvec3 gravity = SERVICE_LOCATOR.GetPhysicsManager()->GetGravity();
	glm::dvec3 gravDir = glm::normalize(gravity);
	double  gravityDot = glm::dot(glm::normalize(m_velocity), gravDir);
	if (gravityDot <= 0) { return std::nullopt; } // If this is moving upwards, this is not grounded
	double deltaTime = SERVICE_LOCATOR.GetTime()->GetDeltaTime();
	glm::dvec3 startPosition = collisionComponent->GetCollisionShape()->GetPosition();
	glm::dvec3 endPosition = startPosition + (m_velocity * deltaTime);
	glm::dvec3 startRotation = collisionComponent->GetCollisionShape()->GetRotation();
	glm::dvec3 endRotation = startRotation + (m_rotationalVelocity * deltaTime);

	auto castResult = collisionComponent->Cast_FirstCollision(startPosition, endPosition, startRotation, endRotation, 10);
	CollisionComponent* collidedComponent = castResult.second;
	if (!collidedComponent) { return std::nullopt; }	// If no collision occurred, the object is not grounded
	if (collidedComponent->HasComponent<PhysicsComponent>()) { return std::nullopt; }	// If the collided object has no physics component, this cannot be grounded
	return std::pair<CollisionComponent, CollisionComponent> {castResult.first, *castResult.second};
}

bool PhysicsComponent::IsStillGrounded()
{
    CollisionComponent* collisionComponent = this->GetComponent<CollisionComponent>();
    if (!collisionComponent) { return false; }	// If this has no collision component, this cannot be grounded
	
    glm::dvec3 gravity = SERVICE_LOCATOR.GetPhysicsManager()->GetGravity();
	double gravityDot = glm::dot(m_velocity, gravity); // how much of the velocity is aligned with gravity
	if (gravityDot < 0) { return false; } // If dot product is negative, the object is moving upwards and not grounded

    glm::dvec3 startPosition = collisionComponent->GetCollisionShape()->GetPosition();
	glm::dvec3 endPosition = startPosition + (glm::normalize(gravity) * s_offset);	// Small offset in the direction of gravity
    glm::dvec3 startRotation = collisionComponent->GetCollisionShape()->GetRotation();
    glm::dvec3 endRotation = startRotation;

    auto castResult = collisionComponent->Cast_FirstCollision(startPosition, endPosition, startRotation, endRotation);
    CollisionComponent* collidedComponent = castResult.second;
    if (!collidedComponent) { return false; }	// If no collision occurred, the object is not grounded
    if (collidedComponent->HasComponent<PhysicsComponent>()) { return false; } // If the collided object has no physics component, this cannot be grounded

    return true;
}
