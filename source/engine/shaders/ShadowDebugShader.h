#pragma once
class ShadowDebugShader : public Shader
{
public:
    ShadowDebugShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
        : Shader(vertexPath, fragmentPath, geometryPath) {
    }

    void PassShaderData() override {}
};

