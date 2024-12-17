// Created by Garrett Grossner (Initial integration and quaterion support) and
// Cameron Allen (Refactor/cleanup and Input compatibility)
#pragma once

class Input;

class Camera
{
private:
	Input* mp_input;
public:
	glm::vec3 m_rot = glm::vec3();
	float m_speed;
	float m_lastTime;

	float m_tx;
	float m_ty;
	float m_zoom;

	float m_ry;
	float m_front;
	float m_back;
	bool m_invert = false;

	glm::mat4 m_worldProjection;
	glm::mat4 m_worldView;

	static Camera* GetInstance();
	void Update();

	void ToggleInvert() { m_invert = !m_invert; }
private:
	double m_prevMouseX = 0.0;
	double m_prevMouseY = 0.0;

	bool m_shifted = false;
	bool m_leftDown = false;
	bool m_middleDown = false;
	bool m_rightDown = false;
	const bool& m_focussedOnUI;

	static std::unique_ptr<Camera> mp_instance;
	Camera();

	void UpdateMouseKeyPress();
	void UpdateMouseMovement(const double& x, const double& y);
};

