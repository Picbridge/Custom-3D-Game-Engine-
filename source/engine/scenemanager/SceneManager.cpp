#include "../pch.h"
#include "SceneManager.h"

std::unique_ptr<SceneManager> SceneManager::instance = nullptr;

SceneManager* SceneManager::GetInstance()
{
	if (!instance)
		instance = std::unique_ptr<SceneManager>(new SceneManager());

	return instance.get();
}

SceneManager::SceneManager() : m_pCurrentScene(nullptr), m_currentSceneIndex(-1) 
{
}

SceneManager::~SceneManager()
{
	for (auto& scene : m_scenes)
		delete scene.second;

	instance = nullptr;
}

void SceneManager::ExportScene(Scene* scene)
{
	if (!scene)
		throw std::runtime_error("Scene is null");

	auto sceneName = scene->GetName();
	// Create a RapidJSON Document for the scene
	rapidjson::Document document(rapidjson::kObjectType);
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	// Array of nodes in the scene
	rapidjson::Value sceneJson(rapidjson::kObjectType);
	std::unordered_map<std::string, rapidjson::Value> classes;

	for (auto& node : scene->GetNodes())
	{
		auto className = getClassName(*node);
		if (classes.find(className) == classes.end())
		{
			rapidjson::Value classJson(rapidjson::kObjectType);
			classes[className] = classJson;
		}
		// Array of components in the node
		rapidjson::Value nodeJson(rapidjson::kObjectType);
		// Export the scene as a json file by going through each node and its components
		processNode(node, nodeJson, allocator);
		classes[className].AddMember(rapidjson::Value(node->GetName().c_str(), allocator).Move(), nodeJson, allocator);
	}
	//iterate through classes 
	for (auto& [key, val] : classes)
		sceneJson.AddMember(rapidjson::Value(key.c_str(), allocator).Move(), val, allocator);
	
    document.AddMember(rapidjson::Value(sceneName.c_str(), allocator).Move(), sceneJson, allocator);

	// Serialize the JSON document to a string
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	writer.SetIndent(' ', 4);
	document.Accept(writer);

	auto source = scene->GetSceneSource();
	if (source.empty())
		throw std::runtime_error("Scene source is not set.");

	std::filesystem::path sourcePath(source);
	std::filesystem::path directory = sourcePath.parent_path();

	// Ensure the directory exists
	if (!directory.empty())
		std::filesystem::create_directories(directory);
	else
		throw std::runtime_error("Invalid directory for scene source.");

	// Save the JSON string to the file
	std::ofstream outFile(sourcePath); // Open the file for writing
	if (!outFile) 
		throw std::runtime_error("Failed to create the JSON file: " + sourcePath.string());
	
	outFile << buffer.GetString(); // Write the JSON string to the file
	outFile.close(); // Close the file

	// Output the message with the file name and path
	std::cout << "Scene exported to file: " << sourcePath.filename() << " in path: " << directory << std::endl;
}

void SceneManager::AddScene(std::string name, const std::source_location& location)
{
	auto scene = new Scene();
	auto tempName = name;
	if (tempName.empty())
	{
		tempName = "EmptyScene";
		name = tempName;
	}
	
	int count = 1;
	while (m_scenes.find(tempName) != m_scenes.end())
		tempName = name + " (" + std::to_string(count++) + ")";

	m_scenes[tempName] = scene;
	m_sceneMap[tempName] = m_sceneOrder.size();
	m_sceneOrder.push_back(tempName);


	std::filesystem::path callerPath(location.file_name());
	std::filesystem::path directory = callerPath.parent_path(); // Get the directory of the caller's source file
	std::string fileName = tempName + ".json";

	// Try to load in the path
	std::filesystem::path defaultPath = directory / fileName;

	// Create the sourcefile if the source doesn't exist
	if (!std::filesystem::exists(defaultPath))
	{
		std::ofstream outFile(defaultPath); // Open the file for writing
		if (!outFile)
			throw std::runtime_error("Failed to create the JSON file: " + defaultPath.string());

		outFile << "{}"; // Write an empty JSON object to the file
		outFile.close(); // Close the file
	}

	scene->SetSceneSource(defaultPath.string().c_str());
	scene->SetName(tempName);
}

