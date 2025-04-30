#pragma once
#include "Component.h"
#include "CollisionComponent.h"
class Collision
{
public:
    Collision(GameObject* collider1, GameObject* collider2);
    Collision(CollisionComponent* collider1, CollisionComponent* collider2) : Collision(collider1->GetOwnerAsGameObject(), collider2->GetOwnerAsGameObject()) {}
    Collision(const Collision& other) = default;
    ~Collision() = default;

    // Getters
    inline GameObject*          GetObject1          () const { return m_object1; }
    inline GameObject*          GetObject2          () const { return m_object2; }
    inline CollisionComponent*  GetCollider1        () const { return GetObject1()->GetComponent<CollisionComponent>(); }
    inline CollisionComponent*  GetCollider2        () const { return GetObject2()->GetComponent<CollisionComponent>(); }
    inline glm::dvec3           GetCollisionNormal  () const { return m_collisionNormal; }
    inline double               GetPenetrationDepth () const { return m_penetrationDepth; }
    inline bool                 IsColliding         () const { return m_colliding; }
    inline bool                 IsResolved          () const { return m_resolved; }
    //@brief Returns if the collision is between two dynamic objects
    //@return bool True if both collided objects are dynamic
    inline bool                 IsDynamicCollision  () const { return m_dynamicCollision; }
    //@brief Returns the dynamic object of the collision
    //@return GameObject* The first dynamic object of the collision or nullptr if neither object is dynamic
    inline GameObject*          GetDynamicObject    () const { return GetDynamicComponent()->GetOwnerAsGameObject(); }
    //@brief Returns the static object of the collision
    //@return GameObject* The first static object of the collision or nullptr if neither object is static
    inline GameObject*          GetStaticObject     () const { return GetStaticComponent()->GetOwnerAsGameObject(); }
    //@brief Returns if the collision is a trigger
    //@return bool True if the collision is a trigger
    inline bool                 IsTrigger           () const { return m_isTrigger; }

    void                        ResolveCollision    ();
    //@brief Returns the dynamic collision component
    //@return CollisionComponent* The first dynamic collision component or nullptr if neither component is Dynamic
    CollisionComponent*         GetDynamicComponent () const;
    //@brief Returns the static collision component
    //@return CollisionComponent* The first static collision component or nullptr if neither component is Static
    CollisionComponent*         GetStaticComponent  () const;
private:
    // Setters
    inline void SetCollisionNormal  (const glm::dvec3 normal) 
    { 
        assert(normal != glm::dvec3(0) && "CollisionNormal will be NaN");
        m_collisionNormal = glm::normalize(normal);
    }
    inline void SetContactPoints    (const std::vector<glm::dvec3> points) { m_contactPoints = points; }
    inline void SetPenetrationDepth (const double depth)                   { m_penetrationDepth = depth; }
    inline void SetColliding        (const bool colliding)                 { m_colliding = colliding; }
    inline void SetResolved         (const bool resolved)                  { m_resolved = resolved; }
    inline void SetDynamicCollision (const bool dynamic)                   { m_dynamicCollision = dynamic; }
    inline void SetIsTrigger        (const bool trigger)                   { m_isTrigger = trigger; }

    void FillCollisionData();
    bool CheckForCollision();
    bool SAT();
    bool SphereToCuboid();
    bool SphereToSphere();

    std::vector<glm::dvec3> GetAxes (const CollisionShape* shape) const;

    GameObject*             m_object1;          // The first object that is colliding
    GameObject*             m_object2;          // The second object that is colliding
    glm::dvec3              m_collisionNormal;  // The normal of the collision (if dynamic, from object1 to object2; if not, from static to dynamic)
    std::vector<glm::dvec3> m_contactPoints;    // Not Used
    double                  m_penetrationDepth; // The depth of the penetration
    bool                    m_colliding;        // If the objects are colliding
    bool                    m_resolved;         // If the collision has been resolved
    bool                    m_dynamicCollision; // If the collision is between two dynamic objects
    bool                    m_isTrigger;        // If the collision is a trigger
};

