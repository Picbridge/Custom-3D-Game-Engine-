#include "../pch.h"
#include "ResourceManager.h"

std::unique_ptr<ResourceManager> ResourceManager::instance = nullptr;

ResourceManager* ResourceManager::GetInstance()
{
	if (!instance)
		instance = std::unique_ptr<ResourceManager>(new ResourceManager());;

	return instance.get();
}

ResourceManager::~ResourceManager()
{
	for (auto& kv : m_textures)
		delete kv.second;
	m_textures.clear();
	for (auto& kv : m_geometries)
		delete kv.second;
	m_geometries.clear();
	for (auto& kv : m_materials)
		delete kv.second;
	m_materials.clear();
	for (auto& kv : m_shaders)
	{
		kv.second->Unuse();
		delete kv.second;
	}
	m_shaders.clear();
}

void ResourceManager::AddTexture(const std::string& name, Texture* texture)
{
	m_textures[name] = texture;
}

void ResourceManager::AddShader(const std::string& name, Shader* shader)
{
	m_shaders[name] = shader;
}

void ResourceManager::AddMaterial(const std::string& name, Material* material)
{
	m_materials[name] = material;
}

void ResourceManager::AddGeometry(const std::string& name, Geometry* geometry)
{
	m_geometries[name] = geometry;
}

void ResourceManager::ExportAllMaterial()
{
	for (auto& kv : m_materials)
	{
		ExportMaterial(kv.first);
	}
}

void ResourceManager::ExportMaterial(const std::string& name)
{
	auto mat = m_materials[name];

	// Create a RapidJSON Document for the material
	rapidjson::Document document(rapidjson::kObjectType);
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	// Material JSON
	//rapidjson::Value document(rapidjson::kObjectType);

	// Export shader
	auto shaderName = GetShaderName(mat->GetShader());
	rapidjson::Value shaderJson(rapidjson::kStringType);
	shaderJson.SetString(shaderName.c_str(), allocator);
	document.AddMember(rapidjson::Value("Shader", allocator).Move(), shaderJson, allocator);

	// Export diffuse texture
	if (mat->GetTextureDiffuse() || mat->GetTextureSpecular())
	{
		rapidjson::Value textureJson(rapidjson::kObjectType);

		if (mat->GetTextureDiffuse())
		{
			auto textureName = GetTextureName(mat->GetTextureDiffuse());
			rapidjson::Value diffuseJson(rapidjson::kStringType);
			diffuseJson.SetString(textureName.c_str(), allocator);
			textureJson.AddMember(rapidjson::Value("Diffuse", allocator).Move(), diffuseJson, allocator);
		}
		if (mat->GetTextureSpecular())
		{
			auto textureName = GetTextureName(mat->GetTextureSpecular());
			rapidjson::Value specularJson(rapidjson::kStringType);
			specularJson.SetString(textureName.c_str(), allocator);
			textureJson.AddMember(rapidjson::Value("Specular", allocator).Move(), specularJson, allocator);
		}

		document.AddMember(rapidjson::Value("Textures", allocator).Move(), textureJson, allocator);
	}
	
	rapidjson::Value colorJson(rapidjson::kArrayType);
	auto color = mat->GetColor();
	colorJson.PushBack(color.r, allocator);
	colorJson.PushBack(color.g, allocator);
	colorJson.PushBack(color.b, allocator);
	document.AddMember(rapidjson::Value("Color", allocator).Move(), colorJson, allocator);

	document.AddMember(rapidjson::Value("Shininess", allocator).Move(), mat->GetShininess(), allocator);

	document.AddMember(rapidjson::Value("Alpha", allocator).Move(), mat->GetAlpha(), allocator);

	// Serialize the JSON document to a string
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	writer.SetIndent(' ', 4);
	document.Accept(writer);
	
	auto source = m_resourceSource + name + ".json";

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
	std::cout << "Resource exported to file: " << sourcePath.filename() << " in path: " << directory << std::endl;
}

void ResourceManager::RemoveTexture(const std::string& name)
{
	delete m_textures[name];
	m_textures.erase(name);
}

void ResourceManager::RemoveShader(const std::string& name)
{
	m_shaders[name]->Unuse();
	delete m_shaders[name];
	m_shaders.erase(name);
}

void ResourceManager::RemoveMaterial(const std::string& name)
{
	delete m_materials[name];
	m_materials.erase(name);
}

void ResourceManager::RemoveGeometry(const std::string& name)
{
	delete m_geometries[name];
	m_geometries.erase(name);
}