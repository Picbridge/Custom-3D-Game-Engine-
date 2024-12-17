#pragma once

struct MeshVertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct MeshTexture {
	unsigned int id;
	std::string type;
	std::string path;
};

struct MeshTransform {
	glm::mat4 position;
	glm::mat4 rotation;
	glm::mat4 scale;
};

class Mesh
{
public:
	std::vector<MeshVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<MeshTexture> textures;
	MeshTransform transform;

	Mesh(std::vector<MeshVertex> _vertices, std::vector<unsigned int> _indices, std::vector<MeshTexture> _textures);
	void Draw(Shader& shader, glm::mat4 projection, glm::mat4 view, glm::vec3 lightPos);

private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
	void SetUpUniforms(Shader target, glm::mat4 projection, glm::mat4 view, glm::vec3 lightPos);
};



