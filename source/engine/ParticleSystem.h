#pragma once

struct ParticleProps
{
	glm::vec3 position;
	glm::vec3 velocity, velocityVariation;
	glm::vec4 colorBegin, colorEnd;
	float sizeBegin, sizeEnd, sizeVariation;
	float lifetime = 1.0f;
};

class ParticleSystem
{
public:
	ParticleSystem();

	void Update(float deltaTime);
	void Render();

	void Emit(const ParticleProps& particleProps);
private:
	struct Particle
	{
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec4 colorBegin, colorEnd;
		float rotation = 0.0f;
		float sizeBegin, sizeEnd;
		float lifeTime = 1.0f;
		float lifeRemaining = 0.0f;
		bool active = false;
	};

	std::vector<Particle> m_ParticlePool;
	uint32_t m_PoolIndex;

	GLuint m_QuadVA = 0;
	Shader* m_ParticleShader;
	GLint m_ParticleShaderViewProj, m_ParticleShaderTransform, m_ParticleShaderColor;
};

