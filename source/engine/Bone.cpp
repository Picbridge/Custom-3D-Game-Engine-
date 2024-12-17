#include "pch.h"

Bone::Bone()
{
}

// Update Bone Position
void Bone::Update(float animationTime)
{
	if (getKeyIndex(animationTime) == -1)
		return;
	glm::mat4 translation = InterpolatePosition(animationTime);
	glm::mat4 rotation = InterpolateRotation(animationTime);
	glm::mat4 scale = InterpolateScale(animationTime);
	transform = translation * rotation * scale;
}

// Get closest key frame
int Bone::getKeyIndex(float animationTime)
{
	for (int i = 0; i < numKeys - 1; i++)
	{
		if (animationTime < keyTimes[i + 1])
			return i;
	}

	return -1;
}

// Get t
float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
	float scaleFactor = 0.0f;
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	scaleFactor = midWayLength / framesDiff;
	return scaleFactor;
}

// Regular Lerp for position
glm::mat4 Bone::InterpolatePosition(float animationTime)
{
	int p0 = getKeyIndex(animationTime);
	int pn = p0 + 1;
	float scaleFactor = GetScaleFactor(static_cast<float>(keyTimes[p0]), static_cast<float>(keyTimes[pn]), animationTime);
	glm::vec3 position = (1 - scaleFactor) * keyFrames[p0].v + scaleFactor * keyFrames[pn].v;
	return glm::translate(glm::mat4(1.0f), position);
}

// Quaternion Slerp (handles edge case where 
// close interpolation frames cause stuttering
glm::mat4 Bone::InterpolateRotation(float animationTime)
{
	int p0 = getKeyIndex(animationTime);
	int pn = p0 + 1;
	float scaleFactor = GetScaleFactor(static_cast<float>(keyTimes[p0]), static_cast<float>(keyTimes[pn]), animationTime);
	float angle = Quaternion::Dot(keyFrames[p0].q / keyFrames[p0].q.Magnitude(), keyFrames[pn].q / keyFrames[pn].q.Magnitude());
	float threshold = 0.985f;
	Quaternion q = (keyFrames[p0].q * sin(angle - scaleFactor * angle) + keyFrames[pn].q * sin(scaleFactor * angle)) / sin(angle);
	// To handle stuttering case, check if angle between quaternions
	// is within a threshold, if so, use Lerp instead
	if (angle > threshold)
	{
		q = keyFrames[p0].q * (1 - scaleFactor) + keyFrames[pn].q * scaleFactor;
	}
	return q.QuaternionToMatrix();
}

// Exponential Lerp for scale
glm::mat4 Bone::InterpolateScale(float animationTime)
{
	int p0 = getKeyIndex(animationTime);
	int pn = p0 + 1;
	float scaleFactor = GetScaleFactor(static_cast<float>(keyTimes[p0]), static_cast<float>(keyTimes[pn]), animationTime);
	float scale = keyFrames[p0].s * pow((keyFrames[pn].s / keyFrames[p0].s), scaleFactor);
	return glm::scale(glm::mat4(1.0f), glm::vec3(scale));
}


