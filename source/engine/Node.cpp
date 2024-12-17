#include "pch.h"
#include "TransformComponent.h"
#include "scenemanager/SceneManager.h"

Node::Node() : m_pParent(nullptr), m_needsDeletion(false), m_id(-1), m_name("")
{
	m_pTransform = std::unique_ptr<Transform>(new Transform());
	GetWorldTransform();
	// Add a transform component to the node as a default component
	AddComponent<TransformComponent>();
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
	// Remove the node from the scene graph in order to make the child node dependent on the parent node
	SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->RemoveNode(node);
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

	if (index == lastIndex)
	{
		m_children[index]->Destroy();
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

void Node::Flush()
{
	if (m_needsDeletion)
		delete this;
}

void Node::SetName(std::string name)
{
	m_name = name;
}

void Node::setOwner(Component* sub)
{
	sub->SetOwner(this);
}

void Node::initComponent(Component* sub)
{
	sub->Init();
}