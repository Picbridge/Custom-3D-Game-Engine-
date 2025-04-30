#pragma once
class ParticleShader :public Shader
{
public:
    ParticleShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
        : Shader(vertexPath, fragmentPath, geometryPath) {
    }

    void PassShaderData() override {}
};

