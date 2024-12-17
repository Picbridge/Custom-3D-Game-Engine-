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
	// @brief load in the json and create game objects based on the data loaded
	// @param source: path of the json file 
	void CreateAllResources(const char* source);
private:
	static ResourceFactory* GetInstance();
	static std::unique_ptr<ResourceFactory> instance;

	// @brief creates game object with extracted components
	// @param member: set of component name and data 
	void createResource(rapidjson::Value::ConstMemberIterator member, ResourceType type = ResourceType::UNKNOWN);

	friend class ServiceLocator;
};