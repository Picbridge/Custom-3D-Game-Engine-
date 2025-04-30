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
	void Prepass(const CameraComponent* camera);
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

	//@brief sets the tiling position of the texture
	//@param pos : tiling position of the texture
	void SetTexTilingPos(glm::vec2 pos) { m_texTilingPos = pos; }

	//@brief sets the tiling size of the texture
	//@param size : tiling size of the texture
	void SetTexTilingScale(glm::vec2 size) { m_texTilingScale = size; }

	//--------------------------------
	//Getters
	//--------------------------------
	
	//@brief Returns the material of the current game object
	Material* GetMaterial() { return m_pMaterial; }
	//@brief Returns the geometry of the current game object
	Geometry* GetGeometry() { return m_pGeometry; }
	//@brief Returns the shader of the current game object
	Shader* GetShader() { return m_pMaterial->GetShader(); }

	//@brief returns the tiling position of the texture
	//@return glm::vec2 the tiling position of the texture
	glm::vec2 GetTexTilingPos() { return m_texTilingPos; }

	//@brief returns the tiling size of the texture
	//@return glm::vec2 the tiling size of the texture
	glm::vec2 GetTexTilingScale() { return m_texTilingScale; }

private:
	Geometry* m_pGeometry;
	Material* m_pMaterial;
	Model* m_pModel;
	Shader* m_pPrepassShader;

	glm::vec2 m_texTilingPos;
	glm::vec2 m_texTilingScale;

	int m_currentWidth;
	int m_currentHeight;

	GLuint m_prepassColorTex;
	GLuint m_prepassFBO;
	GLuint m_depthBuffer;

	GLuint m_LineVAO, m_LineVBO;
	glm::vec3 m_LineVertices[2];
	glm::vec4 m_prepassColor;
	void defineMember() override;
};

