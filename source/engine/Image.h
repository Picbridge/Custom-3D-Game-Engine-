#pragma once
class Image
{
public:
	Image(std::string name);
	~Image();
	void DrawImage(float alpha = 1.0f);
private:
	GLuint texture;
	GLuint VAO, VBO, EBO;
	GLuint shaderProgram;
};

