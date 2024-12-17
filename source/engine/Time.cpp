#include "pch.h"

std::unique_ptr<Time> Time::instance = nullptr;

Time* Time::GetInstance()
{
	if (instance == nullptr)
		instance = std::unique_ptr<Time>(new Time());

	return instance.get();
}

void Time::Init(double fixedDT)
{
	m_fixedDeltaTime = fixedDT;
	m_deltaTime = 0.0f;
	m_lastTime = std::chrono::high_resolution_clock::now();
}

void Time::Update()
{
	auto currTime = std::chrono::high_resolution_clock::now();
	m_deltaTime = std::chrono::duration<double>(currTime - m_lastTime).count();
	m_lastTime = currTime;
}
