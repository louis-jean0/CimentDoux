#pragma once
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <TP/Actor/ObjController.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Actor
{
public: 

	void init();
	void load(const char* _path, const glm::vec3& _color, GLuint _colorID);
	void update(float _deltaTime, GLFWwindow* _window, glm::quat _cameraRotation);
	void moveGround(glm::quat& _cameraRotation, GLFWwindow* _window, float _deltaTime);
	void jumpUpdate(GLFWwindow* _window, float& _deltaTime);
	void updateViewAndDraw(const Camera& _camera, GLuint _matrixID, GLuint _modelMatrixID);
	void destroy();

	bool		canMoveTarget() const {return m_moveTarget;}
	glm::vec3	getPosition() const {return m_position;}
private:
	bool m_showTarget{ false };
	bool m_moveTarget{ false };
	bool m_isJumping{ false };
	ObjController m_model;


	float m_translationSpeed{ 15.f };
	float m_rotationSpeed{ 30.f };

	glm::vec3 m_velocity;
	glm::vec3 m_position;
	glm::quat m_rotation;

	//dirty
	float m_gravity = 70.f; // Gravité
	float m_jumpForce = 30.0f; // Force du saut
	float m_groundHeight = 38.f; // Hauteur du sol
};