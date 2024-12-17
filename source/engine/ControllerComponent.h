#pragma once

class ControllerComponent : public Component
{
private:
	const float m_speed = 0.0015f;
	const Input* m_InputHandler = SERVICE_LOCATOR.GetInput();
	void defineMember() override {}
public:
	ControllerComponent() {}

	void Init() override;
	void Update() override;
	void Shutdown() override;

};