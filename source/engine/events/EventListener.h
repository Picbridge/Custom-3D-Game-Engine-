// Author: Lane Thompson
#pragma once

class EventListener
{
private:
    std::map<std::string, std::function<void(Event*)>> subscribedEventMap;

public:
    ~EventListener();
    void BroadcastEvent(Event*);
    void SubscribeToEvent(std::string name, std::function<void(Event*)> callback);
    void UnsubscribeFromEvent(std::string name);
    void EventCallback(std::string, Event* event);
};