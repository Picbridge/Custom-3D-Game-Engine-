#include "pch.h"
#include "ServiceLocator.h"
#include "ui/UI.h"
#include "Input.h"
#include "physics/PhysicsManager.h"
#include "physics/CollisionManager.h"
#include "events/EventHandler.h"
#include "ScriptManager.h"
#include "scenemanager/SceneManager.h"
#include "AudioManager.h"

#include "resourcemanager/ResourceFactory.h"
#include "resourcemanager/ResourceManager.h"
#include "objectmanager/GameObjectFactory.h"
#include "objectmanager/GameObjectManager.h"

// Define the static instance
ServiceLocator SERVICE_LOCATOR;

SceneManager* ServiceLocator::GetSceneManager() const
{
	if (!mp_SceneManager)
	{
		mp_SceneManager = new SceneManager();
	}
	return mp_SceneManager;
}

ResourceFactory* ServiceLocator::GetResourceFactory() const
{
	if (!mp_ResourceFactory)
	{
		mp_ResourceFactory = new ResourceFactory();
	}
	return mp_ResourceFactory;
}

ResourceManager* ServiceLocator::GetResourceManager() const
{
	if (!mp_ResourceManager)
	{
		mp_ResourceManager = new ResourceManager();
	}
	return mp_ResourceManager;
}

GameObjectFactory* ServiceLocator::GetGameObjectFactory() const
{
	if (!mp_GameObjectFactory)
	{
		mp_GameObjectFactory = new GameObjectFactory();
	}
	return mp_GameObjectFactory;
}

GameObjectManager* ServiceLocator::GetGameObjectManager() const
{
	if (!mp_GameObjectManager)
	{
		mp_GameObjectManager = new GameObjectManager();
	}
	return mp_GameObjectManager;
}

ServiceLocator::ServiceLocator() :
	mp_WindowHandler(&WindowHandler::GetInstance()),
	mp_Renderer(Renderer::GetInstance()),
	mp_Input(&Input::GetInstance()),
	mp_Ui(&UI::GetInstance()),
	mp_Time(Time::GetInstance()),
	mp_PhysicsManager(PhysicsManager::GetInstance()),
	mp_EventHandler(EventHandler::GetInstance()),
	mp_CollisionManager(CollisionManager::GetInstance()),
	mp_ScriptManager(ScriptManager::GetInstance()),
	mp_AudioManager(AudioManager::GetInstance())
{}

WindowHandler* ServiceLocator::GetWindowHandler() const
{
	return mp_WindowHandler;
}

UI* ServiceLocator::GetUI() const
{
	return mp_Ui;
}

Renderer* ServiceLocator::GetRenderer() const
{
	return mp_Renderer;
}

Input* ServiceLocator::GetInput() const
{
	return mp_Input;
}

Time* ServiceLocator::GetTime() const
{
	return mp_Time;
}

PhysicsManager* ServiceLocator::GetPhysicsManager() const
{
	return mp_PhysicsManager;
}

CollisionManager* ServiceLocator::GetCollisionManager() const
{
	return mp_CollisionManager;
}

EventHandler* ServiceLocator::GetEventHandler() const
{
	return mp_EventHandler;
}

ScriptManager* ServiceLocator::GetScriptManager() const
{
	return mp_ScriptManager;
}

AudioManager* ServiceLocator::GetAudioManager() const
{
	return mp_AudioManager;
}
