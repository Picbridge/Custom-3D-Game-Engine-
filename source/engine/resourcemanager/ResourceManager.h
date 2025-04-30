#pragma once
class ResourceManager
{
public:

	// @brief Destructor
	~ResourceManager();
	// @brief Adds the texture to the list
	// @param name : Name of the texture
	// @param texture : Texture to add
	void AddTexture(const std::string& name, Texture* texture);
	// @brief Adds the shader to the list
	// @param name : Name of the shader
	// @param shader : Shader to add
	void AddShader(const std::string& name, Shader* shader);
	// @brief Adds the material to the list
	// @param name : Name of the material
	// @param material : Material to add
	void AddMaterial(const std::string& name, Material* material);
	// @brief Adds the geometry to the list
	// @param name : Name of the geometry
	// @param geometry : Geometry to add
	void AddGeometry(const std::string& name, Geometry* geometry);

	// @brief Exports all resources
	void ExportAllMaterial();

	// @brief Exports the material
	// @param name : Name of the material
	void ExportMaterial(const std::string& name);

	void RemoveTexture(const std::string& name);
	void RemoveShader(const std::string& name);
	void RemoveMaterial(const std::string& name);
	void RemoveGeometry(const std::string& name);

	// @brief Returns the name of the texture
	// @param texture : Texture
	// @return std::string : Name of the texture
	std::string GetTextureName(Texture* texture)
	{
		for (auto& it : m_textures)
		{
			if (it.second == texture)
				return it.first;
		}
		return "";
	}

	// @brief Returns the name of the shader
	// @param shader : Shader
	// @return std::string : Name of the shader
	std::string GetShaderName(Shader* shader)
	{
		for (auto& it : m_shaders)
		{
			if (it.second == shader)
				return it.first;
		}
		return "";
	}

	// @brief Returns the name of the material
	// @param material : Material
	// @return std::string : Name of the material
	std::string GetMaterialName(Material* material)
	{
		for (auto& it : m_materials)
		{
			if (it.second == material)
				return it.first;
		}
		return "";
	}

	// @brief Returns the name of the geometry
	// @param geometry : Geometry
	// @return std::string : Name of the geometry
	std::string GetGeometryName(Geometry* geometry)
	{
		for (auto& it : m_geometries)
		{
			if (it.second == geometry)
				return it.first;
		}
		return "";
	}

	// @brief Searches for the texture by name
	// @param name : Name of the texture
	// @return Texture* : Texture
	inline Texture* GetTexture(const std::string& name) const { 
		if (m_textures.find(name) != m_textures.end())
			return m_textures.at(name);
		return nullptr;
	}

	// @brief Searches for the shader by name
	// @param name : Name of the shader
	// @return Shader* : Shader
	inline Shader* GetShader(const std::string& name) const { 
		if (m_shaders.find(name) != m_shaders.end())
			return m_shaders.at(name); 
		return m_shaders.at("Default");
	}

	// @brief Searches for the material by name
	// @param name : Name of the material
	// @return Material* : Material
	inline Material* GetMaterial(const std::string& name) const { 
		if (m_materials.find(name) != m_materials.end())
			return m_materials.at(name);
		return m_materials.at("Default");
	}


	// @brief Returns map of materials
	std::unordered_map<std::string, Material*>& GetMaterials() 
	{ return m_materials; }

	// @brief Returns map of textures
	std::unordered_map<std::string, Texture*>& GetTextures()
	{ return m_textures; }

	// @brief Searches for the geometry by name
	// @param name : Name of the geometry
	// @return Geometry* : Geometry
	inline Geometry* GetGeometry(const std::string& name)const { 
		if (m_geometries.find(name) != m_geometries.end())
			return m_geometries.at(name);
		return m_geometries.at("Cube");
	}

	//@brief Sets the resource source
	//@param sceneSource : Resource source
	void SetResourceSource(const char* resourceSource) { m_resourceSource = resourceSource; }
private:
	static ResourceManager* GetInstance();
	static std::unique_ptr<ResourceManager> instance;

	std::unordered_map<std::string, Texture*> m_textures;
	std::unordered_map<std::string, Shader*> m_shaders;
	std::unordered_map<std::string, Material*> m_materials;
	std::unordered_map<std::string, Geometry*> m_geometries;

	std::string m_resourceSource;
	friend class ServiceLocator;
};