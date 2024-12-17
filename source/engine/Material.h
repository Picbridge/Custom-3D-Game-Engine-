#pragma once
//TODO: This may be updated for general use of the custom shader
struct MaterialData
{
	glm::vec3 color;
	GLuint diffuse;
	GLuint specular;
	float shininess;
};

class Material
{
public:
	Material();
	Material(Shader* pShader);
	~Material();

	//--------------------------------
	//Material control
	//--------------------------------

	//@brief replace the current shader with user defined shader
	//@param shader : user defined shader
	void SetShader(Shader* shader);

	//@brief replace the current shader with user defined shader
	// @param name : Shader name that will be stored in the shader map
	void SetShader(const std::string name);

	//@brief apply the diffuse texture to current material
	//@param texture : the diffuse texture
	void SetTextureDiffuse(Texture* texture);

	//@brief apply the specular texture to current material
	//@param texture : the specular texture
	void SetTextureSpecular(Texture* texture);

	//@brief sets the color of the current material
	//@param color : color of range [0, 255]
	void SetColor(glm::vec3 color);

	//@brief sets the color of the current material
	//@param r : color of range [0, 255]
	//@param g : color of range [0, 255]
	//@param b : color of range [0, 255]
	void SetColor(float r, float g, float b);

	//@brief sets the shininess of the current material
	//@param shininess : shininess of the material
	void SetShininess(float shininess);
	 
	//--------------------------------
	//Render control
	//--------------------------------

	//@brief passes the material data to the shader
	void SetupUniformData();

	//@brief binds the texture set before
	void Bind();

	//@brief unbind the current texture for next object render
	void Unbind();

	//--------------------------------
	//Getters
	//--------------------------------

	//@brief sets the color of the current material
	const glm::vec3& GetColor() const { return m_data.color; }

	//@brief returns the shininess of the current material
	float GetShininess() const { return m_data.shininess; }

	//@brief returns the shader of the current material
	//@return Shader* the shader of the current material
	Shader* GetShader() { return m_pShader; }

	//@brief returns the color of the current material
	//@return glm::vec3 the color of the current material
	glm::vec3 GetColor() { return m_data.color; }
private:
	MaterialData m_data;
	Shader* m_pShader;

	//TODO: Should be removed after Texture Manager integration
	Texture* m_pDiffuse;
	Texture* m_pSpecular;
};