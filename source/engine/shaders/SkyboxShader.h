#pragma once
class SkyboxShader :public Shader
{
public:
    SkyboxShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
        : Shader(vertexPath, fragmentPath, geometryPath) {
    }

    void PassShaderData() override {}

};

