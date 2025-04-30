#include "pch.h"
#include "resourcemanager/ResourceManager.h"

Material::Material() : m_pTexDiffuse(nullptr), m_pShader(nullptr), m_pTexSpecular(nullptr)
{
	//Set shader as default shader
	ServiceLocator* serviceLocator = &SERVICE_LOCATOR;
	m_pShader = SERVICE_LOCATOR.GetResourceManager()->GetShader("Default");
	m_data.color = glm::vec3(1.0f);
	m_data.shininess = 0.f;
	m_data.alpha = 1.0f;
}

Material::Material(Shader* pShader) : m_pTexDiffuse(nullptr), m_pShader(nullptr), m_pTexSpecular(nullptr)
{
	m_pShader = pShader;
	m_data.color = glm::vec3(1.0f);
	m_data.shininess = 0.f;
	m_data.alpha = 1.0f;
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
	m_pTexDiffuse = texture;

	glm::vec3* dataDiff;

	m_pTexDiffuse->AssignTextureToDest(dataDiff);
	glGenTextures(1, &m_data.diffuse);
	glBindTexture(GL_TEXTURE_2D, m_data.diffuse);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_pTexDiffuse->GetWidth(), m_pTexDiffuse->GetHeight(), 0, GL_RGB, GL_FLOAT, dataDiff);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Material::SetTextureSpecular(Texture* texture)
{
	m_pTexSpecular = texture;

	glm::vec3* dataSpec;

	m_pTexSpecular->AssignTextureToDest(dataSpec);
	glGenTextures(1, &m_data.specular);
	glBindTexture(GL_TEXTURE_2D, m_data.specular);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_pTexSpecular->GetWidth(), m_pTexSpecular->GetHeight(), 0, GL_RGB, GL_FLOAT, dataSpec);
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

void Material::Bind()
{
	if (m_pTexDiffuse == nullptr)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	if (m_pTexSpecular == nullptr)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Material::Unbind()
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
}