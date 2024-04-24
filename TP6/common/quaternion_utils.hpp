#pragma once
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>



class QuaternionUtils
{
public:
	static glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);

	static glm::quat LookAt(glm::vec3 direction, glm::vec3 desiredUp);

	static glm::quat RotateTowards(glm::quat q1, glm::quat q2, float maxAngle);

};