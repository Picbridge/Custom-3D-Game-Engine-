#include "pch.h"

void Component::SetOwner(Node* owner)
{
	pOwner = owner;
}

//@brief Get the owner of the component
//@return GameObject* The owner of the component
GameObject* Component::GetOwnerAsGameObject() { return dynamic_cast<GameObject*>(pOwner); }