void SceneManager::RemoveScene(const Scene* scene)
{
	auto name = scene->GetName();
	RemoveScene(name);
}

void SceneManager::RemoveScene(const std::string& name)
{
	auto it = m_scenes.find(name);
	if (name == m_pCurrentScene->GetName())
		throw std::runtime_error("Cannot delete the current scene");

	if (it != m_scenes.end())
	{
		delete it->second;

		// Remove from m_sceneMap and m_sceneOrder
		auto mapIt = m_sceneMap.find(name);
		if (mapIt != m_sceneMap.end()) {
			size_t index = mapIt->second;
			m_sceneMap.erase(mapIt);

			if (index < m_sceneOrder.size()) {
				m_sceneOrder.erase(m_sceneOrder.begin() + index);
			}
		}

		m_scenes.erase(it);

	}
}

void SceneManager::SetSceneName(const Scene* scene, const std::string& name)
{
	auto originalName = scene->GetName();

	auto index = m_sceneMap[originalName];
	m_sceneMap.erase(originalName);
	m_sceneMap[name] = index;

	m_sceneOrder[index] = name;
	
	auto updatedScene = m_scenes[originalName];
	m_scenes.erase(originalName);
	m_scenes[name] = updatedScene;
	updatedScene->SetName(name);

	// Delete the old file and create a new one with the new name
	auto originalPath = updatedScene->GetSceneSource();
	if (std::filesystem::exists(originalPath))
		std::filesystem::remove(originalPath);

	std::filesystem::path sourcePath(originalPath);
	std::filesystem::path directory = sourcePath.parent_path();
	std::string fileName = name + ".json";

	std::filesystem::path newPath = directory / fileName;
	updatedScene->SetSceneSource(newPath.string().c_str());

	ExportScene(updatedScene);
}

void SceneManager::SetCurrentScene(const Scene* scene)
{
	auto name = scene->GetName();
	SetCurrentScene(name);
}

void SceneManager::SetCurrentScene(const std::string& name)
{
	auto it = m_scenes.find(name);

	if (m_pCurrentScene)
		m_pCurrentScene->Shutdown();

	if (it != m_scenes.end())
	{
		m_currentSceneIndex = m_sceneMap[name];
		m_pCurrentScene = it->second;

		// Initialize the new current scene
		if (m_pCurrentScene)
			m_pCurrentScene->Init();
	}
}

void SceneManager::MoveToNextScene()
{
	if (m_currentSceneIndex < m_sceneOrder.size() - 1)
	{
		if (m_pCurrentScene)
			m_pCurrentScene->Shutdown();
		m_currentSceneIndex++;
		m_pCurrentScene = m_scenes[m_sceneOrder[m_currentSceneIndex]];
		m_pCurrentScene->Init();
	}
}

void SceneManager::MoveToPreviousScene()
{
	if (m_currentSceneIndex > 0)
	{
		if (m_pCurrentScene)
			m_pCurrentScene->Shutdown();
		m_currentSceneIndex--;
		m_pCurrentScene = m_scenes[m_sceneOrder[m_currentSceneIndex]];
		m_pCurrentScene->Init();
	}
}

std::string SceneManager::toLower(const std::string& str)
{
	std::string lower = str;
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	return lower;
}

void SceneManager::processNode(Node* node, rapidjson::Value& nodeJson, rapidjson::Document::AllocatorType& allocator)
{
	if (!node || node->GetComponents().empty())
		return;

	rapidjson::Value children(rapidjson::kObjectType);
	for (auto& child : node->GetChildren())
	{
		rapidjson::Value childJson(rapidjson::kObjectType);
		processNode(child, childJson, allocator);
		children.AddMember(rapidjson::Value(child->GetName().c_str(), allocator).Move(), childJson, allocator);
	}
	nodeJson.AddMember(rapidjson::Value("Children", allocator).Move(), children, allocator);

	rapidjson::Value componentGroupJson(rapidjson::kObjectType);
    for (auto& [name, component] : node->GetComponents())
	{
		// json object for component
		rapidjson::Value componentJson(rapidjson::kObjectType);

		processComponent(component, componentJson, allocator);
		componentGroupJson.AddMember(rapidjson::Value(name.c_str(), allocator).Move(), componentJson, allocator);
	}
	nodeJson.AddMember(rapidjson::Value("Components", allocator).Move(), componentGroupJson, allocator);
}

