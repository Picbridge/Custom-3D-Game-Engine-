#include "../pch.h"
#include "CelShader.h"
#include "../cameramanager/CameraComponent.h"
#include "../cameramanager/CameraManager.h"
#include "../LightComponent.h"
#include "../Input.h"

void CelShader::PassShaderData()
{
	auto transform = m_pCurrObject->GetTransform();
	auto mainCamera = SERVICE_LOCATOR.GetCameraManager()->GetMainCamera();

	setUniform("model", m_pCurrObject->GetWorldTransform());
	setUniform("view", mainCamera->GetViewMatrix());
	setUniform("projection", mainCamera->GetProjectionMatrix());
	setUniform("viewPos", mainCamera->GetPosition());

	if (m_pCurrLight)
	{
		setUniform("lightPos", m_pCurrLight->GetPosition());
		setUniform("lightColor", m_pCurrLight->GetAmbient());
	}

	
	//if (SERVICE_LOCATOR.GetInput()->IsKeyPressed(GLFW_KEY_1))
	//{
	//	if (SERVICE_LOCATOR.GetInput()->IsKeyJustPressed(GLFW_KEY_MINUS))
	//	{
	//		depthThreshold -= 0.001f;
	//	}
	//	else if (SERVICE_LOCATOR.GetInput()->IsKeyJustPressed(GLFW_KEY_EQUAL))
	//	{
	//		depthThreshold += 0.001f;
	//	}
	//}
	//if (SERVICE_LOCATOR.GetInput()->IsKeyPressed(GLFW_KEY_2))
	//{
	//	if (SERVICE_LOCATOR.GetInput()->IsKeyJustPressed(GLFW_KEY_MINUS))
	//	{
	//		m_ambient -= 0.01f;
	//	}
	//	else if (SERVICE_LOCATOR.GetInput()->IsKeyJustPressed(GLFW_KEY_EQUAL))
	//	{
	//		m_ambient += 0.01f;
	//	}
	//}
	//if (SERVICE_LOCATOR.GetInput()->IsKeyPressed(GLFW_KEY_3))
	//{
	//	if (SERVICE_LOCATOR.GetInput()->IsKeyJustPressed(GLFW_KEY_MINUS))
	//	{
	//		normalEdgeBias -= 0.1f;
	//	}
	//	else if (SERVICE_LOCATOR.GetInput()->IsKeyJustPressed(GLFW_KEY_EQUAL))
	//	{
	//		normalEdgeBias += 0.1f;
	//	}
	//}
	//if (SERVICE_LOCATOR.GetInput()->IsKeyPressed(GLFW_KEY_4))
	//{
	//	if (SERVICE_LOCATOR.GetInput()->IsKeyJustPressed(GLFW_KEY_MINUS))
	//	{
	//		outlineScale -= 0.1f;
	//	}
	//	else if (SERVICE_LOCATOR.GetInput()->IsKeyJustPressed(GLFW_KEY_EQUAL))
	//	{
	//		outlineScale += 0.1f;
	//	}
	//}

	setUniform("ambient", m_ambient);
	setUniform("Shades", shades);
	setUniform("Smoothness", smoothness);
	setUniform("EdgeDiffuse", edgeDiffuse);
	setUniform("EdgeSpecular", edgeSpecular);
	setUniform("EdgeSpecularOffset", edgeSpecularOffset);
	setUniform("EdgeDistanceAttenuation", edgeDistanceAttenuation);
	setUniform("EdgeShadowAttenuation", edgeShadowAttenuation);
	setUniform("EdgeRimOffset", edgeRimOffset);
	setUniform("RimThreshold", rimThreshold);
	//setUniform("depthThreshold", depthThreshold);
	//setUniform("normalThreshold", normalThreshold);
	//setUniform("normalEdgeBias", normalEdgeBias);
	//setUniform("outlineScale", outlineScale);

}