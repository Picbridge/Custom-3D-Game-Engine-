#pragma once
class Raycast
{
public:
    Raycast(glm::dvec3 origin, glm::dvec3 direction, CollisionShape* shape);
    Raycast();
    ~Raycast();
    inline bool            Hit()              const { return m_shapeHit != nullptr; }
    inline glm::dvec3      GetOrigin()        const { return m_origin; }
    inline glm::dvec3      GetDirection()     const { return m_direction; }
    inline CollisionShape* GetShapeHit()      const { return m_shapeHit; }
    inline glm::dvec3      GetHitPoint()      const { return m_hitPoint; }
    inline glm::dvec3      GetHitNormal()     const { return m_hitNormal; }
    inline double          GetLength()        const { return m_length; }
    //inline int             GetCollisionMask() const { return m_collisionMask; }
private:

    glm::dvec3 m_origin;
    glm::dvec3 m_direction;
    CollisionShape* m_shapeHit;
    glm::dvec3 m_hitPoint;
    glm::dvec3 m_hitNormal;
    double m_length;
    //int m_collisionMask;
};

