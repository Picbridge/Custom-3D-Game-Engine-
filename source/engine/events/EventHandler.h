// Author: Lane Thompson
#pragma once
class EventListener;

class EventHandler
{
private:
	std::queue<Event*> eventQueue;
	std::multimap<std::string, EventListener*> subscriptionMap;

	static std::unique_ptr<EventHandler> instance;

	EventHandler  () {};

public:
	~EventHandler () {};
	static EventHandler* GetInstance();

	void Update();

	void AddEvent(Event* event);
	void AddSubscription(std::string, EventListener*);
	void RemoveSubscription(std::string name, EventListener* listener);
};