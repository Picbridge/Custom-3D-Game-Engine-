#include "pch.h"
#include "VectorCalculations.h"

glm::vec3 VectorCalculation::GetForwardVec(const glm::vec3& rot)
{
    return glm::normalize(calculateRotationMatrix(rot) * glm::vec3(0, 0, -1));
}

glm::vec3 VectorCalculation::GetRightVec(const glm::vec3& rot)
{
    return glm::normalize(calculateRotationMatrix(rot) * glm::vec3(1, 0, 0));
}

glm::vec3 VectorCalculation::GetUpVec(const glm::vec3& rot)
{
    return glm::normalize(calculateRotationMatrix(rot) * glm::vec3(0, 1, 0));
}

glm::vec3 VectorCalculation::GetForwardVecFromView(const glm::mat4& viewMatrix)
{
    return -glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]);
}

glm::vec3 VectorCalculation::GetRightVecFromView(const glm::mat4& viewMatrix)
{
    return glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
}

glm::vec3 VectorCalculation::GetUpVecFromView(const glm::mat4& viewMatrix)
{
    return glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
}

glm::mat3 VectorCalculation::calculateRotationMatrix(const glm::vec3& rotation)
{
    glm::vec3 radRot = glm::radians(rotation);

    // Corrected order: Yaw (Y) -> Pitch (X) -> Roll (Z)
    glm::mat3 yawMatrix   = glm::mat3(glm::rotate(glm::mat4(1.0f), radRot.y, glm::vec3(0, 1, 0)));  // Yaw (Y)
    glm::mat3 pitchMatrix = glm::mat3(glm::rotate(glm::mat4(1.0f), radRot.x, glm::vec3(1, 0, 0)));  // Pitch (X)
    glm::mat3 rollMatrix  = glm::mat3(glm::rotate(glm::mat4(1.0f), radRot.z, glm::vec3(0, 0, 1)));  // Roll (Z)

    // Apply rotations in the correct order: Yaw -> Pitch -> Roll
    return  pitchMatrix * yawMatrix * rollMatrix;
}

glm::dquat VectorCalculation::CalculateRotationQuat(const glm::dvec3& rotation)
{
    glm::dvec3 radRotation = glm::radians(rotation);
    glm::dquat yawQuat   = glm::angleAxis(radRotation.y, glm::dvec3(0.0f, 1.0f, 0.0f));
    glm::dquat pitchQuat = glm::angleAxis(radRotation.x, glm::dvec3(1.0f, 0.0f, 0.0f));
    glm::dquat rollQuat  = glm::angleAxis(radRotation.z, glm::dvec3(0.0f, 0.0f, 1.0f));
    glm::dquat rotationQuat = yawQuat * pitchQuat * rollQuat;
    return rotationQuat;
}