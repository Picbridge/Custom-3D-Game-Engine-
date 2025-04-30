class Node;
class Skybox;	
class LightComponent;
class UIComponent;

#pragma once
class Scene
{
	friend class SceneManager;
	friend class UI;

public:
	Scene() : m_nodeCount(0) {};
	~Scene();

	void Init();
	void Update();
	void Render();
	void PostUpdate();
	void Shutdown();

	//@brief Adds node to the current scene
	//@param node : Node to add
	void AddNode(Node* node, Node* parent = nullptr);

	//@brief Deletes the node from the scene
	//@param node : Node to delete
	void DeleteNode(Node* node);

	//@brief Removes the node from the lisr without deleting it
	//@param node : Node to remove
	//@return Node* : Removed node
	Node* RemoveNode(Node* node);
	
	//@brief Sets the scene name
	//@param name : Scene name
	void SetName(const std::string& name) { m_name = name; }

	//@brief Sets the scene source
	//@param sceneSource : Scene source
	void SetSceneSource(const char* sceneSource) { m_sceneSource = sceneSource; }

	//@brief Returns the list of nodes
	//@return std::vector<Node*> : List of nodes
	const std::vector<Node*>& GetNodes() { return m_nodes; }

	//@brief Returns the scene name
	//@return std::string : Scene name
	inline std::string GetName() const { return m_name; }

	//@brief Returns the scene source
	//@return std::string : Scene source
	inline std::string GetSceneSource() const { return m_sceneSource; }

	//@brief Returns if the scene is initialized
	//@return bool : represents if the scene is initialized
	inline bool IsInitialized() const { return m_init; }

	//@brief Returns active light in the scene
	//@return LightComponent* : Current active light
	LightComponent* GetLight() { return light; }

	//@brief Sets the active light in the scene
	//@param light : Light to set
	void SetLight(LightComponent* light) { this->light = light; }

	//@brief Returns UI node used for rendering Game UI
	//@return Node* : UI node
	Node* GetUINode() { return m_pUINode.get(); }

	UIComponent* FindUIComponent(const std::string& name, Node* parent = nullptr);

private:
	LightComponent* light;
	unsigned int m_nodeCount;
	std::string m_name;
	std::string m_sceneSource;
	std::vector<Node*> m_nodes;

	std::vector<Node*> m_nonRenderableNodes;
	std::vector<Node*> m_opaqueBucket;
	std::vector<Node*> m_transparentBucket;
	std::unique_ptr<Skybox> m_pSkybox;

	std::unique_ptr<Node> m_pUINode;
	bool m_init = false;

	void FlushNodes(Node* parent = nullptr);
	void collectRenderableNodes(Node* node, std::vector<Node*>& nonRenderable, std::vector<Node*>& opaqueBucket, std::vector<Node*>& transparentBucket);
};
