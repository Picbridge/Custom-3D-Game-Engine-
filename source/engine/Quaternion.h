#pragma once
class Quaternion
{
public:
	float s;
	glm::vec3 v;

	// Initialization
	Quaternion(float _s, glm::vec3 _v) : s(_s), v(_v) {};
	Quaternion(glm::vec4 _sv);
	~Quaternion() {};

	// Operations
	Quaternion operator+(Quaternion const& other);
	Quaternion operator*(Quaternion const& other);
	Quaternion operator*(float const& f);
	Quaternion operator/(float const& f);

	// Member Functions
	float Magnitude() const;
	Quaternion Conjugate();
	Quaternion Inverse();
	glm::mat4 QuaternionToMatrix();
	
	// Static Functions
	static Quaternion Identity();
	static float Dot(Quaternion const& q1, Quaternion const& q2);
	static glm::vec3 Rotation(Quaternion &q, glm::vec3 r);
};

