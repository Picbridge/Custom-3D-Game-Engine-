#include "pch.h"
#include "headers.h"

Geometry::Geometry() : m_VAO(0), m_VBO(0), m_IBO(0), m_UV(0), m_NORMALBUFFER(0), m_uvType(PLANAR)
{
	glDisable(GL_DEBUG_OUTPUT);
	genBuffers();
}

Geometry::Geometry(const char* path) : m_VAO(0), m_VBO(0), m_IBO(0), m_UV(0), m_NORMALBUFFER(0), m_uvType(PLANAR)
{
	glDisable(GL_DEBUG_OUTPUT);
	if (!LoadGeometry(path))
		std::cerr << "Geometry::LoadGeometry() - Failed to load geometry" << std::endl;

	genBuffers();
}

Geometry::~Geometry()
{
	CleanUpBuffers();
}

bool Geometry::LoadGeometry(const char* path)
{
	return ObjLoader::LoadObj(path, m_vertexData, m_normalData, m_uvInfo);
}

void Geometry::Bind(Shader* shader)
{
	glBindVertexArray(m_VAO);

	// Bind vertex position buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertexData.vertex_buffer.size() * sizeof(glm::vec3), &m_vertexData.vertex_buffer[0], GL_STATIC_DRAW);
	GLint positionLocation = shader->GetAttributeLocation("aPos");
	if (positionLocation == -1)
		std::cerr << "Geometry::Bind() - Position attribute not found" << std::endl;
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(
		positionLocation,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0);

	// Bind normal buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_NORMALBUFFER);
	glBufferData(GL_ARRAY_BUFFER, m_normalData.vertex_normal_buffer.size() * sizeof(glm::vec3), &m_normalData.vertex_normal_buffer[0], GL_STATIC_DRAW);
	GLint normalLocation = shader->GetAttributeLocation("aNormal");
	if (normalLocation != -1)
	{
		glEnableVertexAttribArray(normalLocation);
		glVertexAttribPointer(
			normalLocation,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0);
	}
	

	GLint texCoordLocation = shader->GetAttributeLocation("aTexCoords");
	if (texCoordLocation != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_UV); // Bind the buffer for UV coordinates
		// Set the correct UV buffer based on the mapping type
		switch (m_uvType)
		{
		case CYLINDRICAL:
			glBufferData(GL_ARRAY_BUFFER, m_uvInfo.Cylindrical.size() * sizeof(glm::vec2), &m_uvInfo.Cylindrical[0], GL_STATIC_DRAW);
			break;
		case SPHERICAL:
			glBufferData(GL_ARRAY_BUFFER, m_uvInfo.Spherical.size() * sizeof(glm::vec2), &m_uvInfo.Spherical[0], GL_STATIC_DRAW);
			break;
		case PLANAR:
			glBufferData(GL_ARRAY_BUFFER, m_uvInfo.Planar.size() * sizeof(glm::vec2), &m_uvInfo.Planar[0], GL_STATIC_DRAW);
			break;
		case CUBE:
			glBufferData(GL_ARRAY_BUFFER, m_uvInfo.Cube.size() * sizeof(glm::vec2), &m_uvInfo.Cube[0], GL_STATIC_DRAW);
			break;
		}

		glEnableVertexAttribArray(texCoordLocation);
		glVertexAttribPointer(
			texCoordLocation,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(glm::vec2),
			(void*)0);
	}

	// Bind index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vertexData.index_buffer.size() * sizeof(unsigned int), &m_vertexData.index_buffer[0], GL_STATIC_DRAW);

}

void Geometry::Unbind()
{
	// Unbind the VAO
	glBindVertexArray(0);

	// Unbind the VBO, Normal buffer, and UV buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind the IBO (index buffer)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Geometry::CleanUpBuffers()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);
	glDeleteBuffers(1, &m_UV);
	glDeleteBuffers(1, &m_NORMALBUFFER);
	glDeleteVertexArrays(1, &m_VAO);
}

void Geometry::Render()
{
	glDrawElements(GL_TRIANGLES, (GLsizei)m_vertexData.index_buffer.size(), GL_UNSIGNED_INT, nullptr);
}

void Geometry::SetUVType(UV_TYPE type)
{
	m_uvType = type;
}

void Geometry::genBuffers()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);
	glGenBuffers(1, &m_UV);
	glGenBuffers(1, &m_NORMALBUFFER);

}