#pragma once

class ComponentItrEvent : public Event
{
public:
	ComponentItrEvent(std::string name, std::unordered_map<std::string, Component*>::iterator _itr) : 
		Event(name), itr(_itr) {}
	std::unordered_map<std::string, Component*>::iterator itr;
};