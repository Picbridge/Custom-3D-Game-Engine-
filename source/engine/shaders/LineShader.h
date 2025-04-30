#pragma once
class LineShader : public Shader
{
public:
    LineShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
        : Shader(vertexPath, fragmentPath, geometryPath) {
    }

    void PassShaderData() override {}
};

