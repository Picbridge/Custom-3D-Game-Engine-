#pragma once
class CelShader : public Shader
{
public:
    CelShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
        : Shader(vertexPath, fragmentPath, geometryPath) {
    }

    void PassShaderData() override;

private:
	float m_ambient = 1.F;
	float shades = 4.5f;
	float smoothness = 0.65f;
	float edgeDiffuse = 0.09f;
	float edgeSpecular = 0.03f;
	float edgeSpecularOffset = 0.f;
	float edgeDistanceAttenuation = 0.09f;
	float edgeShadowAttenuation = 0.8f;
	float edgeRimOffset = 0.02f;
	float rimThreshold = 2.5f;
	float depthThreshold = 0.02f;
	float normalThreshold = 0.25f;
	float normalEdgeBias = 1.f;
	float outlineScale = 2.f;
};

