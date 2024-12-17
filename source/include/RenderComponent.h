#pragma once
class Model;

class RenderComponent : public Component
{
public:
	RenderComponent();
	~RenderComponent();

	void Init() override;
	void Update() override;
	void Shutdown() override;

	//@brief Render the 
	// object
	void Render();
	void Render(Shader* shader);
	void DrawCollider();
	void DrawVelocity();

	//@brief Sets the color of the current material
	//@param color : color of range [0, 1]
	void SetColor(glm::vec3 color);

	//@brief Sets the color of the current material
	//@param r : color of range [0, 1]
	//@param g : color of range [0, 1]
	//@param b : color of range [0, 1]
	void SetColor(float r, float g, float b);

	//@brief Set the current material
	//@param pMaterial : the material to set
	void SetMaterial(Material* pMaterial);

	//@brief Set the current material
	//@param name : Material name that will be stored in the material map
	void SetMaterial(const std::string name);

	//@brief Replace the current shader with user defined shader
	//@param shader : user defined shader
	void SetShader(Shader* pShader);

	//@brief Replace the current shader with user defined shader
	//@param name : Shader name that will be stored in the shader map
	void SetShader(const std::string name);

	//@brief Replace the cuurent geometry with user defined geometry
	//@param pGeometry : the geometry to set
	void SetGeometry(Geometry* pGeometry);

	//@brief Geometry setter overload to set geometry by class name
	//@param geometryName : Name of the geometry class to set
	void SetGeometry(const std::string& geometryName);

	//@brief Set the UV type of the current geometry
	//@param type : UV type of the geometry
	void SetUVType(UV_TYPE type);

	//--------------------------------
	//Getters
	//--------------------------------
	
	//@brief Returns the material of the current game object
	Material* GetMaterial() { return m_pMaterial; }
	//@brief Returns the geometry of the current game object
	Geometry* GetGeometry() { return m_pGeometry; }
	//@brief Returns the shader of the current game object
	Shader* GetShader() { return m_pMaterial->GetShader(); }


private:
	Geometry* m_pGeometry;
	Material* m_pMaterial;
	Model* m_pModel;

	GLuint m_LineVAO, m_LineVBO;
	glm::vec3 m_LineVertices[2];

	void defineMember() override;
};

