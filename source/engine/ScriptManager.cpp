#include "pch.h"
#include "ScriptManager.h"
#include "ScriptComponent.h"


void TestPrint(std::string s) 
{
	std::cout << "LUA: testPrint() string: " << s << std::endl;
}

// Required reading:
// https://sol2.readthedocs.io/en/latest/tutorial/functions.html
// TODO? https://sol2.readthedocs.io/en/latest/api/variadic_args.html

void LuaTest() 
{
	sol::state lua;

	// Call Lua function in script file from C++.
	lua.open_libraries(sol::lib::base);
	lua.script_file("sample-1.lua");
	float x = lua["addAndDouble"](5, 7);
	std::cout << "LUA: addAndDouble(5, 7) = " << x << std::endl;

	// Call C++ function from Lua (from C++). 
	lua.set("testPrint", TestPrint);
	lua.script("testPrint('meow')");
}


std::unique_ptr <ScriptManager> ScriptManager::instance = nullptr;

ScriptManager* ScriptManager::GetInstance() 
{
	if (!instance)
	{
		instance = std::unique_ptr<ScriptManager>(new ScriptManager());
	}
	return instance.get();
}

ScriptManager::ScriptManager() 
{
	Init();
}

void ScriptManager::Init() 
{
	m_lua.open_libraries(sol::lib::base);

	ExposeClasses();
}

void ScriptManager::Update(double deltaTime)
{
	for (auto& component : m_scriptComponents)
	{
		component->Update(deltaTime);
	}
}

void ScriptManager::Shutdown() 
{
	printf("ScriptManager Shutdown\n");
	while (!m_scriptComponents.empty())
	{
		delete m_scriptComponents[0];
	}
	m_scriptComponents.clear();
}

void ScriptManager::ExposeClasses() 
{
	// We need to expose the following:
	// GameObject (and by extension, probably Node).
	// Each Component class and their public members, so ScriptComponents can affect their siblings.
	// Child nodes and their components.
	// Utility classes like Input, etc.
}

void ScriptManager::AddScriptComponent(ScriptComponent* component)
{
	m_scriptComponents.push_back(component);
}

void ScriptManager::RemoveScriptComponent(ScriptComponent* component)
{
	auto it = std::find(m_scriptComponents.begin(), m_scriptComponents.end(), component);
	if (it != m_scriptComponents.end())
	{
		m_scriptComponents.erase(it);
	}
}
