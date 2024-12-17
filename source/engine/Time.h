#pragma once

class Time
{
public:
	//@brief Initializes the time
	void Init(double fixedDT);
	//@brief Updates the time
	void Update();

	//@brief Returns the fixed delta time
	//@return float : Fixed delta time
	inline const double GetFixedDeltaTime() const { return m_fixedDeltaTime; }
	//@brief Returns the delta time
	//@return double : Delta time
	inline const double GetDeltaTime() const { return m_deltaTime; }

private:
	static Time* GetInstance();
	static std::unique_ptr<Time> instance;

	double m_fixedDeltaTime;
	double m_deltaTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_lastTime;

	friend class ServiceLocator;
};