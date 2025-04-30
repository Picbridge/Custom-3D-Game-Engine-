#pragma once

class BoolEvent : public Event
{
public:
	BoolEvent(std::string _name, bool _value) :
		Event(_name), value(_value) {}
	bool value;
};