#pragma once

class IndexEvent : public Event
{
public:
	IndexEvent(std::string _name, int _index) :
		Event(_name), index(_index) {}
	int index;
};