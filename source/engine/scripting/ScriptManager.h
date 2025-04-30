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
	void AddScriptComponent(ScriptComponent* component);
	void RemoveScriptComponent(ScriptComponent* component);
	
	ScriptComponent* FindScriptComponent(std::string objName);

private:
	ScriptManager();

	static std::unique_ptr<ScriptManager> instance;
	static ScriptManager* GetInstance();

	friend class ServiceLocator;

	std::vector<ScriptComponent*> m_scriptComponents;
};