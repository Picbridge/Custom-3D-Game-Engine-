#include "pch.h"
#include "resourcemanager/ResourceManager.h"
#include "Camera.h"

static void PrintMatrix(const glm::mat4& mat) {
	std::cout << "P" << std::endl;
	std::cout << std::fixed << std::setprecision(3); // Optional formatting
	for (int i = 0; i < 4; ++i) { // Loop over rows
		for (int j = 0; j < 4; ++j) { // Loop over columns
			std::cout << std::setw(10) << mat[j][i] << " ";
		}
		std::cout << "\n"; // Newline after each row
	}
	std::cout << std::endl; // Additional newline for separation
}

ParticleSystem::ParticleSystem()
{
	m_ParticlePool.resize(1000);
}

void ParticleSystem::Update(float deltaTime)
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.active)
			continue;
		if (particle.lifeRemaining <= 0.0f)
		{
			particle.active = false;
			continue;
		}
		particle.lifeRemaining -= deltaTime;
		particle.position += particle.velocity * deltaTime;
		particle.rotation += 0.01f * deltaTime;
	}
}

void ParticleSystem::Render()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ResourceManager* manager = SERVICE_LOCATOR.GetResourceManager();
	m_ParticleShader = manager->GetShader("Particle");
	Geometry* geometry = manager->GetGeometry("Cube");
	int count = 0;
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.active)
			continue;
		// Fade away particles
		float life = particle.lifeRemaining / particle.lifeTime;
		glm::vec4 color = glm::lerp(particle.colorEnd, particle.colorBegin, life);
		//color.a = color.a * life;

		float size = glm::lerp(particle.sizeEnd, particle.sizeBegin, life);

		// Render
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.position.x, particle.position.y, particle.position.z })
			* glm::rotate(glm::mat4(1.0f), particle.rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), glm::vec3(size));
		//PrintMatrix(transform);
		m_ParticleShader->Use();
		geometry->Bind(m_ParticleShader);
		m_ParticleShader->SetUniform("u_Proj", Camera::GetInstance()->m_worldProjection);
		m_ParticleShader->SetUniform("u_View", Camera::GetInstance()->m_worldView);
		m_ParticleShader->SetUniform("u_Model", transform);
		m_ParticleShader->SetUniform("u_Color", color);
		geometry->Render();
		m_ParticleShader->Unuse();
		geometry->Unbind();
	}
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.active = true;
	particle.position = particleProps.position;
	particle.rotation = Random::Float() * 2.0f * glm::pi<float>();

	particle.velocity = particleProps.velocity;
	particle.velocity.x += particleProps.velocityVariation.x * (Random::Float() - 0.5f);
	particle.velocity.y += particleProps.velocityVariation.y * (Random::Float() - 0.5f);
	particle.velocity.z += particleProps.velocityVariation.z * (Random::Float() - 0.5f);

	particle.colorBegin = particleProps.colorBegin;
	particle.colorEnd = particleProps.colorEnd;

	particle.lifeTime = particleProps.lifetime;
	particle.lifeRemaining = particleProps.lifetime;
	particle.sizeBegin = particleProps.sizeBegin + particleProps.sizeVariation * (Random::Float() - 0.5f);
	particle.sizeEnd = particleProps.sizeEnd;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}
