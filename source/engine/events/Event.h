// Author: Lane Thompson
#pragma once

class Event
{
public:
	Event(std::string name) : name(name) {};
	virtual std::string GetName() const { return this->name; }

protected:
	std::string name;
};

