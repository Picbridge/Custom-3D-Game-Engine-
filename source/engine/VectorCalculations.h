#pragma once
class VectorCalculation
{
public:

	//@brief returns the forward vector of a given rotation
	//@param rotation : The rotation of the object
	//@return glm::vec3 the forward vector of the object
	static glm::vec3 GetForwardVec(glm::vec3 rotation);

	//@brief returns the right vector of a given rotation
	//@param rotation : The rotation of the object
	//@return glm::vec3 the right vector of the object
	static glm::vec3 GetRightVec(glm::vec3 rotation);

	//@brief returns the up vector of a given rotation
	//@param rotation : The rotation of the object
	//@return glm::vec3 the up vector of the object
	static glm::vec3 GetUpVec(glm::vec3 rotation);

private:
	//@brief calculates the rotation matrix
	//@param rotation : The rotation of the object
	//@return glm::mat3 the rotation matrix
	static glm::mat3 calculateRotationMatrix(glm::vec3 rotation);
};