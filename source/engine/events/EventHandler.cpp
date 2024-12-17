#include "../pch.h"
#include "EventHandler.h"
#include "EventListener.h"

std::unique_ptr<EventHandler> EventHandler::instance = nullptr;

EventHandler* EventHandler::GetInstance()
{
	if (instance == nullptr) {
		instance = std::unique_ptr<EventHandler>(new EventHandler());
	}
	return instance.get();
}

void EventHandler::Update()
{
	while (!eventQueue.empty())
	{
		Event* event = eventQueue.front();

		for (auto& subscription : subscriptionMap)
		{
			if (subscription.first == event->GetName())
			{
				subscription.second->EventCallback(subscription.first, event);
			}
		}
		eventQueue.pop();
	}
}

void EventHandler::AddEvent(Event* event)
{
	this->eventQueue.push(event);
}

void EventHandler::AddSubscription(std::string name, EventListener* eventListener)
{
	this->subscriptionMap.insert(make_pair(name, eventListener));
}

void EventHandler::RemoveSubscription(std::string name, EventListener* listener)
{
	auto range = subscriptionMap.equal_range(name);
	for (auto it = range.first; it != range.second; ++it)
	{
		if (it->second == listener)
		{
			subscriptionMap.erase(it);
			break;
		}
	}
}
