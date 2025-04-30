#include "../pch.h"
#include "DefaultShader.h"
#include "../cameramanager/CameraComponent.h"
#include "../cameramanager/CameraManager.h"
#include "../ui/UI.h"
#include "../LightComponent.h"


void DefaultShader::PassShaderData()
{
    if (SERVICE_LOCATOR.GetSystemSettings()->GetBoolSetting("Normals"))
        setUniform("DebugNormal", 1);
    else
        setUniform("DebugNormal", 0);

	auto transform = m_pCurrObject->GetTransform();
	auto mainCamera = SERVICE_LOCATOR.GetCameraManager()->GetMainCamera();

	setUniform("model", m_pCurrObject->GetWorldTransform());
	setUniform("view", mainCamera->GetViewMatrix());
	setUniform("projection", mainCamera->GetProjectionMatrix());

	setUniform("viewPos", mainCamera->GetPosition());

	if (m_pCurrLight)
	{
		setUniform("light.position", transform->GetPosition() + m_pCurrLight->GetPosition());
		setUniform("light.ambient", m_pCurrLight->GetAmbient());
		setUniform("light.diffuse", m_pCurrLight->GetDiffuse());
		setUniform("light.specular", m_pCurrLight->GetSpecular());
		setUniform("lightSpaceMatrix", m_pCurrLight->GetLightSpaceMatrix());
	}

	bool hasDiffuse = m_pCurrMaterial->GetTextureDiffuse() != nullptr;
	bool hasSpecular = m_pCurrMaterial->GetTextureSpecular() != nullptr;

	setUniform("material.color", m_pCurrMaterial->GetColor());
	setUniform("material.shininess", m_pCurrMaterial->GetShininess());
	setUniform("material.alpha", m_pCurrMaterial->GetAlpha());

	setUniform("hasDiffuse", hasDiffuse);
	setUniform("hasSpecular", hasSpecular);

	if (hasDiffuse)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_pCurrMaterial->GetTextureDiffuseID());
		setUniform("material.diffuse", 1);
	}

	if (hasSpecular)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_pCurrMaterial->GetTextureSpecularID());
		setUniform("material.specular", 2);
	}

}
