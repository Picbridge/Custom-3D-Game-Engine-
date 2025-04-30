#include "../pch.h"
#include "ScriptComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "../physics/PhysicsComponent.h"
#include "../physics/CollisionComponent.h"
#include "../Input.h"
#include "../cameramanager/CameraComponent.h"
#include "../CameraControllerComponent.h"
#include "../ParticleComponent.h"
#include "../ui/UI.h"
#include "../VectorCalculations.h"
#include "../GrappleComponent.h"
#include "../Debug.h"
#include "../AudioManager.h"
#include "../physics/Collision.h"
#include "../scenemanager/SceneManager.h"
#include "../SampleGame/SampleGame.h"
#include "../objectmanager/GameObjectManager.h"

static EventListener eventListener;

void CreateComponentBindings(sol::state& m_lua);

TransformComponent* ScriptComponent::GetTransformComponent(Node* parent)
{
	return parent->GetComponent<TransformComponent>();
}

RenderComponent* ScriptComponent::GetRenderComponent(Node* parent)
{
	return parent->GetComponent<RenderComponent>();
}

PhysicsComponent* ScriptComponent::GetPhysicsComponent(Node* parent)
{
	return parent->GetComponent<PhysicsComponent>();
}

ParticleComponent* ScriptComponent::GetParticleComponent(Node* parent)
{
	return parent->GetComponent<ParticleComponent>();
}

CameraControllerComponent* ScriptComponent::GetControllerComponent(Node* parent)
{
	return parent->GetComponent<CameraControllerComponent>();
}

CollisionComponent* ScriptComponent::GetCollisionComponent(Node* parent)
{
	return parent->GetComponent<CollisionComponent>();
}

ScriptComponent* ScriptComponent::GetScriptComponent(Node* parent)
{
	return parent->GetComponent<ScriptComponent>();
}

CameraComponent* ScriptComponent::GetCameraComponent(Node* parent)
{
	return parent->GetComponent<CameraComponent>();
}

GrappleComponent* ScriptComponent::GetGrappleComponent(Node* parent)
{
	return parent->GetComponent<GrappleComponent>();
}

void ScriptComponent::CollisionEventCallback(Event* event)
{
	CollisionEvent* collisionEvent = static_cast<CollisionEvent*>(event);
	GameObject* staticObject = collisionEvent->m_collision->GetStaticObject();
	if (staticObject->GetName() == GetOwner()->GetName() && m_lua["OnCollision"].valid())
	{
		m_lua["OnCollision"]();
	}
}

void ScriptComponent::BindCollisionBoxEvent()
{
	std::cout << "Bound CB Event!\n";
	m_pEventListener = new EventListener();
	m_pEventListener->SubscribeToEvent("Triggered", std::bind(&ScriptComponent::CollisionEventCallback, this, std::placeholders::_1));
}

UIComponent* ScriptComponent::FindUIComponent(std::string name)
{
	return SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->FindUIComponent(name);
}

std::string ScriptComponent::GetCurrentSceneName()
{
	return SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->GetName();
}

void ScriptComponent::SetUITextComponentActiveState(std::string name, bool state)
{
	UITextComponent* uitc = static_cast<UITextComponent*>(SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->FindUIComponent(name));
	if (uitc != nullptr)
		uitc->SetActive(state);
	else
		std::cout << "UITextComponent " << name << " not found in ScriptComponent::SetUITextComponentActiveState()!" << std::endl;
}

