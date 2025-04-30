#pragma once
//TODO: This may be updated for general use of the custom shader
struct MaterialData
{
	glm::vec3 color;
	GLuint diffuse;
	GLuint specular;
	float shininess;
    float alpha;
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
	//@param name : the name of the diffuse texture
	void SetTextureDiffuse(const std::string& name);

	//@brief apply the specular texture to current material
	//@param name : the name of the specular texture
	void SetTextureSpecular(const std::string& name);

	//@brief apply the diffuse texture to current material
	//@param texture : the diffuse texture
	void SetTextureDiffuse(Texture* texture);

	//@brief apply the specular texture to current material
	//@param texture : the specular texture
	void SetTextureSpecular(Texture* texture);

	//@brief sets the color of the current material
	//@param color : color of range [0, 255]
	void SetColor(glm::vec3 color);

	void SetAlpha(float alpha) { m_data.alpha = alpha; }

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
	//@brief binds the texture set before
	void Bind();

	//@brief unbind the current texture for next object render
	void Unbind();

	//--------------------------------
	//Getters
	//--------------------------------

	//@brief gets the color of the current material
	const glm::vec3& GetColor() const { return m_data.color; }

	//@brief gets the alpha value of the current material
	float GetAlpha() const { return m_data.alpha; }

	//@brief returns the shininess of the current material
	float GetShininess() const { return m_data.shininess; }

	//@brief returns the diffuse texture of the current material
	GLuint GetTextureDiffuseID() { return m_data.diffuse; }
	
	//@brief returns the specular texture of the current material
	GLuint GetTextureSpecularID() { return m_data.specular; }
	
	//@brief returns the shader of the current material
	//@return Shader* the shader of the current material
	Shader* GetShader() { return m_pShader; }

	//@brief returns the diffuse texture of the current material
	//@return Texture* the diffuse texture of the current material
	inline Texture* GetTextureDiffuse() { return m_pTexDiffuse ? m_pTexDiffuse : nullptr; }

	//@brief returns the specular texture of the current material
	//@return Texture* the specular texture of the current material
	inline Texture* GetTextureSpecular() { return m_pTexSpecular ? m_pTexSpecular : nullptr; }

private:
	MaterialData m_data;
	Shader* m_pShader;

	//TODO: Should be removed after Texture Manager integration
	Texture* m_pTexDiffuse;
	Texture* m_pTexSpecular;


};