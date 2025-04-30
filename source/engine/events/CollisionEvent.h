#pragma once
class CollisionEvent : public Event
{
public:
    CollisionEvent() : Event(""),m_collision(nullptr) {};
    ~CollisionEvent() { delete m_collision; }
    CollisionEvent(std::string name, Collision* collision) : Event(name), m_collision(collision) {};
    Collision* m_collision;
};

