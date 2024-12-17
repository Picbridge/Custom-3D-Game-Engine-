#include "../pch.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"
//#include "CollisionManager.h"


void CollisionComponent::Init()
{
	defineMember();
	SERVICE_LOCATOR.GetCollisionManager()->AddCollisionComponent(this);
}

void CollisionComponent::Update()
{
	this->GetCollisionShape()->SetPosition(pOwner->GetComponent<TransformComponent>()->GetPosition());
	this->GetCollisionShape()->SetRotation(pOwner->GetComponent<TransformComponent>()->GetRotation());
	//this->GetCollisionShape()->SetScale(pOwner->GetComponent<TransformComponent>()->GetScale());
}

void CollisionComponent::Shutdown()
{
	SERVICE_LOCATOR.GetCollisionManager()->RemoveCollisionComponent(this);
}

bool CollisionComponent::CanCollideWith(const CollisionComponent* other) const
{
	assert(other && "Other collision component is null");
    if (!other) return false;
    return (m_collisionLayer & other->GetCollisionMask()) != 0 && (other->GetCollisionLayer() & m_collisionMask) != 0;
}

glm::dvec3 CollisionComponent::Cast_LastAvailablePosition
	(glm::dvec3 startPosition, glm::dvec3 endPosition, glm::dvec3 startRotation, glm::dvec3 endRotation, int iterations = 1)
{
	CollisionComponent tempComponent = *this;
	tempComponent.GetCollisionShape()->SetPosition(startPosition);
	glm::dvec3 lastPosition = startPosition;
	for (int i = 1; i < iterations; i++)
	{
		glm::dvec3 newPosition = startPosition + ((endPosition - startPosition) * static_cast<double>(i / iterations));
		tempComponent.GetCollisionShape()->SetPosition(newPosition);

		if (SERVICE_LOCATOR.GetCollisionManager()->ShapeIsColliding(&tempComponent)) { break; }
		lastPosition = newPosition;
	}
	return lastPosition;
}

glm::dvec3 CollisionComponent::Cast_FirstAvailablePosition
	(glm::dvec3 startPosition, glm::dvec3 endPosition, int iterations = 1)
{
	CollisionComponent tempComponent = *this;
	tempComponent.GetCollisionShape()->SetPosition(startPosition);
	for (int i = 1; i < iterations; i++)
	{
		glm::dvec3 newPosition = startPosition + ((endPosition - startPosition) * static_cast<double>(i / iterations));
		tempComponent.GetCollisionShape()->SetPosition(newPosition);

		if (!SERVICE_LOCATOR.GetCollisionManager()->ShapeIsColliding(&tempComponent)) { return newPosition; }
	}
	return startPosition;
}

std::pair<CollisionComponent, CollisionComponent*> CollisionComponent::Cast_FirstCollision
	(glm::dvec3 startPosition, glm::dvec3 endPosition, glm::dvec3 startRotation, glm::dvec3 endRotation, int iterations)
{
	assert(iterations > 0 && "Iterations must be greater than 0");
	CollisionComponent tempComponent = *this;
	glm::dvec3 lastPosition = startPosition;
	glm::dvec3 lastRotation = startRotation;
	glm::dvec3 moveVector = endPosition - startPosition;
	glm::dvec3 rotateVector = endRotation - startRotation;
	for (int i = 1; i <= iterations; i++)
	{
		glm::dvec3 newPosition = startPosition + (moveVector * static_cast<double>(i) / static_cast<double>(iterations));
		glm::dvec3 newRotation = startRotation + (rotateVector * static_cast<double>(i) / static_cast<double>(iterations));
		tempComponent.GetCollisionShape()->SetPosition(newPosition);
		tempComponent.GetCollisionShape()->SetRotation(newRotation);
		if (CollisionComponent* collidedObject = SERVICE_LOCATOR.GetCollisionManager()->ShapeIsColliding(this))
		{
			return std::make_pair(tempComponent, collidedObject);
		}
	}
	return std::pair{ tempComponent, nullptr };
}
