#pragma once

class Skybox : public Node
{
public:
	Skybox(const char* filename);
	~Skybox();

	//TODO: Add change skybox function that takes in a texture from the resource manager

	void LoadSkybox(const char* filename);
	void Render();

private:
	Geometry* m_pGeometry;
	Shader* m_pShader;
	
	std::vector<GLuint> m_skybox;

	void extractFace(
		std::vector<unsigned char>& face, 
		unsigned faceWidth, 
		unsigned faceHeight, 
		unsigned startX, 
		unsigned startY, 
		unsigned imageWidth, 
		const std::vector<unsigned char>& image);
};