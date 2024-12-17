#include "pch.h"

Quaternion::Quaternion(glm::vec4 _sv)
{
	s = _sv.x;
	v = glm::vec3(_sv.y, _sv.z, _sv.w);
}

Quaternion Quaternion::operator+(Quaternion const& other)
{
	return Quaternion(this->s + other.s, this->v + other.v);
}

Quaternion Quaternion::operator*(Quaternion const& other)
{
	return Quaternion(this->s * other.s - glm::dot(this->v, other.v), this->s * other.v + other.s * this->v + glm::cross(this->v, other.v));
}

Quaternion Quaternion::operator*(float const& f)
{
	return Quaternion(this->s * f, this->v * f);
}

Quaternion Quaternion::operator/(float const& f)
{
	return Quaternion(this->s / f, this->v / f);
}

float Quaternion::Magnitude() const
{
	return sqrt(this->s * this->s + this->v.x * this->v.x + this->v.y * this->v.y + this->v.z * this->v.z);
}

Quaternion Quaternion::Conjugate()
{
	return Quaternion(this->s, -this->v);
}

Quaternion Quaternion::Inverse()
{
	float temp = this->Magnitude();
	temp *= temp;
	return Quaternion(this->Conjugate() * (1 / temp));
}

Quaternion Quaternion::Identity()
{
	return Quaternion(1, glm::vec3(0));
}

float Quaternion::Dot(Quaternion const& q1, Quaternion const& q2)
{
	return q1.s * q2.s  + glm::dot(q1.v , q2.v);
}

glm::vec3 Quaternion::Rotation(Quaternion &q, glm::vec3 r)
{
	Quaternion p(0, r);
	return glm::vec3((q * p * q.Inverse()).v);
}

glm::mat4 Quaternion::QuaternionToMatrix()
{
	// Extract quaternion components
	float w = s;
	float x = v.x;
	float y = v.y;
	float z = v.z;

	// Create the 4x4 rotation matrix
	glm::mat4 rotationMatrix = glm::mat4(1.0f); // Identity matrix
	rotationMatrix[0][0] = 1 - 2 * (y * y + z * z);
	rotationMatrix[1][0] = 2 * (x * y - z * w);
	rotationMatrix[2][0] = 2 * (x * z + y * w);
	rotationMatrix[0][1] = 2 * (x * y + z * w);
	rotationMatrix[1][1] = 1 - 2 * (x * x + z * z);
	rotationMatrix[2][1] = 2 * (y * z - x * w);
	rotationMatrix[0][2] = 2 * (x * z - y * w);
	rotationMatrix[1][2] = 2 * (y * z + x * w);
	rotationMatrix[2][2] = 1 - 2 * (x * x + y * y);
	rotationMatrix[3][3] = 1.0f; // Homogeneous coordinate

	return rotationMatrix;
}