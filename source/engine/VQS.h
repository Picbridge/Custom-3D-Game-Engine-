#pragma once
class VQS
{
public:
	glm::vec3 v;
	Quaternion q;
	float s;

	VQS(glm::vec3 _v, Quaternion _q, float _s) : v(_v), q(_q), s(_s) {};

	VQS operator+(VQS const& other);
	VQS operator*(float c);
	VQS operator*(VQS const& other);
	glm::vec3 operator*(glm::vec3 r);

	VQS Inverse();

	static VQS Identity();
};

