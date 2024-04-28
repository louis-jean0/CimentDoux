#pragma once

#include "Camera_Helper.hpp"


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

enum InterpolationMode {
	LINEAR,
	SMOOTHSTEP,
	SMOOTHSTEP2,
	SMOOTHSTEP3,
	SMOOTHERSTEP,
	SQUARED,
	INV_SQUARED,
	CUBED,
	INV_CUBED,
	SIN
};

class Camera
{
public: 

	//gestion plusieurs cam
	int mode_cam=0;
	glm::vec3 pos_player;

	void init();
	void reset();
	void update(float _deltaTime, GLFWwindow* _window);
	void updateInterface(float _deltaTime);
	void updateFreeInput(float _deltaTime, GLFWwindow* _window);
	void computeFinalView();

	glm::quat getRotation() const {return m_rotation;}
	glm::mat4 getViewMatrix() const {return m_viewMatrix;}
	glm::mat4 getProjectionMatrix() const {return m_projectionMatrix;}
	glm::vec3 getPosition() const {return m_position;}
	glm::vec3 getRotationDegrees() const {return m_eulerAngleInDegrees;}
	bool getShowMouse() const {return m_showMouse;}
	glm::vec3 getCFront() const;
	glm::vec3 getCUp() const;
	glm::vec3 getCRight() const;
	void setPosition(glm::vec3 position) {this->m_position = position;}
	void setRotationDegrees(glm::vec3 degrees) {this->m_eulerAngleInDegrees = degrees;}
	void setShowMouse(bool m_showMouse) {this->m_showMouse = m_showMouse;} // For callback (in TP.cpp) purpose
	void transition(float delta_time);


private:

	//Camera parameters 
	float		m_fovDegree{ 45.0f };
	glm::vec3	m_position{ glm::vec3(10.f, 40.f, 10.f) };
	glm::vec3	m_eulerAngle{ glm::vec3(0.f, 0.f, 0.f) };
	glm::vec3	m_eulerAngleInDegrees{glm::vec3(0.0f, 0.0f, 0.0f)};
	glm::quat	m_rotation{};
	float 		m_translationSpeed{1.0f};
	float 		m_rotationSpeed{0.01f};
	bool 		m_xAxisReversed{false};
	bool 		m_yAxisReversed{false};

	// Transition
	InterpolationMode m_interpolationMode{SMOOTHSTEP};
	bool		m_isTransitioning{false};
	int 		m_transitionDuration{3};
	float 		m_transitionProgress{0.0f};
	glm::vec3 	m_transitionStartPosition{m_position};
	glm::vec3	m_transitionStopPosition{m_position + glm::normalize(getCFront()) * 30.0f};

	//Interface option
	bool m_showImguiDemo{ false };
	bool m_showHelp{false};
	CameraMode	m_cameraMode{FIRST_MODE};
	bool m_showMouse = true;



	// Window handling (I'm not sure it's a good practice to do this here, but for this TP it will do just fine)
	double lastCursorXPos,lastCursorYPos;
	bool firstPass = true;

	//View
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

};