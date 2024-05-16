#include <Camera_Helper.hpp>

glm::vec3 Camera_Helper::quatToEuler(glm::quat _quat)
{
	// Opengl quat to euler function give yaw betweel -90 and 90
	// If you want correct pitch and yaw you can use this 
	// Src http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm
	// But roll will be bewteen -90 and 90 here

	glm::vec3 eulerAngles;

	float test = _quat.x * _quat.y + _quat.z * _quat.w;

	if (test > 0.499f) { // singularity at north pole
		eulerAngles.y = 2.f * atan2(_quat.x, _quat.w);
		eulerAngles.z = M_PI / 2.f;
		eulerAngles.x = 0.f;
		return eulerAngles;
	}
	if (test < -0.499f) { // singularity at south pole
		eulerAngles.y = -2.f * atan2(_quat.x, _quat.w);
		eulerAngles.z = -M_PI / 2.f;
		eulerAngles.x = 0.f;
		return eulerAngles;
	}
	float sqx = _quat.x * _quat.x;
	float sqy = _quat.y * _quat.y;
	float sqz = _quat.z * _quat.z;

	eulerAngles.y = atan2(2.f * _quat.y * _quat.w - 2.f * _quat.x * _quat.z, 1.f - 2.f * sqy - 2.f * sqz);
	eulerAngles.z = asin(2.f * test);
	eulerAngles.x = atan2(2.f * _quat.x * _quat.w - 2.f * _quat.y * _quat.z, 1.f - 2.f * sqx - 2.f * sqz);

	return eulerAngles;
}

void Camera_Helper::computeFinalView(glm::mat4& _outProjectionMatrix, glm::mat4& _outviewMatrix, glm::vec3& _position, glm::quat _rotation, float _fovDegree)
{
	// Projection matrix : FOV, 4:3 ratio, display range : 0.1 unit <-> 100 units
	_outProjectionMatrix = glm::perspective(glm::radians(_fovDegree), 16.0f / 9.0f, 0.1f, 10000.0f);

	const glm::vec3 front = normalize(_rotation* VEC_FRONT);
	const glm::vec3 up = normalize(_rotation * VEC_UP);

	// Camera matrix
	_outviewMatrix = glm::lookAt(_position, _position + front, up);
}

glm::vec3 Camera_Helper::projectVectorOnPlane(glm::vec3 &vector, glm::vec3 &planeNormal) {
	return cross(planeNormal, cross(vector, planeNormal));
}

// Here, the angle will be clipped to value, that means that if you are at 180 and you want to go further, you won't be able
// This is satisfying for the pitch since we don't want to be upside-down, but not for the yaw (you want to be able to do a full 360 turn)
void Camera_Helper::clampAngleToValue(float &angle, float value) {
	if(angle > value) {
		angle = value;
	}
	if(angle < -value) {
		angle = -value;
	}
}

// This is more appropriate for the yaw
void Camera_Helper::clipAngleToBounds(float &angle, float value) {
	if(angle > value) {
		angle -= 360;
	}
	if(angle < -value) {
		angle += 360;
	}
}