#include "../pch.h"
#include "ResourceFactory.h"
#include "ResourceManager.h"

#include "../shaders/DefaultShader.h"
#include "../shaders/LineShader.h"
#include "../shaders/ShadowShader.h"
#include "../shaders/DebugShader.h"
#include "../shaders/ShadowDebugShader.h"
#include "../shaders/ParticleShader.h"
#include "../shaders/SkyboxShader.h"
#include "../shaders/CelShader.h"


std::unique_ptr<ResourceFactory> ResourceFactory::instance = nullptr;

ResourceFactory* ResourceFactory::GetInstance()
{
	if (!instance)
		instance = std::unique_ptr<ResourceFactory>(new ResourceFactory());

	return instance.get();
}

void ResourceFactory::CreateDefaultResources()
{
	std::filesystem::path contentPath = Utils::GetExecutableDirectory().parent_path().parent_path().parent_path() / "content";
	std::filesystem::path shaderPath = contentPath / "code" / "shader";
	std::filesystem::path modelPath = contentPath / "art" / "obj";
	std::filesystem::path texturePath = contentPath / "art" / "texture";

	// Load the default resources
	loadResources(shaderPath.string());
	loadResources(modelPath.string());
	loadResources(texturePath.string());

	Material* material = new Material();
	SERVICE_LOCATOR.GetResourceManager()->AddMaterial("Default", material);
}

void ResourceFactory::CreateAllResources()
{
	std::filesystem::path exeDir = Utils::GetExecutableDirectory();
	std::filesystem::path gameResourcesPath = exeDir.parent_path().parent_path().parent_path() / "content" / "game" / "resources";

	if (!std::filesystem::exists(gameResourcesPath)) {
		throw std::runtime_error("Resources directory not found: " + gameResourcesPath.string());
		std::cerr << gameResourcesPath.string() << " could not be resolved by ResourceFactory::CreateAllResources()." << std::endl;
	}

	loadResources(gameResourcesPath.string());

	// Save path for exporting
	SERVICE_LOCATOR.GetResourceManager()->SetResourceSource(gameResourcesPath.string().append("\\").c_str());
}

void ResourceFactory::loadResources(std::string directory)
{
	std::unordered_map<std::string, std::pair<std::string, std::string>> shaderFiles;
	std::vector<std::filesystem::path> materialFiles;

	for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
	{
		if (entry.is_regular_file())
		{
			const auto& path = entry.path();
			const auto& extension = path.extension().string();

			if (extension == ".vert" || extension == ".frag")//Shader
			{
				std::string shaderName = path.stem().string();
				if (extension == ".vert")
					shaderFiles[shaderName].first = path.string();
				else if (extension == ".frag")
					shaderFiles[shaderName].second = path.string();
			}
			else if (extension == ".obj")//Geometry
			{
				std::string geometryName = path.stem().string();
				Geometry* geometry = new Geometry(path.string().c_str());
				SERVICE_LOCATOR.GetResourceManager()->AddGeometry(geometryName, geometry);
			}
			else if (extension == ".png")//Texture
			{
				std::string textureName = path.stem().string();
				Texture* texture = new Texture(path.string().c_str());
				SERVICE_LOCATOR.GetResourceManager()->AddTexture(textureName, texture);
			}
			else if (extension == ".json")//Material
			{
				materialFiles.push_back(path);
			}
		}
	}

	createShaders(shaderFiles);
	createMaterials(materialFiles);
}

void ResourceFactory::createShaders(const std::unordered_map<std::string, std::pair<std::string, std::string>>& shaderFiles) {
	for (const auto& [name, paths] : shaderFiles)
	{
		const auto& [vertPath, fragPath] = paths;
		if (!vertPath.empty() && !fragPath.empty())
		{
			std::string geomPathStr = vertPath.substr(0, vertPath.find_last_of('.')) + ".geom";
			const char* geomPath = geomPathStr.c_str();

			std::string key = std::string(name) + "Shader";
			auto it = ShaderDictionary::shaderStore.find(key);
			if (it != ShaderDictionary::shaderStore.end())
			{
				Shader* shader = nullptr;

				// Use std::visit to extract the stored pointer and deduce its type.
				std::visit([&](auto shaderPtr)
					{
						// Remove any cv/ref qualifiers.
						using PtrType = std::remove_cv_t<std::remove_reference_t<decltype(shaderPtr)>>;
						// Get the underlying shader class.
						using ShaderClass = std::remove_pointer_t<PtrType>;

						// Construct the shader based on whether geomPath exists.
						if (std::filesystem::exists(geomPath))
						{
							shader = new ShaderClass(vertPath.c_str(), fragPath.c_str(), geomPath);
						}
						else
						{
							shader = new ShaderClass(vertPath.c_str(), fragPath.c_str());
						}
					}, it->second);

				// Add the shader to your resource manager.
				SERVICE_LOCATOR.GetResourceManager()->AddShader(name, shader);
			}
		}
	}
}

void ResourceFactory::createMaterials(const std::vector<std::filesystem::path>& materialFiles)
{
	for (const auto& path : materialFiles)
	{
		FILE* fp;
		fopen_s(&fp, path.string().c_str(), "rb");
		if (!fp)
		{
			std::cout << "Failed to load data source" << std::endl;
			exit(EXIT_FAILURE);
		}

		char readBuffer[8192];
		rapidjson::FileReadStream inputStream(fp, readBuffer, sizeof(readBuffer));

		rapidjson::Document matDoc;
		matDoc.ParseStream(inputStream);
		fclose(fp);

		if (matDoc.HasParseError())
			std::cerr << "Error parsing the material json file" << std::endl;

		if (!matDoc.IsObject() || matDoc.MemberCount() == 0)
		{
			std::cerr << "Invalid material json file. File name:" << path.string() << std::endl;
			continue;
		}

		std::string materialName = path.stem().string();
		Material* material = new Material();

		if (matDoc.HasMember("Shader"))
			material->SetShader(SERVICE_LOCATOR.GetResourceManager()->GetShader(matDoc["Shader"].GetString()));
		if (matDoc.HasMember("Textures"))
		{
			const auto& textures = matDoc["Textures"];
			if (textures.HasMember("Diffuse"))
				material->SetTextureDiffuse(SERVICE_LOCATOR.GetResourceManager()->GetTexture(textures["Diffuse"].GetString()));
			if (textures.HasMember("Specular"))
				material->SetTextureSpecular(SERVICE_LOCATOR.GetResourceManager()->GetTexture(textures["Specular"].GetString()));
		}
		if (matDoc.HasMember("Color"))
		{
			const auto& color = matDoc["Color"].GetArray();
			material->SetColor(glm::vec3(color[0].GetFloat(), color[1].GetFloat(), color[2].GetFloat()));
		}
		if (matDoc.HasMember("Shininess"))
			material->SetShininess(matDoc["Shininess"].GetFloat());
		if (matDoc.HasMember("Alpha"))
			material->SetAlpha(matDoc["Alpha"].GetFloat());

		SERVICE_LOCATOR.GetResourceManager()->AddMaterial(materialName, material);
	}
}