// Author: Lane Thompson
#pragma once

class Event
{
public:
	virtual std::string GetName() const { return this->name; }

protected:
	Event(std::string name) : name(name) {};
	std::string name;
};

