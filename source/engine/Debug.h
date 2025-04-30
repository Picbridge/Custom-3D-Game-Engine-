#pragma once
namespace Debug
{
    void DrawRay(glm::vec3 start, glm::vec3 direction, float length = 100,glm::vec3 color = glm::vec3(1,0,0), float width = 1);
    void DrawSphere(glm::dvec3 pos, glm::vec3 color = glm::vec3(1,0,0), float size = .25f);
    void DrawReticle();
};

