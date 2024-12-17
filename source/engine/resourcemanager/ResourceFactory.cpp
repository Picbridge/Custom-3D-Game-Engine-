#include "../pch.h"
#include "ResourceFactory.h"
#include "ResourceManager.h"

std::unique_ptr<ResourceFactory> ResourceFactory::instance = nullptr;

ResourceFactory* ResourceFactory::GetInstance()
{
	if (!instance)
		instance = std::unique_ptr<ResourceFactory>( new ResourceFactory());

	return instance.get();
}

void ResourceFactory::CreateAllResources(const char* source)
{
	FILE* fp;
	fopen_s(&fp, source, "rb");
	if (!fp)
	{
		std::cout << "Failed to load data source" << std::endl;
		exit(EXIT_FAILURE);
	}

	char readBuffer[8192];
	rapidjson::FileReadStream inputStream(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document resourceDoc;
	resourceDoc.ParseStream(inputStream);

	const rapidjson::Value& texture = resourceDoc.FindMember("Texture")->value;
	for (rapidjson::Value::ConstMemberIterator it = texture.MemberBegin(); it != texture.MemberEnd(); ++it)
		createResource(it, ResourceType::TEXTURE);

	const rapidjson::Value& shader = resourceDoc.FindMember("Shader")->value;
	for (rapidjson::Value::ConstMemberIterator it = shader.MemberBegin(); it != shader.MemberEnd(); ++it)
		createResource(it, ResourceType::SHADER);

	const rapidjson::Value& material = resourceDoc.FindMember("Material")->value;
	for (rapidjson::Value::ConstMemberIterator it = material.MemberBegin(); it != material.MemberEnd(); ++it)
		createResource(it, ResourceType::MATERIAL);

	const rapidjson::Value& geometry = resourceDoc.FindMember("Geometry")->value;
	for (rapidjson::Value::ConstMemberIterator it = geometry.MemberBegin(); it != geometry.MemberEnd(); ++it)
		createResource(it, ResourceType::GEOMETRY);

	fclose(fp);
}

void ResourceFactory::createResource(rapidjson::Value::ConstMemberIterator member, ResourceType type)
{
	if (type == ResourceType::TEXTURE)
	{
		const char* path = member->value["path"].GetString();
		Texture* texture = new Texture(path);
		SERVICE_LOCATOR.GetResourceManager()->AddTexture(member->name.GetString(), texture);
	}
	else if (type == ResourceType::SHADER)
	{
		const char* vertexPath = member->value[GameResourceConstants::VERTEX_SHADER.data()].GetString();
		const char* fragmentPath = member->value[GameResourceConstants::FRAGMENT_SHADER.data()].GetString();
		if (member->value.HasMember(GameResourceConstants::GEOMETRY_SHADER.data()))
		{
			const char* geometryPath = member->value[GameResourceConstants::GEOMETRY_SHADER.data()].GetString();
			Shader* shader = new Shader(vertexPath, fragmentPath, geometryPath);
			SERVICE_LOCATOR.GetResourceManager()->AddShader(member->name.GetString(), shader);
		}
		else
		{
			Shader* shader = new Shader(vertexPath, fragmentPath);
			SERVICE_LOCATOR.GetResourceManager()->AddShader(member->name.GetString(), shader);
		}
	}
	else if (type == ResourceType::MATERIAL)
	{
		Material* material = new Material();
		if (member->value.HasMember(GameResourceConstants::SHADER.data()))
			material->SetShader(SERVICE_LOCATOR.GetResourceManager()->GetShader(member->value[GameResourceConstants::SHADER.data()].GetString()));
		if (member->value.HasMember(GameResourceConstants::DIFFUSE.data()))
			material->SetTextureDiffuse(SERVICE_LOCATOR.GetResourceManager()->GetTexture(member->value[GameResourceConstants::DIFFUSE.data()].GetString()));
		if (member->value.HasMember(GameResourceConstants::SPECULAR.data()))
			material->SetTextureSpecular(SERVICE_LOCATOR.GetResourceManager()->GetTexture(member->value[GameResourceConstants::SPECULAR.data()].GetString()));

		SERVICE_LOCATOR.GetResourceManager()->AddMaterial(member->name.GetString(), material);
	}
	else if (type == ResourceType::GEOMETRY)
	{
		const char* path = member->value["path"].GetString();
		Geometry* geometry = new Geometry(path);
		SERVICE_LOCATOR.GetResourceManager()->AddGeometry(member->name.GetString(), geometry);
	}
}