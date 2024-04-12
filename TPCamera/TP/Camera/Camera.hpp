#pragma once

#include <TP/Camera/Camera_Helper.hpp>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

enum CameraMode {
	FIRST_MODE = 1,
	SECOND_MODE,
	MODE_COUNT
};

class Camera
{
public: 

	void init();
	void update(float _deltaTime, GLFWwindow* _window);
	void updateInterface(float _deltaTime);
	void updateFreeInput(float _deltaTime, GLFWwindow* _window);
	void computeFinalView();

	glm::quat getRotation() const {return m_rotation;}
	glm::mat4 getViewMatrix() const {return m_viewMatrix;}
	glm::mat4 getProjectionMatrix() const {return m_projectionMatrix;}
	bool getShowMouse() const {return m_showMouse;}
	glm::vec3 getCFront() const;
	glm::vec3 getCUp() const;
	glm::vec3 getCRight() const;
	void setShowMouse(bool m_showMouse) {this->m_showMouse = m_showMouse;} // For callback (in TP.cpp) purpose
	void interpolate(float delta_time);

private:

	//Camera parameters 
	float		m_fovDegree{ 45.0f };
	glm::vec3	m_position{ glm::vec3(0.f, 40.f, 0.f) };
	glm::vec3	m_eulerAngle{ glm::vec3(0.f, 0.f, 0.f) };
	glm::vec3	m_eulerAngleInDegrees{glm::vec3(0.0f, 0.0f, 0.0f)};
	glm::quat	m_rotation{};
	float 		m_translationSpeed{1.0f};
	float 		m_rotationSpeed{0.01f};
	bool 		m_xAxisReversed{false};
	bool 		m_yAxisReversed{false};

	// Interpolation (for transition)
	InterpolationMode m_interpolationMode{SMOOTHERSTEP};
	bool		m_isInterpolating{false};
	float 		m_interpolationDuration{5.0f};
	float 		m_interpolationProgress{0.0f};
	glm::vec3 	m_interpolationStartPosition{m_position};
	glm::vec3	m_interpolationStopPosition{(m_position.x, m_position.y, m_position.z + 20)};
	
	//Interface option
	bool m_showImguiDemo{ false };
	CameraMode	m_cameraMode{FIRST_MODE};
	bool m_showMouse = true;

	// Window handling (I'm not sure it's a good practice to do this here, but for this TP it will do just fine)
	double lastCursorXPos,lastCursorYPos;
	bool firstPass = true;

	//View
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;
};