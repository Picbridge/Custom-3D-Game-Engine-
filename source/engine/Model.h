#pragma once

struct ModelNode
{
	std::string name;
	glm::mat4 transform;
	aiMatrix4x4 mTransformation;
	std::vector<ModelNode*> children;
	int numChildren = 0;
	bool hasParent = false;
};

class Animation;
class Bone;

class Model
{
public:
	Model() {};
	~Model();
	Model(std::string path)
	{
		loadModel(path);
	}
	void Draw(Shader& shader, glm::mat4 projection, glm::mat4 view, glm::vec3 lightPos);
	void DrawSkeleton(glm::mat4 projection, glm::mat4 view, ModelNode* node, glm::mat4 parentTransform = glm::mat4(1.0f));
	void Update(float deltaTime);
	ModelNode* origin;
	const aiScene* scene;
	bool hasAnimation = false;
	std::map<std::string, Bone> boneMap;
private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<MeshTexture> textures_loaded;
	//RenderComponent* sphere;
	GLuint vao, vbo;
	Shader *shader;
	glm::vec3 vertices[2];
	std::vector<Animation*> animations;

	void UpdateHierarchy(ModelNode* node);
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene, ModelNode* myNode);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	glm::mat4 AssimpToGLM(const aiMatrix4x4& assimpMatrix);
	void DrawLine(glm::vec3 startPoint, glm::vec3 endPoint, glm::mat4 projection, glm::mat4 view);
	void SetUpBuffers();
	glm::mat4 QuaternionToMatrix(const aiQuaternion& quat);

};

