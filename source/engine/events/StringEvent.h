#pragma once

class StringEvent : public Event
{
public:
	StringEvent(std::string _name, std::string _value) :
		Event(_name), value(_value) {}
	std::string value;
};