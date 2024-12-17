#pragma once
class GameObjectManager
{
public:

	//@brief Constructor
	GameObjectManager() {}

	//@brief Destructor
	~GameObjectManager();

	//@brief Initializes the game objects
	void Init();

	//@brief Updates the game objects
	void Update();

	//@brief Renders the game objects
	void Render();

	// @brief deletes all game objects
	void Shutdown();

	//@brief Adds game object to the list
	//@param object : GameObject to add
	void AddGameObject(GameObject* object);

	//@brief Deletes the game object from the list
	//@param object : GameObject to delete
	void DeleteGameObject(GameObject* object);

	//@brief Searches for the game object by name
	//@param name : Name of the game object
	//@return GameObject* : Game object
    inline GameObject* GetGameObject(const std::string name) { return m_gameObjects[m_gameObjectMap[name]]; }

	//@brief Returns the list of game objects
	//@return std::vector<GameObject*> : List of game objects
	inline const std::vector<GameObject*> GetGameObjects() { return m_gameObjects; }

	const glm::mat4 GetShadowMatrix(glm::mat4 worldProj)
	{
		return glm::translate(glm::mat4(1.0f), glm::vec3(0.5f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)) * worldProj * lightView;
	}


private:
	static GameObjectManager* GetInstance();
	static std::unique_ptr<GameObjectManager> instance;
	FBO* gbuffer;
	FBO* m_shadowBuffer;
	glm::mat4 lightView;

	glm::mat4 LookAt(const glm::vec3 Eye, const glm::vec3 Center, const glm::vec3 Up)
	{
		glm::vec3 V = glm::normalize(Center - Eye);
		glm::vec3 A = glm::normalize(glm::cross(V, Up));
		glm::vec3 B = glm::cross(A, V);

		glm::mat4 R = glm::mat4(1.0f);

		R[0][0] = A.x;
		R[1][0] = A.y;
		R[2][0] = A.z;
		R[3][0] = -glm::dot(A, Eye);

		R[0][1] = B.x;
		R[1][1] = B.y;
		R[2][1] = B.z;
		R[3][1] = -glm::dot(B, Eye);

		R[0][2] = -V.x;
		R[1][2] = -V.y;
		R[2][2] = -V.z;
		R[3][2] = glm::dot(V, Eye);

		R = { { A.x, B.x, -V.x, 0}, {A.y, B.y, -V.y, 0}, {A.z, B.z, -V.z, 0}, {0, 0, 0, 1} };


		return R * glm::translate(glm::mat4(1.0f), glm::vec3(-Eye.x, -Eye.y, -Eye.z));
	}



	// Allow adding, seraching and deleting game objects by name in constant time while mantaining the order of the game objects in memory
	std::unordered_map<std::string, size_t> m_gameObjectMap;
	std::vector<GameObject*> m_gameObjects;

	friend class ServiceLocator;
};

