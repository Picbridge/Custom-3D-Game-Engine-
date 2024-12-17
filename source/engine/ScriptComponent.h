#pragma once

class TransformComponent;
class RenderComponent;
class PhysicsComponent;
class ControllerComponent;
class CollisionComponent;

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

	void LoadScript(std::string filepath);
	void ExposeClasses();
	void TestFunc();

	std::string GetScriptFilepath() { return m_scriptFilepath; }
	TransformComponent* GetTransformComponent(Node* parent);
	RenderComponent* GetRenderComponent(Node* parent);
	PhysicsComponent* GetPhysicsComponent(Node* parent);
	ControllerComponent* GetControllerComponent(Node* parent);
	CollisionComponent* GetCollisionComponent(Node* parent);
	ScriptComponent* GetScriptComponent(Node* parent);
private:
	sol::state m_lua;
	std::string m_scriptFilepath;

	void defineMember() override;

};
