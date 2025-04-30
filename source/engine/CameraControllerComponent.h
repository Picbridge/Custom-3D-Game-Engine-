#pragma once
class CameraComponent;

class CameraControllerComponent : public Component
{
public:
    CameraControllerComponent();
    ~CameraControllerComponent();

    void Init() override;
    void Update() override;
    void Shutdown() override;

private:
    const bool& m_focussedOnUI;
    bool m_invert;
    bool m_leftDown;
    bool m_rightDown;
    bool m_middleDown;
    float m_speed;
    float m_prevMouseX;
    float m_prevMouseY;
	float m_zoom;

    Input* m_pInputHandler;
	Transform* m_pTransform;
	CameraComponent* m_pCamera;

	// CameraComponent movement
    glm::vec3 m_pos;
    glm::vec3 m_rot;

    void updateMouseClick();
    void updateMouseMovement(const double& x, const double& y);
    void updateKeyboardMovement();
    void defineMember() override {};
};