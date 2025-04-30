#include "pch.h"
#include "ServiceLocator.h"
#include "ui/UI.h"
#include "Input.h"
#include "physics/PhysicsManager.h"
#include "physics/CollisionManager.h"
#include "events/EventHandler.h"
#include "scripting/ScriptManager.h"
#include "scenemanager/SceneManager.h"
#include "AudioManager.h"

#include "cameramanager/CameraManager.h"
#include "resourcemanager/ResourceFactory.h"
#include "resourcemanager/ResourceManager.h"
#include "objectmanager/GameObjectFactory.h"
#include "objectmanager/GameObjectManager.h"

// Define the static instance
ServiceLocator SERVICE_LOCATOR;

SceneManager* ServiceLocator::GetSceneManager() const
{
	if (!m_pSceneManager)
	{
		m_pSceneManager = new SceneManager();
	}
	return m_pSceneManager;
}

ResourceFactory* ServiceLocator::GetResourceFactory() const
{
	if (!m_pResourceFactory)
	{
		m_pResourceFactory = new ResourceFactory();
	}
	return m_pResourceFactory;
}

ResourceManager* ServiceLocator::GetResourceManager() const
{
	if (!m_pResourceManager)
	{
		m_pResourceManager = new ResourceManager();
	}
	return m_pResourceManager;
}

GameObjectFactory* ServiceLocator::GetGameObjectFactory() const
{
	if (!m_pGameObjectFactory)
	{
		m_pGameObjectFactory = new GameObjectFactory();
	}
	return m_pGameObjectFactory;
}

GameObjectManager* ServiceLocator::GetGameObjectManager() const
{
	if (!m_pGameObjectManager)
	{
		m_pGameObjectManager = new GameObjectManager();
	}
	return m_pGameObjectManager;
}

SystemSettings* ServiceLocator::GetSystemSettings() const
{
	return m_pSystemSettings;
}

ServiceLocator::ServiceLocator() :
	m_pWindowHandler(&WindowHandler::GetInstance()),
	m_pRenderer(Renderer::GetInstance()),
	m_pInput(&Input::GetInstance()),
	m_pUi(&UI::GetInstance()),
	m_pTime(Time::GetInstance()),
	m_pPhysicsManager(PhysicsManager::GetInstance()),
	m_pEventHandler(EventHandler::GetInstance()),
	m_pCollisionManager(CollisionManager::GetInstance()),
	m_pScriptManager(ScriptManager::GetInstance()),
	m_pAudioManager(AudioManager::GetInstance()),
	m_pParticleManager(ParticleManager::GetInstance()),
	m_pCameraManager(CameraManager::GetInstance()),
	m_pSystemSettings(SystemSettings::GetInstance())
{
}

WindowHandler* ServiceLocator::GetWindowHandler() const
{
	return m_pWindowHandler;
}

UI* ServiceLocator::GetUI() const
{
	return m_pUi;
}

Renderer* ServiceLocator::GetRenderer() const
{
	return m_pRenderer;
}

Input* ServiceLocator::GetInput() const
{
	return m_pInput;
}

Time* ServiceLocator::GetTime() const
{
	return m_pTime;
}

PhysicsManager* ServiceLocator::GetPhysicsManager() const
{
	return m_pPhysicsManager;
}

CollisionManager* ServiceLocator::GetCollisionManager() const
{
	return m_pCollisionManager;
}

EventHandler* ServiceLocator::GetEventHandler() const
{
	return m_pEventHandler;
}

ScriptManager* ServiceLocator::GetScriptManager() const
{
	return m_pScriptManager;
}

AudioManager* ServiceLocator::GetAudioManager() const
{
	return m_pAudioManager;
}

ParticleManager* ServiceLocator::GetParticleManager() const
{
	return m_pParticleManager;
}

CameraManager* ServiceLocator::GetCameraManager() const
{
	return m_pCameraManager;
}