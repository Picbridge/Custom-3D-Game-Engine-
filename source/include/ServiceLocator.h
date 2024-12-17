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

class ServiceLocator
{
private:
	WindowHandler* mp_WindowHandler;
	Renderer* mp_Renderer;
	Input* mp_Input;
	UI* mp_Ui;
	Time* mp_Time;
	PhysicsManager* mp_PhysicsManager;
	EventHandler* mp_EventHandler;
	CollisionManager* mp_CollisionManager;
	ScriptManager* mp_ScriptManager;
	AudioManager* mp_AudioManager;

	mutable SceneManager* mp_SceneManager = nullptr;
	mutable ResourceFactory* mp_ResourceFactory = nullptr;
	mutable ResourceManager* mp_ResourceManager = nullptr;
	mutable GameObjectFactory* mp_GameObjectFactory = nullptr;
	mutable GameObjectManager* mp_GameObjectManager = nullptr;

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

	// @brief Returns the ResourceFactory
	ResourceFactory* GetResourceFactory() const;
	// @brief Returns the ResourceManager
	ResourceManager* GetResourceManager() const;
	// @brief Returns the GameObjectFactory
	GameObjectFactory* GetGameObjectFactory() const;
	// @brief Returns the GameObjectManager
	GameObjectManager* GetGameObjectManager() const;
};

// Declare the static service locator instance
extern ServiceLocator SERVICE_LOCATOR;

//struct Services
//{
//public:

//	//@brief Returns the window handler
//	inline WindowHandler& GetWindow() const { return *AWindow; }
//	//@brief Returns the UI manager
//	inline UI& GetUI() { return *ui; }
//	//@brief Returns RenderSystem
//	inline Renderer* GetRenderSystem() const { return renderer; }
//	//@brief Returns Input manager
//	inline Input& GetInput() const { return *input; }
//	//@brief Returns the time
//	inline Time* GetTime() const { return time; }
//	//@brief Returns the PhysicsManager
//	inline PhysicsManager* GetPhysics() { return physics; }
//	//@brief Returns the CollisionManager
//	inline CollisionManager* GetCollisions() { return collisions; } 
//	//@brief Returns the Event Handler
//	inline EventHandler* GetEventHandler() { return eventHandler; }
//	//@brief Returns the ScriptManager
//	inline ScriptManager* GetScriptManager() { return scriptManager; }
//	// @brief Returns the ResourceFactory
//	ResourceFactory* GetResourceFactory() const;
//	// @brief Returns the ResourceManager
//	ResourceManager* GetResourceManager() const;
//	// @brief Returns the GameObjectFactory
//	GameObjectFactory* GetGameObjectFactory() const;
//	// @brief Returns the GameObjectManager
//	GameObjectManager* GetGameObjectManager() const;

//private:
//	WindowHandler* AWindow = &WindowHandler::GetInstance();
//	Renderer* renderer = Renderer::GetInstance();
//	Input* input = &Input::GetInstance();
//	UI* ui = &UI::GetInstance();
//	Time* time = Time::GetInstance();
//	PhysicsManager* physics = PhysicsManager::GetInstance();
//	EventHandler* eventHandler = EventHandler::GetInstance();
//	CollisionManager* collisions = CollisionManager::GetInstance(); 
//	ScriptManager* scriptManager = ScriptManager::GetInstance();

//	mutable ResourceFactory* resource_factory = nullptr;
//	mutable ResourceManager* resource_manager = nullptr;
//	mutable GameObjectFactory* go_factory = nullptr;
//	mutable GameObjectManager* go_manager = nullptr;
//};

//Services EngineServices;

////@brief Returns the requested service 
//template <typename T>
//inline static T* GetService()
//{
//	if (!instance) {
//		instance = new ServiceLocator();
//	}

//	if constexpr (std::is_same_v<T, Renderer>)
//	{
//		return instance->EngineServices.GetRenderSystem();
//	}
//	else if constexpr (std::is_same_v<T, WindowHandler>)
//	{
//		return &(instance->EngineServices).GetWindow();
//	}
//	else if constexpr (std::is_same_v<T, Input>)
//	{
//		return &(instance->EngineServices).GetInput();
//	}
//	else if constexpr (std::is_same_v<T, UI>)
//	{
//		return &(instance->EngineServices).GetUI();
//	}
//	else if constexpr (std::is_same_v<T, Time>)
//	{
//		return instance->EngineServices.GetTime();
//	}
//	else if constexpr (std::is_same_v<T, PhysicsManager>)
//	{
//		return (instance->EngineServices).GetPhysics();
//	}
//	else if constexpr (std::is_same_v<T, CollisionManager>)
//	{
//		return (instance->EngineServices).GetCollisions();
//	}
//	else if constexpr (std::is_same_v<T, EventHandler>)
//	{
//		return (instance->EngineServices).GetEventHandler();
//   }
//	else if constexpr (std::is_same_v<T, GameObjectFactory>)
//	{
//		return (instance->EngineServices).GetGameObjectFactory();
//	}
//	else if constexpr (std::is_same_v<T, GameObjectManager>)
//	{
//		return (instance->EngineServices).GetGameObjectManager();
//	}
//	else if constexpr (std::is_same_v<T, ScriptManager>)
//	{
//		return (instance->EngineServices).GetScriptManager();
//	}
//	else if constexpr (std::is_same_v<T, ResourceManager>)
//	{
//		return (instance->EngineServices).GetResourceManager();
//	}
//	else if constexpr (std::is_same_v<T, ResourceFactory>)
//	{
//		return (instance->EngineServices).GetResourceFactory();
//	}
//	else
//	{
//		//static_assert(sizeof(T) == -1, "Requested service type not available.");
//		return nullptr;
//	}
//}