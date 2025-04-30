#pragma once
class ShadowShader :public Shader
{
public:
    ShadowShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
        : Shader(vertexPath, fragmentPath, geometryPath) {
    }

    void PassShaderData() override {}
};

