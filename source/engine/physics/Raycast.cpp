#include "../pch.h"
#include "Raycast.h"


Raycast::Raycast(glm::dvec3 origin, glm::dvec3 direction, CollisionShape* shape)
    : m_origin(origin), m_direction(glm::normalize(direction)), m_shapeHit(nullptr),
      m_hitPoint(glm::dvec3(0)), m_hitNormal(glm::dvec3(0)), m_length(0)
{
    auto hitpoint = shape->GetHitPoint(m_origin, m_direction);
    if (hitpoint != std::nullopt)
    {
        m_shapeHit = shape;
        m_hitPoint = hitpoint.value();
        m_length = glm::length(m_hitPoint - m_origin);
        
        auto hitNormal = shape->GetHitNormal(m_origin, m_direction); 
        if (hitNormal != std::nullopt)
        {
            m_hitNormal = hitNormal.value();
        }
    }
}

Raycast::Raycast() 
    : m_origin(glm::dvec3(0)), m_direction(glm::dvec3(0)), m_shapeHit(nullptr),
      m_hitPoint(glm::dvec3(0)), m_hitNormal(glm::dvec3(0)), m_length(0)
{
}

Raycast::~Raycast()
{
}