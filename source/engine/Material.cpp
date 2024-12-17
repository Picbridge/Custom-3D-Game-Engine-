#include "pch.h"
#include "resourcemanager/ResourceManager.h"

Material::Material() : m_pDiffuse(nullptr), m_pShader(nullptr), m_pSpecular(nullptr)
{
	//Set shader as default shader
	ServiceLocator* serviceLocator = &SERVICE_LOCATOR;
	m_pShader = SERVICE_LOCATOR.GetResourceManager()->GetShader("Default");
	m_data.color = glm::vec3(1.0f);
	m_data.shininess = 0.f;
}

Material::Material(Shader* pShader) : m_pDiffuse(nullptr), m_pShader(nullptr), m_pSpecular(nullptr)
{
	m_pShader = pShader;
	m_data.color = glm::vec3(1.0f);
	m_data.shininess = 0.f;
}

Material::~Material()
{
}

void Material::SetShader(Shader* pShader)
{
	m_pShader = pShader;
}

void Material::SetShader(const std::string name)
{
	m_pShader = SERVICE_LOCATOR.GetResourceManager()->GetShader(name);
}

void Material::SetTextureDiffuse(Texture* texture)
{
	m_pDiffuse = texture;

	glm::vec3* dataDiff;

	m_pDiffuse->AssignTextureToDest(dataDiff);
	glGenTextures(1, &m_data.diffuse);
	glBindTexture(GL_TEXTURE_2D, m_data.diffuse);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_pDiffuse->GetWidth(), m_pDiffuse->GetHeight(), 0, GL_RGB, GL_FLOAT, dataDiff);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Material::SetTextureSpecular(Texture* texture)
{
	m_pSpecular = texture;

	glm::vec3* dataSpec;

	m_pSpecular->AssignTextureToDest(dataSpec);
	glGenTextures(1, &m_data.specular);
	glBindTexture(GL_TEXTURE_2D, m_data.specular);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_pSpecular->GetWidth(), m_pSpecular->GetHeight(), 0, GL_RGB, GL_FLOAT, dataSpec);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Material::SetColor(glm::vec3 color)
{
	m_data.color = color;
}

void Material::SetColor(float r, float g, float b)
{
	m_data.color = glm::vec3(r, g, b);
}


void Material::SetShininess(float shininess)
{
	m_data.shininess = shininess;
}

//This may be updated for general use of the custom shader
void Material::SetupUniformData()
{
	m_pShader->SetUniform("material.color", m_data.color);
	m_pShader->SetUniform("material.shininess", m_data.shininess);
	m_pShader->SetUniform("hasDiffuse", m_pDiffuse != nullptr);
	m_pShader->SetUniform("hasSpecular", m_pSpecular != nullptr);
}

void Material::Bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_data.diffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_data.specular);
}

void Material::Unbind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
}