#include "pch.h"
#include "ShaderLoader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	m_id = ShaderLoader::Load(vertexPath, fragmentPath, geometryPath);
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