#include "pch.h"
#include "resourcemanager/ResourceManager.h"
#include "cameramanager/CameraComponent.h"
#include "cameramanager/CameraManager.h"
#include "ParticleComponent.h"

std::unique_ptr<ParticleManager> ParticleManager::instance = nullptr;

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

ParticleManager::ParticleManager()
{
	m_ParticlePool.resize(1000);
}

void ParticleManager::Update(float deltaTime)
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.active)
		{
			continue;
		}
		if (particle.lifeRemaining <= 0.0f)
		{
			particle.active = false;
			{
				continue;
			}
		}
		particle.lifeRemaining -= deltaTime;
		particle.position += particle.velocity * deltaTime;
		particle.rotation += 0.01f * deltaTime;
	}
}

void ParticleManager::Render()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ResourceManager* manager = SERVICE_LOCATOR.GetResourceManager();
	m_ParticleShader = manager->GetShader("Particle");
	Geometry* geometry = manager->GetGeometry("Plane");
	auto cameras = SERVICE_LOCATOR.GetCameraManager()->GetCamerasForRendering();
	auto frameBuffer = SERVICE_LOCATOR.GetWindowHandler()->FrameBuffer;

	for (const auto& camera : cameras)
	{
		if (!camera->IsActive())
		{
			continue;
		}
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.active)
			{
				continue;
			}
			auto viewport = camera->GetViewport();
			glViewport(viewport.X, viewport.Y, viewport.W, viewport.H);

			// Fade away particles
			float life = particle.lifeRemaining / particle.lifeTime;
			glm::vec4 color = glm::lerp(particle.colorEnd, particle.colorBegin, life);

			float size = glm::lerp(particle.sizeEnd, particle.sizeBegin, life);

			// Billboard calculation (camera-facing quad)
			glm::vec3 camPos = camera->GetPosition();
			glm::vec3 toCamera = glm::normalize(camPos - particle.position);

			// Basis vectors for the billboard (lookAt-style matrix)
			glm::vec3 up(0.0f, 1.0f, 0.0f);
			glm::vec3 right = glm::normalize(glm::cross(toCamera, up));
			glm::vec3 forward = glm::cross(right, toCamera);

			// Swapped Y and Z to match plane geometry
			glm::mat4 billboardRotation(1.0f);
			billboardRotation[0] = glm::vec4(right, 0.0f);      // Right (X-axis)
			billboardRotation[1] = glm::vec4(-toCamera, 0.0f);  // Forward (previously Z-axis ? Y-axis)
			billboardRotation[2] = glm::vec4(up, 0.0f);         // Up (previously Y-axis ? Z-axis)

			// Flip Z to fix inverted billboard
			// billboardRotation[2] = glm::vec4(toCamera, 0.0f);  // Flip Z-axis to face camera

			// Render
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.position.x, particle.position.y, particle.position.z })
				* billboardRotation
				* glm::scale(glm::mat4(1.0f), glm::vec3(size));

			m_ParticleShader->Use();
			geometry->Bind(m_ParticleShader);
			m_ParticleShader->PassShaderData("u_Proj", camera->GetProjectionMatrix(), "u_View", camera->GetViewMatrix(), "u_Model", transform, "u_Color", color);
			geometry->Render();
			m_ParticleShader->Unuse();
			geometry->Unbind();

			glViewport(0, 0, frameBuffer.Width, frameBuffer.Height);
		}
	}
}

void ParticleManager::Emit(const ParticleProps& particleProps)
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

void ParticleManager::Emit(ParticleComponent* particleComponent)
{
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.active = true;
	particle.position = particleComponent->GetPosition();
	particle.rotation = Random::Float() * 2.0f * glm::pi<float>();

	particle.velocity = particleComponent->GetVelocity();
	particle.velocity.x += particleComponent->GetVelocityVariation().x * (Random::Float() - 0.5f);
	particle.velocity.y += particleComponent->GetVelocityVariation().y * (Random::Float() - 0.5f);
	particle.velocity.z += particleComponent->GetVelocityVariation().z * (Random::Float() - 0.5f);

	particle.colorBegin = particleComponent->GetColorBegin();
	particle.colorEnd = particleComponent->GetColorEnd();

	particle.lifeTime = particleComponent->GetLifetime();
	particle.lifeRemaining = particleComponent->GetLifetime();
	particle.sizeBegin = particleComponent->GetSizeBegin() + particleComponent->GetSizeVariation() * (Random::Float() - 0.5f);
	particle.sizeEnd = particleComponent->GetSizeEnd();

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}

void ParticleManager::Clear()
{
	for (auto& particle : m_ParticlePool)
	{
		particle.active = false;
	}
}

ParticleManager* ParticleManager::GetInstance()
{
	if (instance == nullptr)
		instance = std::unique_ptr<ParticleManager>(new ParticleManager());
	return instance.get();
}
