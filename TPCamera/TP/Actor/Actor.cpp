#include <TP/Actor/Actor.hpp>
#include <TP/Camera/Camera_Helper.hpp>
#include <common/quaternion_utils.hpp>

// Include GLM
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>


void Actor::init()
{
	m_position = glm::vec3(0.f, 0.f, 8.f);
	m_position.y = m_groundHeight;
	m_model.updateTrs(m_position, glm::mat4{ m_rotation });
}


void Actor::load(const char* _path, const glm::vec3& _color, GLuint _colorID)
{
    m_model.loadObj(_path, _color,  _colorID);
	init();
}


void Actor::update(float _deltaTime, GLFWwindow* _window, glm::quat _cameraRotation)
{
	/*if (ImGui::Begin("Interface"))
	{
		ImGui::Separator();
		ImGui::Checkbox("Show Target##target", &m_showTarget);
		ImGui::DragFloat("Translation Speed##target", &m_translationSpeed, 0.1f, 0.1f, 100.0f);
		ImGui::DragFloat("Rotation Speed##target", &m_rotationSpeed, 0.1f, 0.1f, 100.0f);
		ImGui::DragFloat("Gravity##target", &m_gravity, 0.1f, 0.1f, 100.0f);
		ImGui::DragFloat("Jump Force##target", &m_jumpForce, 0.1f, 0.1f, 100.0f);
	}
	ImGui::End();*/

	if (m_moveTarget)
	{
		moveGround(_cameraRotation, _window, _deltaTime);
		jumpUpdate(_window, _deltaTime);
	}

	m_model.updateTrs(m_position, glm::mat4{ m_rotation });
}

void Actor::moveGround(glm::quat& _cameraRotation, GLFWwindow* _window, float _deltaTime)
{
	glm::vec3 cameraFrontNoUp = _cameraRotation * VEC_FRONT;
	cameraFrontNoUp.y = 0.f;
	cameraFrontNoUp = normalize(cameraFrontNoUp);

	glm::vec3 cameraRightNoUp = _cameraRotation * VEC_RIGHT;
	cameraRightNoUp.y = 0.f;
	cameraRightNoUp = normalize(cameraRightNoUp);

	glm::vec3 translation;
	if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		translation += _deltaTime * m_translationSpeed * cameraFrontNoUp;
	}
	if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		translation -= _deltaTime * m_translationSpeed * cameraFrontNoUp;
	}


	if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		translation += _deltaTime * m_translationSpeed * cameraRightNoUp;
	}
	if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		translation -= _deltaTime * m_translationSpeed * cameraRightNoUp;
	}
	m_position += translation;

	translation = normalize(translation);
	if (length(translation) > 0.f)
	{
		glm::quat toDirection = QuaternionUtils::RotationBetweenVectors(m_rotation * VEC_FRONT, translation);
		m_rotation = QuaternionUtils::RotateTowards(m_rotation, toDirection * m_rotation, m_rotationSpeed * _deltaTime);
	}
}

void Actor::jumpUpdate(GLFWwindow* _window, float& _deltaTime)
{
	if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (!m_isJumping)
		{
			m_velocity.y = m_jumpForce;
			m_isJumping = true;
		}
	}

	if (m_isJumping)
	{
		m_velocity.y -= m_gravity * _deltaTime;
	}
	else
	{
		m_velocity.y = 0.0f;
		m_position.y = m_groundHeight;
		m_isJumping = false;
	}

	// Appliquer la vélocité à la position
	m_position += m_velocity * _deltaTime;
	if (m_isJumping && m_position.y <= m_groundHeight)
	{
		m_isJumping = false;
		m_position.y = m_groundHeight;
	}
}


void Actor::destroy()
{
    m_model.deleteBuffer();
}

void Actor::updateViewAndDraw(const Camera& _camera, GLuint _matrixID, GLuint _modelMatrixID)
{
	if (m_showTarget)
	{
		m_model.updateViewAndDraw(_camera, _matrixID, _modelMatrixID);
	}
}

