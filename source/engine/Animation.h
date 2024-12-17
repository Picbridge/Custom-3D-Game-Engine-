#pragma once

class Animation
{
public:
	// Duration of animation
	double duration;
	double ticksPerSecond;

	float currentTime;
	float deltaTime;

	Model* model;

	Animation(Model* _model);
	void Update(float _deltaTime);
};



