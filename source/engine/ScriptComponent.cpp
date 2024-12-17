#include "pch.h"
#include "ScriptComponent.h"
#include "ScriptManager.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "ControllerComponent.h"
#include "physics/PhysicsComponent.h"
#include "physics/CollisionComponent.h"

ScriptComponent::ScriptComponent() : Component(),
m_scriptFilepath()
{
	SERVICE_LOCATOR.GetScriptManager()->AddScriptComponent(this);
	defineMember();
}

ScriptComponent::ScriptComponent(std::string filepath) : Component(),
m_scriptFilepath(filepath)
{
	SERVICE_LOCATOR.GetScriptManager()->AddScriptComponent(this);
}
ScriptComponent::~ScriptComponent()
{
	SERVICE_LOCATOR.GetScriptManager()->RemoveScriptComponent(this);
}

void ScriptComponent::Init()
{
	m_lua.open_libraries(sol::lib::base, sol::lib::math);
	// Expose classes
	ExposeClasses();
	// Open script
	if (m_scriptFilepath != "")
		m_lua.script_file(m_scriptFilepath);
	// Call Init() in Lua
	if (m_lua["Init"].valid())
		m_lua["Init"]();
}

void ScriptComponent::Update(double deltaTime)
{

	if (m_lua["Update"].valid())
	{
		m_lua["Update"](deltaTime);
		/*
		sol::protected_function f = m_lua["Update"];
		f.error_handler = m_lua["Handler"]; // TODO: error_handler not a member of protected_function??? But it's in the example here: https://sol2.readthedocs.io/en/latest/tutorial/functions.html
		sol::protected_function_result result = f(deltaTime);
		if (result.valid())
		{
			// Update(deltaTime) shouldn't return anything. Do nothing.
		}
		else
		{
			// Call failed
			sol::error err = result;
			std::string what = err.what();
		}
		*/
	}
}

void ScriptComponent::Shutdown()
{
	// Run Shutdown() in Lua.
	// TODO: ScriptComponent::Shutdown() is never called. Fix that if we really need it?
	if (m_lua["Shutdown"].valid())
		m_lua["Shutdown"]();
}

void ScriptComponent::LoadScript(std::string filepath) 
{
	std::cout << "ScriptComponent::LoadScript(): " << filepath << std::endl;
	m_scriptFilepath = filepath;
	m_lua.script_file(m_scriptFilepath);
	// Call Init() in Lua
	if (m_lua["Init"].valid())
		m_lua["Init"]();
}

void ScriptComponent::ExposeClasses() {
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
		"NeedsDeletion", &Node::NeedsDeletion
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
		"IsDead", &GameObject::IsDead
	);

	// Component (base)
	m_lua.new_usertype<Component>("Component",
		"Init", &Component::Init,
		"Update", &Component::Update,
		"Shutdown", &Component::Shutdown,
		"SetOwner", &Component::SetOwner,
		"GetOwner", &Component::GetOwner,
		"GetOwnerAsGameObject", &Component::GetOwnerAsGameObject
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
			static_cast<void (PhysicsComponent::*)(const glm::dvec3&)>(&PhysicsComponent::ApplyForce), // These glm structs don't cause compiler errors here, probably because we're doing &. May or may not actually work.
			static_cast<void (PhysicsComponent::*)(double, double, double)>(&PhysicsComponent::ApplyForce)
		),
		"ApplyTorque", sol::overload(
			static_cast<void (PhysicsComponent::*)(const glm::dvec3&)>(&PhysicsComponent::ApplyTorque),
			static_cast<void (PhysicsComponent::*)(double, double, double)>(&PhysicsComponent::ApplyTorque)
		),
		"SetGrounded", &PhysicsComponent::SetGrounded,
		"SetVelocity", sol::overload(
			static_cast<void (PhysicsComponent::*)(const glm::dvec3&)>(&PhysicsComponent::SetVelocity),
			static_cast<void (PhysicsComponent::*)(double, double, double)>(&PhysicsComponent::SetVelocity)
		),
		"SetMass", &PhysicsComponent::SetMass,
		"SetDrag", &PhysicsComponent::SetDrag,
		"SetGravityMultiplyer", &PhysicsComponent::SetGravityMultiplyer,
		"GetGrounded", &PhysicsComponent::Grounded,
		"GetVelocity", &PhysicsComponent::GetVelocity,
		"GetMass", &PhysicsComponent::GetMass,
		"GetInverseMass", &PhysicsComponent::GetInverseMass,
		"GetDrag", &PhysicsComponent::GetDrag,
		"GetGravityMultiplyer", &PhysicsComponent::GetGravityMultiplyer
	);

	// ControllerComponent
	m_lua.new_usertype<ControllerComponent>("ControllerComponent",
		sol::constructors<ControllerComponent()>(),
		sol::base_classes, sol::bases<Component>(),
		"Init", &ControllerComponent::Init,
		"Update", &ControllerComponent::Update,
		"Shutdown", &ControllerComponent::Shutdown
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
		"ExposeClasses", &ScriptComponent::ExposeClasses,
		"TestFunc", &ScriptComponent::TestFunc,
		"GetTransformComponent", &ScriptComponent::GetTransformComponent,
		"GetRenderComponent", &ScriptComponent::GetRenderComponent,
		"GetPhysicsComponent", &ScriptComponent::GetPhysicsComponent,
		"GetControllerComponent", &ScriptComponent::GetControllerComponent,
		"GetCollisionComponent", &ScriptComponent::GetCollisionComponent
	);

	// Expose this class instance!
	m_lua["my_script"] = this;
}

void ScriptComponent::TestFunc() {
	std::cout << "Hello from TestFunc()!" << std::endl;
}

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

ControllerComponent* ScriptComponent::GetControllerComponent(Node* parent)
{
	return parent->GetComponent<ControllerComponent>();
}

CollisionComponent* ScriptComponent::GetCollisionComponent(Node* parent)
{
	return parent->GetComponent<CollisionComponent>();
}

ScriptComponent* ScriptComponent::GetScriptComponent(Node* parent)
{
	return parent->GetComponent<ScriptComponent>();
}

void ScriptComponent::defineMember()
{
	m_setters["scriptFilepath"] = [this](std::any val) { this->LoadScript(std::any_cast<std::string>(val)); };

	m_getters["scriptFilepath"] = [this]() -> std::any { return this->GetScriptFilepath(); };
}
