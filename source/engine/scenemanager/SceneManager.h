#pragma once
class Component;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	//@brief Exports the scene as a json file
	//@param scene : Scene to export
	void ExportScene(Scene* scene);

	//@brief Adds a new scene to the list
	void AddScene(std::string name = "", const std::source_location& location = std::source_location::current());

	//@brief Deletes the scene from the list
	//@param scene : Scene to delete
	void RemoveScene(const Scene* scene);

	//@brief Deletes the scene from the list by name
	//@param name : Scene name
	inline void RemoveScene(const std::string& name);
	
	//@brief Set name of the scene
	// @param name : Scene name
	void SetSceneName(const Scene* scene, const std::string& name);

	//@brief Moves to the next scene
	void MoveToNextScene();
	//@brief Moves to the previous scene
	void MoveToPreviousScene();

	//@brief Sets the current scene
	//@param scene : Scene to set
	void SetCurrentScene(const Scene* scene);
	//@brief Sets the current scene
	//@param name : Scene name
	inline void SetCurrentScene(const std::string& name);

	inline Scene* GetCurrentScene() { return m_pCurrentScene; }


private:
	static SceneManager* GetInstance();
	static std::unique_ptr<SceneManager> instance;

	std::unordered_map<std::string , Scene*> m_scenes;
	std::vector<std::string> m_sceneOrder;
	std::unordered_map<std::string, size_t> m_sceneMap;
	Scene* m_pCurrentScene;
	size_t m_currentSceneIndex;

	void processNode(Node* node, rapidjson::Value& nodeJson, rapidjson::Document::AllocatorType& allocator);
	void processComponent(IHasGettersSetters* component, rapidjson::Value& componentJson, rapidjson::Document::AllocatorType& allocator);
	void handleGetters(std::unordered_map<std::string, std::function<std::any()>> getters, rapidjson::Value& upperJson, rapidjson::Document::AllocatorType& allocator);
	std::string getClassName(const Node& node);
	inline std::string toLower(const std::string& str);

	friend class ServiceLocator;
};

