#pragma once

class ShaderLoader
{
public:
	//@brief Load the shader from the path provided
	static GLuint Load(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
private:
	//@brief Check the shader compile errors
	static void checkCompileErrors(GLuint shader, std::string type);

};