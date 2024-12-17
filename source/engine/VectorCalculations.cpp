#include "pch.h"
#include "VectorCalculations.h"

glm::vec3 VectorCalculation::GetForwardVec(glm::vec3 rotation)
{
	auto forward = glm::vec3(cos(glm::radians(rotation.x)) * sin(glm::radians(rotation.y)),
		sin(glm::radians(-rotation.x)),
		cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y)));
	return glm::normalize(forward);
}

glm::vec3 VectorCalculation::GetRightVec(glm::vec3 rotation)
{

	auto right = glm::vec3(1.0f, 0.0f, 0.0f);

	glm::vec3 res = calculateRotationMatrix(rotation) * right;
	return glm::normalize(res);
}

glm::vec3 VectorCalculation::GetUpVec(glm::vec3 rotation)
{
	auto up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 res = calculateRotationMatrix(rotation) * up;
	return glm::normalize(res);
}

glm::mat3 VectorCalculation::calculateRotationMatrix(glm::vec3 rotation)
{
	glm::mat3 xMat = {
		{ 1, 0                            , 0                              },
		{ 0, cos(glm::radians(rotation.x)), sin(glm::radians(-rotation.x)) },
		{ 0, sin(glm::radians(rotation.x)), cos(glm::radians(rotation.x))  }
	};

	glm::mat3 yMat = {
		{ cos(glm::radians(-rotation.y)), 0, sin(glm::radians(rotation.y))  },
		{ 0                             , 1, 0                              },
		{ sin(glm::radians(-rotation.y)), 0, cos(glm::radians(-rotation.y)) }
	};

	glm::mat3 zMat = {
		{ cos(glm::radians(rotation.z)), sin(glm::radians(-rotation.z)), 0 },
		{ sin(glm::radians(rotation.z)), cos(glm::radians(rotation.z)) , 0 },
		{ 0                            , 0                             , 1 }
	};

	return (yMat * xMat) * zMat;
}