#include "../pch.h"
#include "ScriptComponent.h"
#include "ScriptManager.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "../CameraControllerComponent.h"
#include "../physics/PhysicsComponent.h"
#include "../physics/CollisionComponent.h"
#include "../ParticleComponent.h"

// Default directory where all scripts are stored
std::filesystem::path scriptDirectory = Utils::GetExecutableDirectory().parent_path().parent_path().parent_path() / "content" / "code" / "scripts";

ScriptComponent::ScriptComponent() : ScriptComponent("")
{}

ScriptComponent::ScriptComponent(std::string filepath) : Component(), m_scriptFilepath(filepath)
{
	m_lua.open_libraries(sol::lib::base, sol::lib::math);
	m_lua.open_libraries(sol::lib::base, sol::lib::string);
	SERVICE_LOCATOR.GetScriptManager()->AddScriptComponent(this);
	defineMember();
	// Expose classes
	CreateAllBindings();
}
ScriptComponent::~ScriptComponent()
{
	SERVICE_LOCATOR.GetScriptManager()->RemoveScriptComponent(this);
	if (m_pEventListener)
		delete(m_pEventListener);
}

void ScriptComponent::Init()
{
	// Open script
	if (m_scriptFilepath != "")
		m_lua.script_file((scriptDirectory / m_scriptFilepath).string());
	// Call Init() in Lua
	if (m_lua["Init"].valid())
		m_lua["Init"]();
}


void ScriptComponent::Update(double deltaTime)
{
	if (m_lua["Update"].valid())
	{
		m_lua["Update"](deltaTime);
	}
}

void ScriptComponent::Shutdown()
{
	// Run Shutdown() in Lua.
	// TODO: ScriptComponent::Shutdown() is never called. Fix that if we really need it?
	if (m_lua["Shutdown"].valid())
		m_lua["Shutdown"]();
}

void ScriptComponent::LoadScript(std::string filename)
{
	std::cout << "ScriptComponent::LoadScript(): " << filename << std::endl;
	SetScriptFilepath(filename);
	Init();
}

void ScriptComponent::TestFunc() {
	std::cout << "Hello from TestFunc()!" << std::endl;
}