void ScriptComponent::CreateAllBindings() {
	CreateGLMBindings();
	CreateComponentBindings(m_lua);

	m_lua.new_usertype<Scene>("Scene",
		sol::constructors<Scene()>(),
		"FindUIComponent", sol::overload([](Scene& self, std::string name)
			{
				return self.FindUIComponent(name);
			},
			static_cast<UIComponent * (Scene::*)(const std::string&, Node*)>(&Scene::FindUIComponent))
	);

	// Node
	m_lua.new_usertype<Node>("Node",
		sol::constructors<Node()>(),
		"AddChild", &Node::AddChild,
		"RemoveChild", &Node::RemoveChild,
		"SetID", &Node::SetID,
		"Destroy", &Node::Destroy,
		"Flush", &Node::Flush,
		"GetID", &Node::GetID,
		"GetTransform", &Node::GetTransform,
		"GetWorldTransform", &Node::GetWorldTransform,
		"GetParent", &Node::GetParent,
		"GetChildren", &Node::GetChildren,
		"NeedsDeletion", &Node::NeedsDeletion,
		"GetName", &Node::GetName
	);

	// GameObject
	m_lua.new_usertype<GameObject>("GameObject",
		sol::constructors<GameObject()>(),
		sol::base_classes, sol::bases<Node>(), // Establish inheritance 
		"SetName", &GameObject::SetName,
		"Init", &GameObject::Init,
		"Update", &GameObject::Update,
		"Render", static_cast<void (GameObject::*)()>(&GameObject::Render),
		"SetDead", &GameObject::SetDead,
		"GetName", &GameObject::GetName,
		"IsDead", &GameObject::IsDead,
		"SetActive", &GameObject::SetActive,
		"IsActive", &GameObject::IsActive
	);

	// GameObjectManager
	m_lua.new_usertype<GameObjectManager>("GameObjectManager",
		sol::no_constructor, // Prevent Lua from creating instances
		// Core functionality
		"Init", &GameObjectManager::Init,
		"Update", &GameObjectManager::Update,
		"Render", &GameObjectManager::Render,
		"Shutdown", &GameObjectManager::Shutdown,
		// Object management with optional parent parameter
		"AddGameObject", [](GameObjectManager& self, GameObject* obj, sol::optional<GameObject*> parent) {
			self.AddGameObject(obj, parent.value_or(nullptr));
		},
		"DeleteGameObject", &GameObjectManager::DeleteGameObject,
		"CloneNode", &GameObjectManager::CloneNode,
		// Object access
		"GetGameObject", &GameObjectManager::GetGameObject,
		"GetGameObjects", &GameObjectManager::GetGameObjects,
		// Rendering utilities
		"GetShadowMatrix", &GameObjectManager::GetShadowMatrix
	);

	// ServiceLocator (minimal exposure)
	m_lua.new_usertype<ServiceLocator>("ServiceLocator",
		sol::no_constructor, // Prevent Lua from creating instances
		"GetGameObjectManager", &ServiceLocator::GetGameObjectManager
	);

	// Expose global SERVICE_LOCATOR instance
	m_lua["SERVICE_LOCATOR"] = &SERVICE_LOCATOR;

	// Input
	// Expose GLFWgamepadstate to Lua (should never *need* this but it's here for posterity)
	m_lua.new_usertype<GLFWgamepadstate>("GLFWgamepadstate",
		"buttons", sol::property([](GLFWgamepadstate& self) {
			return std::vector<unsigned char>(self.buttons, self.buttons + 15); // 15 buttons
			}),
		"axes", sol::property([](GLFWgamepadstate& self) {
			return std::vector<float>(self.axes, self.axes + 6); // 6 axes
			})
	);

	// Input class
	m_lua.new_usertype<Input>("Input",
		sol::no_constructor,
		"GetInstance", &Input::GetInstance,
		"Init", &Input::Init,
		"Update", &Input::Update,
		"PostUpdate", &Input::PostUpdate,
		"IsKeyPressed", &Input::IsKeyPressed,
		"IsKeyJustPressed", &Input::IsKeyJustPressed,
		"IsMouseButtonPressed", &Input::IsMouseButtonPressed,
		"IsMouseButtonJustPressed", &Input::IsMouseButtonJustPressed,
		"GetMouseX", &Input::GetMouseX,
		"GetMouseY", &Input::GetMouseY,
		"GetMouseCoords", &Input::GetMouseCoords,
		"GetMouseNormalizedCoords", &Input::GetMouseNormalizedCoords,
		"GetScrollZoom", &Input::GetScrollZoom,
		"GetScrollZoomDelta", &Input::GetScrollZoomDelta,
		"SetScrollZoom", &Input::SetScrollZoom,
		"GetGamepadCount", &Input::GetGamepadCount,
		"GetGamepadState", &Input::GetGamepadState,
		"GetMouseButtonState", &Input::GetMouseButtonState,
		"IsGamepadButtonJustPressed", &Input::IsGamepadButtonJustPressed,
		"IsGamepadButtonPressed", &Input::IsGamepadButtonPressed,
		"IsGamepadPresent", &Input::IsGamepadPresent
	);

	// Constants
	m_lua["INPUT_MAX_GAMEPADS"] = INPUT_MAX_GAMEPADS;
	m_lua["INPUT_MAX_BUTTONS"] = INPUT_MAX_BUTTONS;
	m_lua["INPUT_MAX_AXES"] = INPUT_MAX_AXES;

	m_lua.new_usertype<CollisionManager>(
		"CollisionManager",
		sol::no_constructor,
		"GetInstance", &CollisionManager::GetInstance,
		"RemoveCollisionComponent", &CollisionManager::RemoveCollisionComponent
	);

	m_lua.new_usertype<AudioManager>(
		"AudioManager",
		sol::no_constructor, // Prevent Lua from creating new instances
		// Static function to get the singleton instance
		"GetInstance", &AudioManager::GetInstance,
		// Member functions with overloads for default parameters
		"Init", &AudioManager::Init,
		"CreateSound", sol::overload(
			[](AudioManager& self, const std::string& filePath, int mode) {
				self.CreateSound(filePath, static_cast<FMOD_MODE>(mode), 1.0f);
			},
			[](AudioManager& self, const std::string& filePath, int mode, float volume) {
				self.CreateSound(filePath, static_cast<FMOD_MODE>(mode), volume);
			}
		),
		"PlaySound", sol::overload(
			[](AudioManager& self, const std::string& filePath) {
				self.PlaySound(filePath, nullptr);
			},
			[](AudioManager& self, const std::string& filePath, glm::vec3 pos) {
				self.PlaySound(filePath, &pos);
			}
		),
		// Other member functions
		"SetMasterVolume", &AudioManager::SetMasterVolume,
		"StopSound", &AudioManager::StopSound,
		"ToggleMute", &AudioManager::ToggleMute,
		"SetMute", &AudioManager::SetMute,
		"Update", &AudioManager::Update,
		"Shutdown", &AudioManager::Shutdown,
		// Public member variables
		"m_playMusic", &AudioManager::m_playMusic,
		"m_playSFX", &AudioManager::m_playSFX,
		"m_muted", &AudioManager::m_muted
	);

	// System Settings
	m_lua.new_usertype<SystemSettings>(
		"SystemSettings",
		sol::no_constructor, // Prevent Lua from creating new instances
		// Static function to get the singleton instance
		"GetInstance", &SystemSettings::GetInstance,
		"GetFloatSetting", &SystemSettings::GetFloatSetting,
		"SetFloatSetting", &SystemSettings::SetFloatSetting,
		"GetBoolSetting", &SystemSettings::GetBoolSetting,
		"SetBoolSetting", &SystemSettings::SetBoolSetting
	);

	// UI
	m_lua.new_enum<IMGUI_ELEMENT_TYPE>("IMGUI_ELEMENT_TYPE", {
		{"BUTTON_TOGGLE", IMGUI_ELEMENT_TYPE::BUTTON_TOGGLE},
		{"BUTTON_ACTION", IMGUI_ELEMENT_TYPE::BUTTON_ACTION},
		{"SLIDER", IMGUI_ELEMENT_TYPE::SLIDER},
		{"TEXT", IMGUI_ELEMENT_TYPE::TEXT},
		{"DROPDOWN_TOGGLE", IMGUI_ELEMENT_TYPE::DROPDOWN_TOGGLE},
		{"DROPDOWN_SELECTION", IMGUI_ELEMENT_TYPE::DROPDOWN_SELECTION},
		{"DROPDOWN_ACTION", IMGUI_ELEMENT_TYPE::DROPDOWN_ACTION},
		{"DROPDOWN_SLIDER", IMGUI_ELEMENT_TYPE::DROPDOWN_SLIDER},
		{"END", IMGUI_ELEMENT_TYPE::END}
		});

	m_lua.new_usertype<UI>("UI",
		sol::no_constructor,
		"GetInstance", &UI::GetInstance,
		"Hovering", sol::property(&UI::Hovering),
		"GetGameIndex", &UI::GetGameIndex,
		"GetIsPaused", &UI::GetIsPaused,
		"GetToggleState", sol::overload(
			[](UI& self, const char* name)
			{
				return self.GetToggleState(name);
			},
			static_cast<int (UI::*)(const char*, const char*) const>(&UI::GetToggleState)
		),
		"GetSliderValue", sol::overload(
			[](UI& self, const char* name)
			{
				return self.GetSliderValue(name);
			},
			static_cast<float (UI::*)(const char*, const char*) const>(&UI::GetSliderValue)
		),
		"GetSelectedSceneName", &UI::GetSelectedSceneName,
		"SetDebug", &UI::SetDebug,
		"ToggleDebug", &UI::ToggleDebug
	);

	m_lua.new_usertype<VectorCalculation>(
		"VectorCalculation", sol::no_constructor,
		"GetForwardVecFromView", sol::overload(
			static_cast<glm::vec3(*)(const glm::mat4&)>(&VectorCalculation::GetForwardVecFromView)
		),
		"GetRightVecFromView", sol::overload(
			static_cast<glm::vec3(*)(const glm::mat4&)>(&VectorCalculation::GetRightVecFromView)
		),
		"GetUpVecFromView", sol::overload(
			static_cast<glm::vec3(*)(const glm::mat4&)>(&VectorCalculation::GetUpVecFromView)
		)
	);

	m_lua.new_usertype<sample>(
		"Game", sol::no_constructor,
		sol::base_classes, sol::bases<Game>(),
		"FinishLevel", &sample::FinishLevel,
		"SetLevelTime", &sample::SetLevelTime,
		"AddPoint", &sample::AddPoint
	);

	// --- GLFW Constants ---
	// Keyboard keys
	m_lua["GLFW_KEY_SPACE"] = GLFW_KEY_SPACE;
	m_lua["GLFW_KEY_W"] = GLFW_KEY_W;
	m_lua["GLFW_KEY_A"] = GLFW_KEY_A;
	m_lua["GLFW_KEY_S"] = GLFW_KEY_S;
	m_lua["GLFW_KEY_D"] = GLFW_KEY_D;
	m_lua["GLFW_KEY_Q"] = GLFW_KEY_Q;
	m_lua["GLFW_KEY_E"] = GLFW_KEY_E;
	m_lua["GLFW_KEY_ESCAPE"] = GLFW_KEY_ESCAPE;
	m_lua["GLFW_KEY_ENTER"] = GLFW_KEY_ENTER;
	m_lua["GLFW_KEY_LEFT_CONTROL"] = GLFW_KEY_LEFT_CONTROL;
	m_lua["GLFW_KEY_LEFT_SHIFT"] = GLFW_KEY_LEFT_SHIFT;
	m_lua["GLFW_KEY_BACKSPACE"] = GLFW_KEY_BACKSPACE;
	// Add more keys as needed...

	// Mouse buttons
	m_lua["GLFW_MOUSE_BUTTON_LEFT"] = GLFW_MOUSE_BUTTON_LEFT;
	m_lua["GLFW_MOUSE_BUTTON_RIGHT"] = GLFW_MOUSE_BUTTON_RIGHT;
	m_lua["GLFW_MOUSE_BUTTON_MIDDLE"] = GLFW_MOUSE_BUTTON_MIDDLE;

	// Gamepad buttons
	m_lua["GLFW_GAMEPAD_BUTTON_A"] = GLFW_GAMEPAD_BUTTON_A;
	m_lua["GLFW_GAMEPAD_BUTTON_B"] = GLFW_GAMEPAD_BUTTON_B;
	m_lua["GLFW_GAMEPAD_BUTTON_X"] = GLFW_GAMEPAD_BUTTON_X;
	m_lua["GLFW_GAMEPAD_BUTTON_Y"] = GLFW_GAMEPAD_BUTTON_Y;
	m_lua["GLFW_GAMEPAD_BUTTON_LEFT_BUMPER"] = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER;
	m_lua["GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER"] = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER;
	m_lua["GLFW_GAMEPAD_BUTTON_BACK"] = GLFW_GAMEPAD_BUTTON_BACK;
	m_lua["GLFW_GAMEPAD_BUTTON_START"] = GLFW_GAMEPAD_BUTTON_START;
	m_lua["GLFW_GAMEPAD_BUTTON_DPAD_UP"] = GLFW_GAMEPAD_BUTTON_DPAD_UP;
	m_lua["GLFW_GAMEPAD_BUTTON_DPAD_RIGHT"] = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT;
	m_lua["GLFW_GAMEPAD_BUTTON_DPAD_DOWN"] = GLFW_GAMEPAD_BUTTON_DPAD_DOWN;
	m_lua["GLFW_GAMEPAD_BUTTON_DPAD_LEFT"] = GLFW_GAMEPAD_BUTTON_DPAD_LEFT;

	// Gamepad axes
	m_lua["GLFW_GAMEPAD_AXIS_LEFT_X"] = GLFW_GAMEPAD_AXIS_LEFT_X;
	m_lua["GLFW_GAMEPAD_AXIS_LEFT_Y"] = GLFW_GAMEPAD_AXIS_LEFT_Y;
	m_lua["GLFW_GAMEPAD_AXIS_RIGHT_X"] = GLFW_GAMEPAD_AXIS_RIGHT_X;
	m_lua["GLFW_GAMEPAD_AXIS_RIGHT_Y"] = GLFW_GAMEPAD_AXIS_RIGHT_Y;
	m_lua["GLFW_GAMEPAD_AXIS_LEFT_TRIGGER"] = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER;
	m_lua["GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER"] = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER;

	// Joystick events
	m_lua["GLFW_CONNECTED"] = GLFW_CONNECTED;
	m_lua["GLFW_DISCONNECTED"] = GLFW_DISCONNECTED;

	// Expose this ScriptComponent!
	m_lua["this_script"] = this;
}

