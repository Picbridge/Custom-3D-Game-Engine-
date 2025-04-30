#pragma once

enum ResourceType
{
	TEXTURE,
	SHADER,
	MATERIAL,
	GEOMETRY,
	UNKNOWN
};

class ResourceFactory
{
public:
	// @brief load in the default resources for engine
	void CreateDefaultResources();

	// @brief load in the resources for current project(game)
	void CreateAllResources();
private:
	static ResourceFactory* GetInstance();
	static std::unique_ptr<ResourceFactory> instance;

	// @brief loads in the resources
	// @param directory: path of the directory containing the shaders
	void loadResources(std::string directory);

	// @brief creates the shader
	// @param shaderFiles: set of shader path including vertex, fragment
	void createShaders(const std::unordered_map<std::string, std::pair<std::string, std::string>>& shaderFiles);

	// @brief creates the material
	// @param path: path of the material json file
	void createMaterials(const std::vector<std::filesystem::path>& materialFiles);

	friend class ServiceLocator;
};