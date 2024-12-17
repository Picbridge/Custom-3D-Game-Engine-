#pragma once

class Texture
{
public:
	Texture(const char* filename);

	//--------------------------------
	//Image Loading
	//--------------------------------

	//@brief Assign the image buffer to destination
	//@param dest : Destination to store the image buffer
	void AssignTextureToDest(glm::vec3*& dest);

	//@brief Load the texture from the file
	//@param filename : Path and file name of the texture
	void LoadTexture(const char* filename);

	//--------------------------------
	//Getters
	//--------------------------------

	//@brief Returns the width of the image
	//@return unsigned int width of the image
	unsigned int GetWidth() { return m_width; }

	//@brief Returns the height of the image
	//@return unsigned int height of the image
	unsigned int GetHeight() { return m_height; }

	//@brief Returns the image
	//@return glm::vec3* : Image buffer
	glm::vec3* GetImage() { return m_pImage.get(); }


private:
	int m_size;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_bytesPerPix;
	unsigned int m_bitDepth;

	std::unique_ptr <glm::vec2> m_pTexcoord;
	std::unique_ptr<glm::vec3> m_pImage;

	//@brief Reads the png file and extracts data
	//@param filename : Path and file name of the png file
	void loadPNGFile(const char* filename);
};
