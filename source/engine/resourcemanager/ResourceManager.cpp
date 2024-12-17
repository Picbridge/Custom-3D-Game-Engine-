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