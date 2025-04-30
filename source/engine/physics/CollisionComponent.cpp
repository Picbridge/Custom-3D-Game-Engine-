#include "../pch.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"
//#include "CollisionManager.h"


void CollisionComponent::Init()
{
	MatchTransform();
}

void CollisionComponent::Update()
{
	MatchTransform();
}

void CollisionComponent::MatchTransform()
{
	auto transformComponent = pOwner->GetComponent<TransformComponent>();
	auto collisionShape = GetCollisionShape();
	collisionShape->SetPosition(transformComponent->GetPosition());
	collisionShape->SetRotation(transformComponent->GetRotation());
	collisionShape->SetScale(transformComponent->GetScale());
}

void CollisionComponent::Shutdown()
{
}

bool CollisionComponent::CanCollideWith(const CollisionComponent* other) const
{
	assert(other && "Other collision component is null");
    if (!other) return false;
    return (m_collisionLayer & other->GetCollisionMask()) != 0 && (other->GetCollisionLayer() & m_collisionMask) != 0;
}


#pragma region ShapeCasts
CollisionComponent CollisionComponent::Cast_LastAvailablePosition
	(glm::dvec3 startPosition, glm::dvec3 endPosition, glm::dvec3 startRotation, glm::dvec3 endRotation, int iterations = 1)
{
	CollisionComponent tempComponent = *this;
	tempComponent.GetCollisionShape()->SetPosition(startPosition);
    tempComponent.GetCollisionShape()->SetRotation(startRotation);
	glm::dvec3 lastPosition = startPosition;
	glm::dvec3 lastRotation = startRotation;
	glm::dvec3 moveVector = endPosition - startPosition;
	glm::dvec3 rotateVector = endRotation - startRotation;

	
	for (int i = 1; i < iterations; i++)
	{
		glm::dvec3 newPosition = startPosition + ((endPosition - startPosition) * static_cast<double>(i / iterations));
        glm::dvec3 newRotation = startRotation + ((endRotation - startRotation) * static_cast<double>(i / iterations));
		CollisionComponent nextComponent = tempComponent;
        nextComponent.GetCollisionShape()->SetPosition(newPosition);
        nextComponent.GetCollisionShape()->SetRotation(newRotation);

		if (SERVICE_LOCATOR.GetCollisionManager()->ShapeIsColliding(&nextComponent)) { break; }
        tempComponent = nextComponent;
	}
	return tempComponent;
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
	(glm::dvec3 startPosition, glm::dvec3 endPosition, glm::dvec3 startRotation, glm::dvec3 endRotation, int iterations) const
{
	assert(iterations > 0 && "Iterations must be greater than 0");
	CollisionComponent tempComponent = *this;
	glm::dvec3 lastPosition = startPosition;
	glm::dvec3 lastRotation = startRotation;
	glm::dvec3 moveVector = endPosition - startPosition;
	glm::dvec3 rotateVector = endRotation - startRotation;
    CollisionComponent* collidedObject = nullptr;
	for (int i = 1; i <= iterations; i++)
	{
		glm::dvec3 newPosition = startPosition + (moveVector * static_cast<double>(i) / static_cast<double>(iterations));
		glm::dvec3 newRotation = startRotation + (rotateVector * static_cast<double>(i) / static_cast<double>(iterations));
		tempComponent.GetCollisionShape()->SetPosition(newPosition);
		tempComponent.GetCollisionShape()->SetRotation(newRotation);
		if (collidedObject = SERVICE_LOCATOR.GetCollisionManager()->ShapeIsColliding(&tempComponent)) { break; }
	}
	return std::pair{ tempComponent, collidedObject };
}
std::pair<CollisionComponent, CollisionComponent*> CollisionComponent::Cast_FirstCollision(glm::dvec3 endPosition, glm::dvec3 endRotation, int iterations) const
{
    glm::dvec3 StartPosition = pOwner->GetComponent<TransformComponent>()->GetPosition();
    glm::dvec3 StartRotation = pOwner->GetComponent<TransformComponent>()->GetRotation();
    return Cast_FirstCollision(StartPosition, endPosition, StartRotation, endRotation, iterations);
}
#pragma endregion