void CreateComponentBindings(sol::state& m_lua) 
{
	// Component (base)
	m_lua.new_usertype<Component>("Component",
		"Init", &Component::Init,
		"Update", &Component::Update,
		"Shutdown", &Component::Shutdown,
		"SetOwner", &Component::SetOwner,
		"GetOwner", &Component::GetOwner,
		"GetOwnerAsGameObject", &Component::GetOwnerAsGameObject
	);

	m_lua.new_usertype<UIComponent>("UIComponent",
		sol::constructors<UIComponent()>(),
		sol::base_classes, sol::bases<Component>(),
		"SetStringBuffer", &UIComponent::SetSaveFilePath
	);

	// TransformComponent
	m_lua.new_usertype<TransformComponent>("TransformComponent",
		sol::constructors<TransformComponent()>(),
		sol::base_classes, sol::bases<Component>(),
		"Init", &TransformComponent::Init,
		"Update", &TransformComponent::Update,
		"Shutdown", &TransformComponent::Shutdown,
		"SetPosition", &TransformComponent::SetPosition,
		"SetRotation", &TransformComponent::SetRotation,
		"SetRotationWithForward", &TransformComponent::SetPositionWithForwardVec,
		"SetScale", static_cast<void (TransformComponent::*)(float)>(&TransformComponent::SetScale), // Expose only the overloads that take standard data types.
		"GetPosition", &TransformComponent::GetPosition,
		"GetRotation", &TransformComponent::GetRotation,
		"GetScale", &TransformComponent::GetScale,
		"GetTranslationMatrix", &TransformComponent::GetTranslationMatrix,
		"GetRotationMatrix", &TransformComponent::GetRotationMatrix,
		"GetScaleMatrix", &TransformComponent::GetScaleMatrix,
		"GetModelMatrix", &TransformComponent::GetModelMatrix
	);

	// RenderComponent
	m_lua.new_usertype<RenderComponent>("RenderComponent",
		sol::constructors<RenderComponent()>(),
		sol::base_classes, sol::bases<Component>(),
		"Init", &RenderComponent::Init,
		"Update", &RenderComponent::Update,
		"Shutdown", &RenderComponent::Shutdown,
		"Render", static_cast<void (RenderComponent::*)()>(&RenderComponent::Render),
		"SetColor", static_cast<void (RenderComponent::*)(float, float, float)>(&RenderComponent::SetColor), // Expose only the overloads that take standard data types.
		"SetMaterial", static_cast<void (RenderComponent::*)(const std::string)>(&RenderComponent::SetMaterial),
		"SetShader", static_cast<void (RenderComponent::*)(const std::string)>(&RenderComponent::SetShader),
		"SetGeometry", static_cast<void (RenderComponent::*)(const std::string&)>(&RenderComponent::SetGeometry),
		"SetUVType", &RenderComponent::SetUVType,
		"GetGeometry", &RenderComponent::GetGeometry,
		"GetShader", &RenderComponent::GetShader
	);


	// PhysicsComponent
	m_lua.new_usertype<PhysicsComponent>("PhysicsComponent",
		sol::constructors<PhysicsComponent(), PhysicsComponent(double, double, double)>(),
		sol::base_classes, sol::bases<Component>(),
		"Init", &PhysicsComponent::Init,
		"Update", sol::overload(
			static_cast<void (PhysicsComponent::*)(double)>(&PhysicsComponent::Update),
			static_cast<void (PhysicsComponent::*)()>(&PhysicsComponent::Update)
		),
		"Shutdown", &PhysicsComponent::Shutdown,
		"ApplyForce", sol::overload(
			static_cast<void (PhysicsComponent::*)(const glm::dvec3)>(&PhysicsComponent::ApplyForce),
			static_cast<void (PhysicsComponent::*)(double, double, double)>(&PhysicsComponent::ApplyForce)
		),
		"ApplyTorque", sol::overload(
			static_cast<void (PhysicsComponent::*)(const glm::dvec3&)>(&PhysicsComponent::ApplyTorque),
			static_cast<void (PhysicsComponent::*)(double, double, double)>(&PhysicsComponent::ApplyTorque)
		),
		"SetGrounded", &PhysicsComponent::SetGrounded,
		"SetVelocity", sol::overload(
			static_cast<void (PhysicsComponent::*)(const glm::dvec3)>(&PhysicsComponent::SetVelocity),
			static_cast<void (PhysicsComponent::*)(double, double, double)>(&PhysicsComponent::SetVelocity)
		),
		"SetMass", &PhysicsComponent::SetMass,
        "SetFrictionCoefficient", &PhysicsComponent::SetFrictionCoefficient,
		"SetDrag", &PhysicsComponent::SetDrag,
		"SetGravityMultiplyer", &PhysicsComponent::SetGravityMultiplyer,
		"GetGrounded", &PhysicsComponent::Grounded,
        "GetGroundedNormal", &PhysicsComponent::GetGroundedNormal,
		"GetVelocity", &PhysicsComponent::GetVelocity,
		"GetMass", &PhysicsComponent::GetMass,
		"GetInverseMass", &PhysicsComponent::GetInverseMass,
        "GetFrictionCoefficient", &PhysicsComponent::GetFrictionCoefficient,
		"GetDrag", &PhysicsComponent::GetDrag,
		"GetGravityMultiplyer", &PhysicsComponent::GetGravityMultiplyer,
		"IsStillGrounded", &PhysicsComponent::StillGrounded
	);

	// Bind Viewport struct first
	m_lua.new_usertype<Viewport>("Viewport",
		sol::constructors<Viewport(int, int, int, int)>(),
		"X", &Viewport::X,
		"Y", &Viewport::Y,
		"W", &Viewport::W,
		"H", &Viewport::H
	);

	m_lua.new_usertype<CameraComponent>("CameraComponent",
		sol::constructors<CameraComponent()>(),
		sol::base_classes, sol::bases<Component>(),
		"Init", &CameraComponent::Init,
		"Update", &CameraComponent::Update,
		"Shutdown", &CameraComponent::Shutdown,
		"SetViewport", sol::overload(
			[](CameraComponent& self, int x, int y, int w, int h) { self.SetViewport(x, y, w, h); },
			[](CameraComponent& self, const Viewport& vp) { self.SetViewport(vp); }
		),
		"GetViewport", &CameraComponent::GetViewport,
		"ToggleDynamicClipping", &CameraComponent::ToggleDynamicClipping,
		"ToggleOcclusionCulling", &CameraComponent::ToggleOcclusionCulling,
		"ToggleActive", &CameraComponent::ToggleActive,
		"SetPosition", &CameraComponent::SetPosition,
		"SetRotation", &CameraComponent::SetRotation,
		"SetFOV", &CameraComponent::SetFOV,
		"SetNearPlane", &CameraComponent::SetNearPlane,
		"SetFarPlane", &CameraComponent::SetFarPlane,
		"IsDynamicClipping", &CameraComponent::IsDynamicClipping,
		"IsOcclusionCulling", &CameraComponent::IsOcclusionCulling,
		"IsActive", &CameraComponent::IsActive,
		"GetFOV", &CameraComponent::GetFOV,
		"GetNearPlane", &CameraComponent::GetNearPlane,
		"GetFarPlane", &CameraComponent::GetFarPlane,
		"GetPosition", &CameraComponent::GetPosition,
		"GetRotation", &CameraComponent::GetRotation,
		"GetViewMatrix", &CameraComponent::GetViewMatrix,
		"GetProjectionMatrix", &CameraComponent::GetProjectionMatrix
	);

	// CameraControllerComponent
	m_lua.new_usertype<CameraControllerComponent>("CameraControllerComponent",
		sol::constructors<CameraControllerComponent()>(),
		sol::base_classes, sol::bases<Component>(),
		"Init", &CameraControllerComponent::Init,
		"Update", &CameraControllerComponent::Update,
		"Shutdown", &CameraControllerComponent::Shutdown
	);

	// CollisionComponent
	m_lua.new_usertype<CollisionComponent>("CollisionComponent",
		sol::constructors<CollisionComponent()>(),
		sol::base_classes, sol::bases<Component>(),
		"Init", &CollisionComponent::Init,
		"Update", &CollisionComponent::Update,
		"Shutdown", &CollisionComponent::Shutdown,
		"SetCollisionLayer", &CollisionComponent::SetCollisionLayer,
		"GetCollisionLayer", &CollisionComponent::GetCollisionLayer,
		"CanCollideWith", &CollisionComponent::CanCollideWith
	);

	// ScriptComponent
	m_lua.new_usertype<ScriptComponent>("ScriptComponent",
		sol::constructors<ScriptComponent()>(),
		sol::base_classes, sol::bases<Component>(),
		"Init", &ScriptComponent::Init,
		"Update", sol::overload(
			static_cast<void (ScriptComponent::*)(double)>(&ScriptComponent::Update),
			static_cast<void (ScriptComponent::*)()>(&ScriptComponent::Update)
		),
		"Shutdown", &ScriptComponent::Shutdown,
		"LoadScript", &ScriptComponent::LoadScript,
		"ExposeClasses", &ScriptComponent::CreateAllBindings,
		"TestFunc", &ScriptComponent::TestFunc,
		"GetTransformComponent", &ScriptComponent::GetTransformComponent,
		"GetRenderComponent", &ScriptComponent::GetRenderComponent,
		"GetPhysicsComponent", &ScriptComponent::GetPhysicsComponent,
		"GetControllerComponent", &ScriptComponent::GetControllerComponent,
		"GetCollisionComponent", &ScriptComponent::GetCollisionComponent,
		"GetScriptComponent", &ScriptComponent::GetScriptComponent,
		"GetCameraComponent", &ScriptComponent::GetCameraComponent,
		"GetParticleComponent", &ScriptComponent::GetParticleComponent,
		"GetGrappleComponent", &ScriptComponent::GetGrappleComponent,
		"BindCollisionBoxEvent", &ScriptComponent::BindCollisionBoxEvent,
		"FindUIComponent", &ScriptComponent::FindUIComponent,
		"SetUITextComponentActiveState", &ScriptComponent::SetUITextComponentActiveState,
		"GetCurrentSceneName", &ScriptComponent::GetCurrentSceneName
	);

	// ParticleComponent
	m_lua.new_usertype<ParticleComponent>("ParticleComponent",
		sol::constructors<ParticleComponent()>(),
		sol::base_classes, sol::bases<Component>(),
		"Toggle", &ParticleComponent::Toggle,
		"SetPosition", &ParticleComponent::SetPosition,
		"SetVelocity", &ParticleComponent::SetVelocity,
		"SetVelocityVariation", &ParticleComponent::SetVelocityVariation,
		"SetColorBegin", &ParticleComponent::SetColorBegin,
		"SetColorEnd", &ParticleComponent::GetColorEnd,
		"SetSizeBegin", &ParticleComponent::SetSizeBegin,
		"SetSizeEnd", &ParticleComponent::SetSizeEnd,
		"SetSizeVariation", &ParticleComponent::SetSizeVariation,
		"SetLifetime", &ParticleComponent::SetLifetime
	);

	// GrappleComponent
	m_lua.new_usertype<GrappleComponent>("GrappleComponent",
		sol::constructors<GrappleComponent()>(),
		sol::base_classes, sol::bases<Component>(),
		"ReelGrappleIn", &GrappleComponent::ReelGrappleIn,
		"ReelGrappleOut", &GrappleComponent::ReelGrappleOut,
		"CastGrapple", sol::overload(
			static_cast<std::optional<glm::vec3>(GrappleComponent::*)(glm::vec3)>(&GrappleComponent::CastGrapple),
			static_cast<std::optional<glm::vec3>(GrappleComponent::*)(glm::vec3, glm::vec3) const>(&GrappleComponent::CastGrapple)
		),
		"AttachGrapple", &GrappleComponent::AttachGrapple,
		"ReleaseGrapple", &GrappleComponent::ReleaseGrapple,
		"SetMinimumGrappleLength", &GrappleComponent::SetMinimumGrappleLength,
		"SetMaximumGrappleLength", &GrappleComponent::SetMaximumGrappleLength,
		"SetReelSpeed", &GrappleComponent::SetReelSpeed,
		"GetAttachPoint", &GrappleComponent::GetAttachPoint,
		"GetDistanceFromAttachPoint", &GrappleComponent::GetDistanceFromAttachPoint,
		"GetMinLength", &GrappleComponent::GetMinLength,
		"GetMaxLength", &GrappleComponent::GetMaxLength,
		"GetGrappleVector", &GrappleComponent::GetGrappleVector,
		"GetGrappleLength", &GrappleComponent::GetGrappleLength,
		"GetGrappleDirection", &GrappleComponent::GetGrappleDirection,
		"GetReelSpeed", &GrappleComponent::GetReelSpeed,
		"IsAttached", &GrappleComponent::IsAttached
	);

	//m_lua.new_usertype<GameObjectManager>("GameObjectManager",
	//	"FindObject", GameObjectManager::GetGameObject
	//);
}