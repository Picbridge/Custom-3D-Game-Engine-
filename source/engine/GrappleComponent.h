#pragma once
#include "Component.h"
#include "TransformComponent.h"
class GrappleComponent :
    public Component
{
public:
    //@brief Constructor for GrappleComponent
    //@param minGrappleLength : The minimum length the grapple can reach and retract to
    //@param maxGrappleLength : The maximum length the grapple can reach
    //@param reelSpeed : The speed the grapple will reel in or out
    //@param collisionLayer : The collision layer the grapple will interact with
    GrappleComponent(double minGrappleLength, double maxGrappleLength, double reelSpeed,int collisionLayer = 255);
    GrappleComponent();
    ~GrappleComponent();
    void Init() override {};
    void Update() override;
    void Shutdown() override {};

    //@brief Cast the grapple in the direction
    //@param grappleDirection : The direction to cast the grapple
    //@return The point the grapple hit
    std::optional<glm::vec3> CastGrapple(glm::vec3 grappleDirection);
    //@brief Cast the grapple from the origin in the direction
    //@param grappleOrigin : The origin of the grapple cast
    //@param grappleDirection : The direction to cast the grapple
    //@return The point the grapple hit
    std::optional<glm::vec3> CastGrapple(glm::vec3 grappleOrigin, glm::vec3 grappleDirection) const;
    //@brief Attach the grapple
    //@param attachPoint : The point the grapple attached to
    void AttachGrapple(glm::vec3 attachPoint);
    //@brief Handle logic for reeling the grapple in
    void ReelGrappleIn();
    //@brief Handle logic for reeling the grapple out
    void ReelGrappleOut();
    //@brief Handle logic for releasing the grapple
    void ReleaseGrapple();

    //@brief Set the minimum length the grapple can reach and retract to
    //@param length : The length to set
    inline void SetMinimumGrappleLength(float length) { m_minGrappleLength = length; }
    //@brief Set the maximum length the grapple can reach and extend to
    //@param length : The length to set
    inline void SetMaximumGrappleLength(float length) { m_maxGrappleLength = length; }
    //@brief Set the speed the grapple will reel in or out
    //@param speed : The speed to set
    inline void SetReelSpeed(float speed) { m_reelSpeed = speed; }
    //@brief Set the collision layer the grapple will interact with
    //@param layer : The layer to set
    inline void SetCollisionLayer(int layer) { m_collisionMask = layer; }

    inline glm::dvec3 GetAttachPoint()             const { return m_attachPoint; }
    inline double     GetDistanceFromAttachPoint() const { return glm::length(GetGrappleVector()); }
    inline float      GetMinLength()               const { return m_minGrappleLength; }
    inline float      GetMaxLength()               const { return m_maxGrappleLength; }
    // Get the vector from the cast point to the attach point
    inline glm::dvec3 GetGrappleVector()           const { return GetAttachPoint() - static_cast<glm::dvec3>(pOwner->GetComponent<TransformComponent>()->GetPosition()); }
    inline double     GetGrappleLength()           const { return m_grappleLength; }
    // Get the normalized vector from the cast point to the attach point
    inline glm::dvec3 GetGrappleDirection()        const { return glm::normalize(GetGrappleVector()); }
    inline float      GetReelSpeed()               const { return m_reelSpeed; }
    inline bool       IsAttached()                 const { return m_attached; }
    inline int        GetCollisionMask()          const { return m_collisionMask; }
private:
    //@brief Handle the physics of the grapple
    void GrapplePhysics();
    glm::dvec3 CalculateReelDifference() const;
    // The point the grapple is attached to
    glm::dvec3 m_attachPoint;
    // The length of the grapple
    double m_grappleLength;
    // The minimum length the grapple can retract to
    float m_minGrappleLength;
    // The maximum length the grapple can reach
    float m_maxGrappleLength;
    // The speed the grapple will reel in or out
    float m_reelSpeed;
    // If the grapple is currently attached
    bool m_attached;
    // The collision layer the grapple will interact with
    int m_collisionMask;

    void defineMember() override
    {
        m_setters["MinLength"] = [this](std::any val) { this->SetMinimumGrappleLength(std::any_cast<float>(val)); };
        m_setters["MaxLength"] = [this](std::any val) { this->SetMaximumGrappleLength(std::any_cast<float>(val)); };
        m_setters["ReelSpeed"] = [this](std::any val) { this->SetReelSpeed(std::any_cast<float>(val)); };
        m_setters["CollisionLayer"] = [this](std::any val) { this->SetCollisionLayer(std::any_cast<int>(val)); };

        //m_getters["GrappleDirection"] = [this]() -> std::any { return this->GetGrappleDirection(); };
        //m_getters["GrappleLength"] = [this]() -> std::any { return this->GetGrappleLength(); };
        //m_getters["GrappleVector"] = [this]() -> std::any { return this->GetGrappleVector(); };
        m_getters["MinLength"] = [this]() -> std::any { return this->GetMinLength(); };
        m_getters["MaxLength"] = [this]() -> std::any { return this->GetMaxLength(); };
        m_getters["ReelSpeed"] = [this]() -> std::any { return this->GetReelSpeed(); };
        m_getters["CollisionLayer"] = [this]() -> std::any { return this->GetCollisionMask(); };
        //m_getters["IsAttached"] = [this]() -> std::any { return this->IsAttached(); };
    }
};

