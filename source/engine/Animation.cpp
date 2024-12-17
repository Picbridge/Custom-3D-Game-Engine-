#include "pch.h"
#include "Model.h"
#include "Animation.h"

Animation::Animation(Model* _model)
{
	model = _model;
	currentTime = 0.0f;
}

void Animation::Update(float _deltaTime)
{
	deltaTime = _deltaTime;
	currentTime += static_cast<float>(ticksPerSecond) * deltaTime;
	currentTime = static_cast<float>(fmod(currentTime, duration));

	for (auto& pair : model->boneMap)
	{
		pair.second.Update(currentTime);
	}
}
