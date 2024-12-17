#pragma once

enum CollisionType
{
	OBSTACLE = 1 << 0,
	OBJECT = 1 << 1
};

class sample : public Game
{
public:
	//@brief Constructor
	sample() : Game() {}

	//@brief Constructor
	sample(WINDOW_PROPS props) : Game(props) {}

	//@brief Constructor
	sample(int width, int height, std::string title) :Game(width, height, title) {}

	void Init() override;

	void Update() override;

	void PostUpdate() override;

	void Render() override;

	void Shutdown() override;
private:
	ParticleProps m_Particle;
	ParticleSystem m_ParticleSystem;
	int count = 0;
};