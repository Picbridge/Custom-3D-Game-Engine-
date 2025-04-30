#include "../pch.h"
#include "CameraManager.h"
class Component;
#include "CameraComponent.h"
#include "../CameraControllerComponent.h"
#include "../objectmanager/GameObjectManager.h"
#include "../scenemanager/SceneManager.h"

std::unique_ptr<CameraManager> CameraManager::instance = nullptr;

CameraManager* CameraManager::GetInstance()
{
	if (!instance)
		instance = std::unique_ptr<CameraManager>(new CameraManager());

	return instance.get();
}


CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
}

void CameraManager::Init()
{
}

void CameraManager::Update()
{
	if (m_pEngineCamera && m_pEngineCamera->GetComponent<CameraComponent>()->IsActive())
	{
		m_pEngineCamera->Update();
	}
	//else
	//{
	//	for (auto& camera : m_activeCameras)
	//	{
	//		camera->GetOwner()->Update();
	//	}
	//}
}

void CameraManager::Shutdown()
{

}

void CameraManager::AddCamera(const std::string& name)
{
	GameObject* camObj = new GameObject();

	if (name != "")
		camObj->SetName(name);
	else
	{
		int i = 1;
		while (SERVICE_LOCATOR.GetGameObjectManager()->GetGameObject("Camera" + std::to_string(i)))
			i++;

		camObj->SetName("Camera" + std::to_string(i));
	}
	camObj->AddComponent<CameraComponent>();

	SERVICE_LOCATOR.GetGameObjectManager()->AddGameObject(camObj);
}

void CameraManager::RegisterCameraComponent(CameraComponent* camera)
{
	if (!camera || (m_pEngineCamera != nullptr && m_pEngineCamera->GetComponent<CameraComponent>() == camera))
	{
		std::cerr << "CameraComponent not found" << std::endl;
		return;
	}
	m_cameras.push_back(camera);

	// Set the main camera as a default if there are no active cameras
	if (m_activeCameras.empty())
		m_pMainCamera = camera;

	if (camera->IsActive())
		m_activeCameras.push_back(camera);
}

void CameraManager::RemoveCamera(CameraComponent* camera)
{
	if (!camera)
	{
		std::cerr << "CameraComponent not found" << std::endl;
		return;
	}
	SERVICE_LOCATOR.GetGameObjectManager()->DeleteGameObject(camera->GetOwnerAsGameObject());

	if (std::find(m_activeCameras.begin(), m_activeCameras.end(), camera) != m_activeCameras.end())
		m_activeCameras.erase(std::remove(m_activeCameras.begin(), m_activeCameras.end(), camera), m_activeCameras.end());

	if (std::find(m_cameras.begin(), m_cameras.end(), camera) != m_cameras.end())
		m_cameras.erase(std::remove(m_cameras.begin(), m_cameras.end(), camera), m_cameras.end());

}

void CameraManager::InflateActiveCameras(const std::vector<Node*>& nodes)
{
	for (auto node : nodes)
	{
		auto camera = node->GetComponent<CameraComponent>();
		if (camera)
		{
			if (!camera->IsActive())
			{
				camera->ToggleActive();
			}
			m_activeCameras.push_back(camera);
		}
	}
}

void CameraManager::ClearActiveCameras()
{
	m_activeCameras.clear();
	for (auto camera : m_cameras)
	{
		if (camera->IsActive())
		{
			camera->ToggleActive();
		}
	}
	m_pMainCamera = nullptr;
}

bool CameraManager::ToggleCamera(CameraComponent* camera)
{
	if (!camera)
	{
		std::cerr << "CameraComponent not found" << std::endl;
		return false;
	}

	auto isActive = camera->ToggleActive();

	if (!isActive)
		m_activeCameras.erase(std::remove(m_activeCameras.begin(), m_activeCameras.end(), camera), m_activeCameras.end());
	else
		m_activeCameras.push_back(camera);

	return !isActive;
}

CameraComponent* CameraManager::GetCameraByName(const std::string& name) const
{
	for (auto& cam : m_cameras)
	{
		if (cam->GetOwner()->GetName() == name)
			return cam;
	}
	return nullptr;
}

CameraComponent* CameraManager::AddEngineCamera()
{
	if (m_pEngineCamera == nullptr)
	{
		std::shared_ptr<GameObject> camObj = std::make_shared<GameObject>();
		m_pEngineCamera = camObj;

		camObj->SetName("EngineCamera");
		camObj->AddComponent<CameraComponent>()->Init();
		camObj->AddComponent<CameraControllerComponent>()->Init();
		camObj->GetComponent<CameraComponent>()->ToggleActive();
		camObj->GetComponent<CameraComponent>()->SetFOV(90.0f);
	}

	return m_pEngineCamera->GetComponent<CameraComponent>();
}

bool CameraManager::ToggleEngineCamera()
{
	if (m_pEngineCamera)
	{
		if (this->GetMainCamera() != this->GetEngineCamera())
			this->GetMainCamera()->ToggleActive();
		else
		{
			this->InflateActiveCameras(SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->GetNodes());
		}
		m_pEngineCamera->GetComponent<CameraComponent>()->ToggleActive();
		return m_pEngineCamera->GetComponent<CameraComponent>()->IsActive();
	}
	return false;
}

void CameraManager::SnapToEngineCamera(GameObject* camera)
{
	if (m_pEngineCamera && camera)
	{
		auto cameraComponent = camera->GetComponent<CameraComponent>();

		cameraComponent->SetPosition(m_pEngineCamera->GetTransform()->GetPosition());
		cameraComponent->SetRotation(m_pEngineCamera->GetTransform()->GetRotation());
		cameraComponent->SetFOV(m_pEngineCamera->GetComponent<CameraComponent>()->GetFOV());
		cameraComponent->SetNearPlane(m_pEngineCamera->GetComponent<CameraComponent>()->GetNearPlane());
		cameraComponent->SetFarPlane(m_pEngineCamera->GetComponent<CameraComponent>()->GetFarPlane());

	}
}

void CameraManager::SnapToEngineCamera(CameraComponent* camera)
{
	if (m_pEngineCamera && camera)
	{
		camera->SetPosition(m_pEngineCamera->GetTransform()->GetPosition());
		camera->SetRotation(m_pEngineCamera->GetTransform()->GetRotation());
		camera->SetFOV(m_pEngineCamera->GetComponent<CameraComponent>()->GetFOV());
		camera->SetNearPlane(m_pEngineCamera->GetComponent<CameraComponent>()->GetNearPlane());
		camera->SetFarPlane(m_pEngineCamera->GetComponent<CameraComponent>()->GetFarPlane());
	}
}

CameraComponent* CameraManager::GetEngineCamera() const
{
	return m_pEngineCamera == nullptr ? nullptr : m_pEngineCamera->GetComponent<CameraComponent>();
}

std::vector<CameraComponent*> CameraManager::GetCamerasForRendering() const
{
	if (m_pEngineCamera && m_pEngineCamera->GetComponent<CameraComponent>()->IsActive())
	{
		std::vector<CameraComponent*> cameras;
		cameras.push_back(m_pEngineCamera->GetComponent<CameraComponent>());
		return cameras;
	}
	return m_activeCameras;
}
