#pragma once
class DebugShader : public Shader
{
public:
	DebugShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
		: Shader(vertexPath, fragmentPath, geometryPath) {
	}

    void PassShaderData() override {}
};

