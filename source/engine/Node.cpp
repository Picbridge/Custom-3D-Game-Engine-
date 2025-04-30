#include "pch.h"
#include "Node.h"
#include "Component.h"
#include "TransformComponent.h"
#include "scenemanager/SceneManager.h"
#include "physics/PhysicsComponent.h"
#include "physics/CollisionComponent.h"

Node::Node() : m_pParent(nullptr), m_needsDeletion(false), m_id(-1), m_name("")
{
	m_pTransform = std::make_unique<Transform>();
	GetWorldTransform();
	// Add a transform component to the node as a default component
	AddComponent<TransformComponent>()->Init();
}

Node::~Node()
{
	//delete m_pTransform;

	// delete all the child nodes on deletion
	for (auto child : m_children)
	{
		SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->DeleteNode(child);
		delete child;
	}
}

void Node::AddChild(Node* node)
{
	node->m_pParent = this;
	node->SetID(m_children.size());
	m_children.push_back(node);

	// Initialize the child node's transform to fit the parent node
	node->GetWorldTransform();
}

void Node::RemoveChild(Node* node)
{
	size_t index = node->GetID();
	size_t lastIndex = m_children.size() - 1;

	m_children[index]->Destroy();

	if (index == lastIndex)
	{
		m_children.pop_back();
	}
	else
	{
		m_children[index] = m_children[lastIndex];
		m_children.pop_back();
		m_children[index]->SetID(index);
	}
}

void Node::SetID(size_t id)
{
m_id = id;
}

void Node::Destroy()
{
m_needsDeletion = true;
}

void Node::Flush(Node* parent)
{
	if (m_needsDeletion) 
	{
		if (parent) { parent->RemoveChild(this); }
		else { SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->DeleteNode(this); }

		delete this;
	}
}

void Node::SetName(std::string name)
{
	m_name = name;
}

void Node::RemoveComponent(std::unordered_map<std::string, Component*>::iterator compItr)
{
	Component* comp = compItr->second;
	m_components.erase(compItr);
	delete comp;
}

void Node::setOwner(Component* sub)
{
	sub->SetOwner(this);
}

void Node::initComponent(Component* sub)
{
	sub->Init();
}

void Node::addComponentToManager(Component* comp)
{
	if (PhysicsComponent* physicsComp = dynamic_cast<PhysicsComponent*>(comp); physicsComp)
	{
		SERVICE_LOCATOR.GetPhysicsManager()->AddPhysicsComponent(physicsComp);
	}
	else if (CollisionComponent* collisionComp = dynamic_cast<CollisionComponent*>(comp); collisionComp)
	{
		SERVICE_LOCATOR.GetCollisionManager()->AddCollisionComponent(collisionComp);
	}
}
