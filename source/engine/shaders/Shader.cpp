#include "../pch.h"
#include "ShaderLoader.h"
#include "../Node.h"
#include "../LightComponent.h"
#include "../../include/TransformComponent.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
    : m_id(0), m_uniformCache(std::unordered_map<std::string, GLint>()), m_pCurrObject(nullptr), m_pCurrLight(nullptr), m_pCurrMaterial(nullptr)
{
	m_id = ShaderLoader::Load(vertexPath, fragmentPath, geometryPath);
}

Shader::~Shader()
{
	Unuse();
}

void Shader::Use()
{
	glUseProgram(m_id);
	Utils::GetGLError();
}

void Shader::Unuse()
{
	glUseProgram(0);
	Utils::GetGLError();
}

void Shader::PassObject(Node* object) 
{
	m_pCurrObject = object;
}

void Shader::PassLight(LightComponent* light)
{
	if (light == nullptr)
	{
		return;
	}

	m_pCurrLight = light;
}

void Shader::PassMaterial(Material* material)
{
	m_pCurrMaterial = material;
}

GLint Shader::GetAttributeLocation(const std::string& name)
{
	GLint location = glGetAttribLocation(m_id, name.c_str());
	Utils::GetGLError();
	return location;
}

void Shader::ClearUniformCache()
{
	m_uniformCache.clear();
}