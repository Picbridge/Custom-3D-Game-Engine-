#include "pch.h"
#include "LightComponent.h"
#include "resourcemanager/ResourceManager.h"
#include "scenemanager/SceneManager.h"
#include "scenemanager/Scene.h"
#include "ui/UI.h"
#include "TransformComponent.h"

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}



LightComponent::LightComponent()
{
	defineMember();
}

LightComponent::~LightComponent()
{
	SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->SetLight(nullptr);
}

void LightComponent::Init()
{
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	depthMapFBO = 0;
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	if (glGetError() != GL_NO_ERROR) {
		printf("Error generating or binding framebuffer\n");
		return;
	}

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	//GLuint depthTexture;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	if (glGetError() != GL_NO_ERROR) {
		printf("Error generating or binding depth texture\n");
		return;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	if (glGetError() != GL_NO_ERROR) {
		printf("Error creating depth texture image\n");
		return;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (glGetError() != GL_NO_ERROR) {
		printf("Error setting texture parameters\n");
		return;
	}

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
	glDrawBuffer(GL_NONE); // No color buffer is drawn to.
	if (glGetError() != GL_NO_ERROR) {
		printf("Error attaching depth texture to framebuffer\n");
		return;
	}

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("FRAMEBUFFER IS NOT OKAY\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightComponent::Shutdown()
{
}

void LightComponent::Update()
{
	//position = pOwner->GetComponent<TransformComponent>()->GetPosition();
	// Light Matrix
	near_plane = 50.0f; far_plane = 500.0f;
	glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane);
	//glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(pOwner->GetComponent<TransformComponent>()->GetPosition() + position,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::normalize(glm::vec3(0.0f, 1.0f, 0.01f)));
	lightSpaceMatrix = lightProjection * lightView * glm::mat4(1.0f);

	SetUpShadowPass();
	GenerateShadowMap();
	CleanUpShadowPass();
}

void LightComponent::SetUpShadowPass() const
{
	// Shadow Map Pass
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	Shader* shadow = SERVICE_LOCATOR.GetResourceManager()->GetShader("Shadow");
	shadow->Use();
	//lightSpaceMatrix = Camera::GetInstance()->m_worldProjection * Camera::GetInstance()->m_worldView * glm::mat4(1.0f);
	shadow->PassShaderData("lightSpaceMatrix", lightSpaceMatrix);
	glCullFace(GL_FRONT);
}

void LightComponent::GenerateShadowMap()
{
	// Render all game objects from light's perspective
	Scene* scene = SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene();
	Shader* shadow = SERVICE_LOCATOR.GetResourceManager()->GetShader("Shadow");

	// set polygon mode to fill
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (auto& node : scene->GetNodes())
	{
		GameObject* gameObject = dynamic_cast<GameObject*>(node);
		if (gameObject)
		{
			gameObject->Render(shadow);
		}
	}
}

void LightComponent::CleanUpShadowPass()
{
	Shader* shadow = SERVICE_LOCATOR.GetResourceManager()->GetShader("Shadow");
	glCullFace(GL_BACK);
	shadow->Unuse();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Reset viewport for scene or debugging
	auto frameBuffer = SERVICE_LOCATOR.GetWindowHandler()->FrameBuffer;
	glViewport(0, 0, frameBuffer.Width, frameBuffer.Height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void LightComponent::DebugDrawShadows()
{
	Shader* shadowDebug = SERVICE_LOCATOR.GetResourceManager()->GetShader("ShadowDebug");
	shadowDebug->Use();
	shadowDebug->PassShaderData("near_plane", near_plane, "far_plane", far_plane);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	renderQuad();
	shadowDebug->Unuse();
}


