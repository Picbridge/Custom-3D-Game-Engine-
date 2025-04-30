#pragma once
#include "Component.h"
class ParticleComponent :
    public Component
{
public:
    ParticleComponent();
    ~ParticleComponent();

    void Init() override;
    void Update() override;
    void Shutdown() override;

    void SetPosition(glm::vec3 _position) { position = _position; }
    void SetVelocity(glm::vec3 _velocity) { velocity = _velocity; }
    void SetVelocityVariation(glm::vec3 _velocityVariation) { velocityVariation = _velocityVariation; }
    void SetColorBegin(glm::vec4 _colorBegin) { colorBegin = _colorBegin; }
    void SetColorEnd(glm::vec4 _colorEnd) { colorEnd = _colorEnd; }
    void SetSizeBegin(float _sizeBegin) { sizeBegin = _sizeBegin; }
    void SetSizeEnd(float _sizeEnd) { sizeEnd = _sizeEnd; }
    void SetSizeVariation(float _sizeVariation) { sizeVariation = _sizeVariation; }
    void SetLifetime(float _lifetime) { lifetime = _lifetime; }

    glm::vec3 GetPosition() { return position; }
    glm::vec3 GetVelocity() { return velocity; }
    glm::vec3 GetVelocityVariation() { return velocityVariation; }
    glm::vec4 GetColorBegin() { return colorBegin; }
    glm::vec4 GetColorEnd() { return colorEnd; }
    float GetSizeBegin() { return sizeBegin; }
    float GetSizeEnd() { return sizeEnd; }
    float GetSizeVariation() { return sizeVariation; }
    float GetLifetime() { return lifetime; }

    void Toggle() { toggle = !toggle; }

private:
    glm::vec3 position;
    glm::vec3 velocity, velocityVariation;
    glm::vec4 colorBegin, colorEnd;
    float sizeBegin, sizeEnd, sizeVariation;
    float lifetime = 1.0f;
    bool toggle = true;
    int timer = 0;
    
    void defineMember() override
    {
        m_setters["position"] = [this](std::any value) { SetPosition(std::any_cast<glm::vec3>(value)); };
        m_setters["velocity"] = [this](std::any value) { SetVelocity(std::any_cast<glm::vec3>(value)); };
        m_setters["velocityVariation"] = [this](std::any value) { SetVelocityVariation(std::any_cast<glm::vec3>(value)); };
        m_setters["colorBegin"] = [this](std::any value) { SetColorBegin(std::any_cast<glm::vec4>(value)); };
        m_setters["colorEnd"] = [this](std::any value) { SetColorEnd(std::any_cast<glm::vec4>(value)); };
        m_setters["sizeBegin"] = [this](std::any value) { SetSizeBegin(std::any_cast<float>(value)); };
        m_setters["sizeEnd"] = [this](std::any value) { SetSizeEnd(std::any_cast<float>(value)); };
        m_setters["sizeVariation"] = [this](std::any value) { SetSizeVariation(std::any_cast<float>(value)); };
        m_setters["lifetime"] = [this](std::any value) { SetLifetime(std::any_cast<float>(value)); };

        m_getters["position"] = [this]() -> std::any { return position; };
        m_getters["velocity"] = [this]() -> std::any { return velocity; };
        m_getters["velocityVariation"] = [this]() -> std::any { return velocityVariation; };
        m_getters["colorBegin"] = [this]() -> std::any { return colorBegin; };
        m_getters["colorEnd"] = [this]() -> std::any { return colorEnd; };
        m_getters["sizeBegin"] = [this]() -> std::any { return sizeBegin; };
        m_getters["sizeEnd"] = [this]() -> std::any { return sizeEnd; };
        m_getters["sizeVariation"] = [this]() -> std::any { return sizeVariation; };
        m_getters["lifetime"] = [this]() -> std::any { return lifetime; };

    }
};

