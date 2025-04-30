#pragma once
#include "Component.h"
class LightComponent :
    public Component
{
public:
    LightComponent();
    ~LightComponent();
    
    void Init();
    void Shutdown();

    void Update();

    void SetUpShadowPass() const;
    void GenerateShadowMap();
    void CleanUpShadowPass();
    void DebugDrawShadows();

    void SetPosition(glm::vec3 _position) { position = _position; }
    void SetAmbient(glm::vec3 _ambient) { ambient = _ambient; }
    void SetDiffuse(glm::vec3 _diffuse) { diffuse = _diffuse; }
    void SetSpecular(glm::vec3 _specular) { specular = _specular; }

    glm::vec3 GetPosition() const { return position; }
    glm::vec3 GetAmbient() const { return ambient; }
    glm::vec3 GetDiffuse() const { return diffuse; }
    glm::vec3 GetSpecular() const { return specular; }
    float GetNearPlane() { return near_plane; }
    float GetFarPlane() { return far_plane; }
    glm::mat4 GetLightSpaceMatrix() { return lightSpaceMatrix; }
    GLuint GetDepthMap() { return depthMap; }

private:

    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    glm::mat4 lightSpaceMatrix;
    float near_plane, far_plane;
    GLuint depthMap;
    GLuint depthMapFBO;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    void defineMember() override
    {
        m_setters["position"] = [this](std::any value) { SetPosition(std::any_cast<glm::vec3>(value)); };
        m_setters["ambient"] = [this](std::any value) { SetAmbient(std::any_cast<glm::vec3>(value)); };
        m_setters["diffuse"] = [this](std::any value) { SetDiffuse(std::any_cast<glm::vec3>(value)); };
        m_setters["specular"] = [this](std::any value) { SetSpecular(std::any_cast<glm::vec3>(value)); };


        m_getters["position"] = [this]() -> std::any { return position; };
        m_getters["ambient"] = [this]() -> std::any { return ambient; };
        m_getters["diffuse"] = [this]() -> std::any { return diffuse; };
        m_getters["specular"] = [this]() -> std::any { return specular; };
    }
};