void SceneManager::processComponent(IHasGettersSetters* component, rapidjson::Value& componentJson, rapidjson::Document::AllocatorType& allocator)
{
	auto getters = component->GetGetters();
	handleGetters(getters, componentJson, allocator);
}

void SceneManager::handleGetters(std::unordered_map<std::string, std::function<std::any()>> getters, rapidjson::Value& upperJson, rapidjson::Document::AllocatorType& allocator)
{
	if (getters.empty())
		return;

	for (auto& [variableName, getter] : getters)
	{
		std::any value = getter();
		rapidjson::Value variable(rapidjson::kArrayType);

		auto id2name = GameObjectTypeDictionary::typeIndexToName.find(value.type());
		if (id2name != GameObjectTypeDictionary::typeIndexToName.end()) {
			variable.PushBack(rapidjson::Value(std::string(id2name->second).c_str(), allocator).Move(), allocator);

			auto name2type = GameObjectTypeDictionary::typeStore.find(id2name->second);
			if (name2type != GameObjectTypeDictionary::typeStore.end()) {
				std::visit([&](auto&& type) {
					using T = std::decay_t<decltype(type)>;
					if constexpr (std::is_same_v<T, glm::vec2>)
					{
						glm::vec2 vec = std::any_cast<glm::vec2>(value);
						rapidjson::Value array(rapidjson::kArrayType);
						array.PushBack(vec.x, allocator);
						array.PushBack(vec.y, allocator);
						variable.PushBack(array, allocator);
					}
					else if constexpr (std::is_same_v<T, glm::vec3>)
					{
						glm::vec3 vec = std::any_cast<glm::vec3>(value);
						rapidjson::Value array(rapidjson::kArrayType);
						array.PushBack(vec.x, allocator);
						array.PushBack(vec.y, allocator);
						array.PushBack(vec.z, allocator);
						variable.PushBack(array, allocator);
					}
					else if constexpr (std::is_same_v<T, glm::dvec3>)
					{
						glm::dvec3 vec = std::any_cast<glm::dvec3>(value);
						rapidjson::Value array(rapidjson::kArrayType);
						array.PushBack(vec.x, allocator);
						array.PushBack(vec.y, allocator);
						array.PushBack(vec.z, allocator);
						variable.PushBack(array, allocator);
					}
					else if constexpr (std::is_same_v<T, glm::vec4>)
					{
						glm::vec4 vec = std::any_cast<glm::vec4>(value);
						rapidjson::Value array(rapidjson::kArrayType);
						array.PushBack(vec.x, allocator);
						array.PushBack(vec.y, allocator);
						array.PushBack(vec.z, allocator);
						array.PushBack(vec.w, allocator);
						variable.PushBack(array, allocator);
					}
					else if constexpr (std::is_same_v<T, std::string>)
					{
						std::string strValue = std::any_cast<std::string>(value);
						variable.PushBack(rapidjson::Value(strValue.c_str(), allocator).Move(), allocator);
					}
					else if constexpr (std::is_same_v<T, CollisionShape*>)
					{
						rapidjson::Value subVariable(rapidjson::kObjectType);
						auto obj = std::any_cast<T>(value);
						auto subGetters = obj->GetGetters();
						handleGetters(subGetters, subVariable, allocator);
						variable.PushBack(subVariable, allocator);
					}
					else
						variable.PushBack(rapidjson::Value(std::any_cast<T>(value)).Move(), allocator);
					}, name2type->second);
			}
		}
		upperJson.AddMember(rapidjson::Value(variableName.c_str(), allocator).Move(), variable, allocator);
	}
}
std::string SceneManager::getClassName(const Node& node) {
	std::string typeName = typeid(node).name();
	// Remove the "class " prefix if it exists
	const std::string classPrefix = "class ";
	size_t pos = typeName.find(classPrefix);
	if (pos != std::string::npos) {
		typeName.erase(pos, classPrefix.length());
	}
	return typeName;
}
