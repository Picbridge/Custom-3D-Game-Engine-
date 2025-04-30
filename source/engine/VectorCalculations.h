#pragma once
class VectorCalculation
{
public:

	//@brief returns the forward vector of a given rotation
	//@param rotation : The rotation of the object
	//@return glm::vec3 the forward vector of the object
	static glm::vec3 GetForwardVec(const glm::vec3& rotation);

	//@brief returns the right vector of a given rotation
	//@param rotation : The rotation of the object
	//@return glm::vec3 the right vector of the object
	static glm::vec3 GetRightVec(const glm::vec3& rotation);

	//@brief returns the up vector of a given rotation
	//@param rotation : The rotation of the object
	//@return glm::vec3 the up vector of the object
	static glm::vec3 GetUpVec(const glm::vec3& rotation);

	static glm::vec3 GetForwardVecFromView	(const glm::mat4& viewMatrix);
	static glm::vec3 GetRightVecFromView	(const glm::mat4& viewMatrix);
	static glm::vec3 GetUpVecFromView		(const glm::mat4& viewMatrix);
	static glm::dquat CalculateRotationQuat (const glm::dvec3& rotation);

private:
	//@brief calculates the rotation matrix
	//@param rotation : The rotation of the object
	//@return glm::mat3 the rotation matrix
	static glm::mat3 calculateRotationMatrix(const glm::vec3& rotation);
};