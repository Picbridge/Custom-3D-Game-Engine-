#pragma once

class WindowHandler;
class Renderer;
class Input;
class UI;
class Time;
class PhysicsManager;
class EventHandler;
class CollisionManager;
class ScriptManager;
class SceneManager;
class ResourceFactory;
class ResourceManager;
class GameObjectFactory;
class GameObjectManager;
class AudioManager;
class ParticleManager;
class CameraManager;

class ServiceLocator
{
private:
	WindowHandler* m_pWindowHandler;
	Renderer* m_pRenderer;
	Input* m_pInput;
	UI* m_pUi;
	Time* m_pTime;
	PhysicsManager* m_pPhysicsManager;
	EventHandler* m_pEventHandler;
	CollisionManager* m_pCollisionManager;
	ScriptManager* m_pScriptManager;
	AudioManager* m_pAudioManager;
	ParticleManager* m_pParticleManager;
	CameraManager* m_pCameraManager;
	SystemSettings* m_pSystemSettings;

	mutable SceneManager* m_pSceneManager = nullptr;
	mutable ResourceFactory* m_pResourceFactory = nullptr;
	mutable ResourceManager* m_pResourceManager = nullptr;
	mutable GameObjectFactory* m_pGameObjectFactory = nullptr;
	mutable GameObjectManager* m_pGameObjectManager = nullptr;

public:
	ServiceLocator();

	//@brief Returns the window handler
	WindowHandler* GetWindowHandler() const;
	//@brief Returns the UI manager
	UI* GetUI() const;
	//@brief Returns RenderSystem
	Renderer* GetRenderer() const;
	//@brief Returns Input manager
	Input* GetInput() const;
	//@brief Returns the time
	Time* GetTime() const;
	//@brief Returns the PhysicsManager
	PhysicsManager* GetPhysicsManager() const;
	//@brief Returns the CollisionManager
	CollisionManager* GetCollisionManager() const;
	//@brief Returns the Event Handler
	EventHandler* GetEventHandler() const;
	//@brief Returns the ScriptManager
	ScriptManager* GetScriptManager() const;
	//@brief Returns the SceneManager
	SceneManager* GetSceneManager() const;
	//@brief Returns the AudioManager
	AudioManager* GetAudioManager() const;
	//@brief Return the ParticleManager
	ParticleManager* GetParticleManager() const;
	// @brief Returns the CameraManager
	CameraManager* GetCameraManager() const;
	// @brief Returns the ResourceFactory
	ResourceFactory* GetResourceFactory() const;
	// @brief Returns the ResourceManager
	ResourceManager* GetResourceManager() const;
	// @brief Returns the GameObjectFactory
	GameObjectFactory* GetGameObjectFactory() const;
	// @brief Returns the GameObjectManager
	GameObjectManager* GetGameObjectManager() const;
	// @brief Returns the SystemSettings
	SystemSettings* GetSystemSettings() const;
};

// Declare the static service locator instance
extern ServiceLocator SERVICE_LOCATOR;