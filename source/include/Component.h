#pragma once
class Node;

class Component : public IHasGettersSetters
{
public:
	Component() : pOwner(nullptr) {}
	virtual ~Component() {}

	//-------------------
	//Component essentials
	//-------------------

	//@brief Initialize the component
	virtual void Init() = 0;
	//@brief Update the component
	virtual void Update() = 0;
	//@brief Shutdown the component
	virtual void Shutdown() = 0;

	//-------------------
	// Component management
	// ------------------
	
	//@brief Assign ownership to the component
	//@param owner : The GameObject to get attached to
	void SetOwner(Node* owner);

	//@brief Get the owner of the component
	//@return GameObject* The owner of the component
	Node* GetOwner() { return pOwner; }
	
	//@brief Get the owner of the component
	//@return GameObject* The owner of the component
	GameObject* GetOwnerAsGameObject() { return dynamic_cast<GameObject*>(pOwner); }

	//@brief Get the component of the owner
	template <typename DataType>
	DataType* GetComponent() { return pOwner->GetComponent<DataType>(); }
	template <typename DataType>
	bool HasComponent() { return pOwner->HasComponent<DataType>(); }

protected:
	Node* pOwner;

	void defineMember() override {}

	friend SceneManager;
};

