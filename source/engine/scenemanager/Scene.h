class Node;
class Skybox;	

#pragma once
class Scene
{
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
	void AddNode(Node* node);

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
	std::vector<Node*> GetNodes() { return m_nodes; }

	//@brief Returns the scene name
	//@return std::string : Scene name
	inline std::string GetName() const { return m_name; }

	//@brief Returns the scene source
	//@return std::string : Scene source
	inline std::string GetSceneSource() const { return m_sceneSource; }

	glm::vec3 lightPosition;
	glm::vec3 lightSpecular;
	glm::vec3 lightDiffuse;
	glm::vec3 lightAmbient;
	glm::mat4 lightSpaceMatrix;
	float near_plane, far_plane;
	unsigned int depthMap;
private:
	int m_nodeCount;
	std::string m_name;
	std::string m_sceneSource;
	std::vector<Node*> m_nodes;
	std::unique_ptr<Skybox> m_pSkybox;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
};
