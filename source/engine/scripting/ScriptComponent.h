#pragma once
class Component;
class TransformComponent;
class RenderComponent;
class PhysicsComponent;
class CameraControllerComponent;
class CollisionComponent;
class ParticleComponent;
class CameraComponent;
class GrappleComponent;

class Node;

class ScriptComponent : public Component
{
public:
	ScriptComponent();
	ScriptComponent(std::string filepath);
	~ScriptComponent();

	void Init() override;
	void Update(double deltaTime);
	void Update() override { Update(1 / 60.0); }
	void Shutdown() override;

	inline void SetScriptFilepath(std::string filepath) { m_scriptFilepath = filepath; }
	void LoadScript(std::string filepath);
	void CreateAllBindings();
	void CreateGLMBindings();
	void TestFunc();

	std::string GetScriptFilepath() { return m_scriptFilepath; }
	TransformComponent* GetTransformComponent(Node* parent);
	RenderComponent* GetRenderComponent(Node* parent);
	PhysicsComponent* GetPhysicsComponent(Node* parent);
	CameraControllerComponent* GetControllerComponent(Node* parent);
	CollisionComponent* GetCollisionComponent(Node* parent);
	ScriptComponent* GetScriptComponent(Node* parent);
	ParticleComponent* GetParticleComponent(Node* parent);
	CameraComponent* GetCameraComponent(Node* parent);
	GrappleComponent* GetGrappleComponent(Node* parent);


	void CollisionEventCallback(Event* event);
	void BindCollisionBoxEvent();
	UIComponent* FindUIComponent(std::string name);
	std::string GetCurrentSceneName();
	void SetUITextComponentActiveState(std::string name, bool state);

	template<typename... Args>
	void CallLuaFunction(const std::string& funcName, Args&&... args)
	{
		if (m_lua[funcName].valid())
		{
			m_lua[funcName](std::forward<Args>(args)...);
		}
		else
		{
			printf("Lua function %s not found\n", funcName.c_str());
		}
	}

private:
	sol::state m_lua;
	std::string m_scriptFilepath; // Misnamed and can't be changed right now. This should only contain the script's filename.
	EventListener* m_pEventListener = nullptr;

	void defineMember() override
	{
		m_setters["scriptFilepath"] = [this](std::any val) { this->SetScriptFilepath(std::any_cast<std::string>(val)); };

		m_getters["scriptFilepath"] = [this]() -> std::any { return this->GetScriptFilepath(); };
	}

};