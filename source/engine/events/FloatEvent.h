#pragma once

class FloatEvent : public Event
{
public:
	FloatEvent(std::string _name, float _value) :
		Event(_name), value(_value) {}
	float value;
};