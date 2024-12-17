#pragma once
class Component;

class Node
{
public:
	Node();
	virtual ~Node();

	//--------------------------------
	//Property control
	//--------------------------------

	virtual void Init() {};
	virtual void Update() {};
	virtual void PostUpdate() {};
	virtual void Render() {};
	virtual void Shutdown() {};

	//@brief Add a child node to current node
	//@param node : Child node pointer
	virtual void AddChild(Node* node);

	//@brief Removes the selected node from the current node children
	//@param node : Child node to remove from the map
	virtual void RemoveChild(Node* node);

	//@brief Sets the node ID (not for user)
	//@param id : id for the current node
	virtual void SetID(size_t id);

	//@brief Destroys the current node
	virtual void Destroy();

	//@brief  Flushes the node from the memory if it needs to be deleted
	void Flush();

	//@brief Set the current object name
	void SetName(std::string name);

	//@brief Add a component to the current object
	template <typename DataType>
	typename std::enable_if_t<std::is_base_of<Component, DataType>::value, DataType*> AddComponent()
	{
		std::string dataType = Utils::GetClassName<DataType>();
		// assign new component
		m_components[dataType] = new DataType();
		setOwner(m_components[dataType]);
		initComponent(m_components[dataType]);
		return static_cast<DataType*>(m_components[dataType]);
	}

	template <typename DataType, typename... Args>
	typename std::enable_if_t<std::is_base_of<Component, DataType>::value, DataType*> AddComponent(Args&&... args) {
		std::string dataType = Utils::GetClassName<DataType>();
		// assign new component
		m_components[dataType] = new DataType(std::forward<Args>(args)...);
		setOwner(m_components[dataType]);
		initComponent(m_components[dataType]);
		return static_cast<DataType*>(m_components[dataType]);
	}

	//@brief Remove a component from the current object
	template <typename DataType>
	typename std::enable_if_t<std::is_base_of<Component, DataType>::value, void*> RemoveComponent()
	{
		std::string dataType = Utils::GetClassName<DataType>();

		auto del = m_components[dataType];
		m_components.erase(dataType);
		delete del;
		del = nullptr;

		return nullptr;
	}

	//@brief Get a component from the current object
	template <typename DataType>
	inline const typename std::enable_if_t<std::is_base_of<Component, DataType>::value, DataType*> GetComponent()
	{
		std::string dataType = Utils::GetClassName<DataType>();
		// check if the current GameObject has the component of DataType
		if (m_components.find(dataType) != m_components.end())
			return static_cast<DataType*>(m_components[dataType]);

		return nullptr;
	}

	//@brief Check if the current object has a component
	template <typename DataType>
	inline const typename std::enable_if_t<std::is_base_of<Component, DataType>::value, bool> HasComponent()
	{
		std::string dataType = Utils::GetClassName<DataType>();
		return m_components.find(dataType) != m_components.end();
	}

	//--------------------------------
	//Getters
	//--------------------------------

	//@brief Returns the node ID
	//@return int : Node ID
	inline const size_t GetID() const { return m_id; }

	//@brief Returns the transform component
	//@return Transform* : Transform component
	inline Transform* GetTransform() const { return m_pTransform.get(); }

	//@brief Returns the world transform matrix
	//@return glm::mat4 : World transform matrix
	inline const glm::mat4 GetWorldTransform()
	{
		glm::mat4 parentTransform = (m_pParent == nullptr) ? glm::mat4(1.f) : m_pParent->GetWorldTransform();
		m_worldTransform = parentTransform * m_pTransform->GetModel();
		return m_worldTransform;
	}


	//@brief Returns the parent node
	//@return Node* : Parent node
	inline Node* GetParent() const { return m_pParent; }

	//@brief Returns the children nodes
	//@return std::unordered_map<int, Node*> : Children nodes
	inline std::vector<Node*> GetChildren() const { return m_children; }

	//@brief Returns whether the node needs to be deleted
	const bool NeedsDeletion() const { return m_needsDeletion; }

	//@brief Get the current object name
	//@return std::string : object name
	const std::string GetName() const { return m_name; }

	//@brief Get the current object components
	//@return std::unordered_map<std::string, Component*> : object components
	const std::unordered_map<std::string, Component*> GetComponents() const { return m_components; }
protected:
	//ID for node entity control
	size_t m_id;
	std::string m_name;
	std::unique_ptr<Transform> m_pTransform;
	Node* m_pParent;
	glm::mat4 m_worldTransform;
	std::unordered_map<std::string, Component*> m_components;
	std::vector<Node*> m_children;
	bool m_needsDeletion;
	
private:
	// @brief Set the owner of the component when added
	void setOwner(Component* sub);
	// @brief Initialize the component when added
	void initComponent(Component* sub);
};

