// 
#pragma once
class ScriptComponent;

class ScriptManager
{
public:
	~ScriptManager() {}

	void Init();
	void Update(double deltaTime);
	void Shutdown();
	void ExposeClasses();
	void AddScriptComponent(ScriptComponent* component);
	void RemoveScriptComponent(ScriptComponent* component);

private:
	ScriptManager();

	static std::unique_ptr<ScriptManager> instance;
	static ScriptManager* GetInstance();

	friend class ServiceLocator;

	std::vector<ScriptComponent*> m_scriptComponents;
	sol::state m_lua; // Tentatively here - still determining if we really have need for a "global" state (for debug if nothing else).
};