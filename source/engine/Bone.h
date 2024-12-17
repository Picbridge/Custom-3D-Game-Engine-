#pragma once
class Bone
{
public:
	std::string name;
	std::vector<int> weights;
	std::vector<VQS> keyFrames;
	std::vector<double> keyTimes;
	glm::vec3 pos;
	bool animated = false;
	glm::mat4 transform;

	std::vector<glm::vec3> positions;
	std::vector<Quaternion> rotations;
	std::vector<float> scales;
	int numKeys;

	Bone();
	void Update(float animationTime);
	int getKeyIndex(float animationTime);
	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
	glm::mat4 InterpolatePosition(float animationTime);
	glm::mat4 InterpolateRotation(float animationTime);
	glm::mat4 InterpolateScale(float animationTime);
};

