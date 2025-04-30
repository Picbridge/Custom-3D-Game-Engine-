#pragma once

class NodeEvent : public Event
{
public:
	NodeEvent(std::string _name, Node* _node) :
		Event(_name), node(_node) {}
	Node* node;
};