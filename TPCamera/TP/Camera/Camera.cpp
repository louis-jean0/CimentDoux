#include <TP/Camera/Camera.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glm/gtc/type_ptr.hpp>
#include <string>

void Camera::init()
{
	m_fovDegree = 45.0f;
	m_position = glm::vec3(0.f, 40.f, 0.f);
	m_translationSpeed = 1.0f;
	m_eulerAngle = glm::vec3(0.f, 0.f, 0.f);
	m_eulerAngleInDegrees = glm::vec3(0.f, 0.f, 0.f);
	m_rotation = glm::quat{};
	m_rotationSpeed = 0.01f;
	m_showImguiDemo = false;
}

void Camera::updateInterface(float _deltaTime)
{
	// ImGUI window creation
	if (ImGui::Begin("Camera interface"))
	{
		ImGui::Separator();
		ImGui::Text("Welcome to this TP about Cameras! Press escape to close the exe");
		ImGui::Text("Long live to the cameras");
		ImGui::Separator();
		ImGui::Checkbox("Show ImGui demo", &m_showImguiDemo);
		ImGui::Separator();
		ImGui::Text(("Camera mode : " + std::to_string(m_cameraMode)).c_str());
		if(ImGui::IsKeyPressed(ImGuiKey_Tab)) {
			m_cameraMode = (CameraMode)(m_cameraMode + 1);
			if(m_cameraMode >= MODE_COUNT) {
				m_cameraMode = FIRST_MODE;
			}
			init(); // Reset values when changing mode
		}
		ImGui::Text("Camera translation");
		ImGui::DragFloat3("Camera position (x,y,z)",glm::value_ptr(m_position),m_translationSpeed);
		ImGui::SliderFloat("Translation speed",&m_translationSpeed,0.01f,5.0f);
		ImGui::Separator();
		ImGui::Text("Camera rotation (in degrees)");
		ImGui::Separator();
		ImGui::Checkbox("Reverse x-axis", &m_xAxisReversed);
		ImGui::Checkbox("Reverse y-axis", &m_yAxisReversed);
		ImGui::Separator();
		ImGui::SliderFloat("Yaw",&m_eulerAngleInDegrees.y,-180.0f,180.0f);
		ImGui::SliderFloat("Pitch",&m_eulerAngleInDegrees.x,-90.0f,90.0f);
		ImGui::SliderFloat("Roll",&m_eulerAngleInDegrees.z,-90.0f,90.0f);
		ImGui::SliderFloat("Rotation speed",&m_rotationSpeed,0.01f,1.0f);
		ImGui::Separator();
		ImGui::SliderFloat("FOV",&m_fovDegree,30.0f,179.9f);
		ImGui::Separator();
		if(ImGui::Button("Reset values")) {
			init();
		}
	}
	ImGui::End();

	if (m_showImguiDemo)
	{
		ImGui::ShowDemoWindow();
	}

}

void Camera::updateFreeInput(float _deltaTime, GLFWwindow* _window)
{

	switch(m_cameraMode) {

		case FIRST_MODE:
			if(!m_showMouse) {
				double cursorXPos,cursorYPos;
				glfwGetCursorPos(_window, &cursorXPos, &cursorYPos);

				if(firstPass) {
					firstPass = false;
					lastCursorXPos = cursorXPos;
					lastCursorYPos = cursorYPos;
				}

				double xDiff = cursorXPos - lastCursorXPos;
				double yDiff = cursorYPos - lastCursorYPos;

				lastCursorXPos = cursorXPos;
				lastCursorYPos = cursorYPos;

				if(m_xAxisReversed) {
					m_eulerAngleInDegrees.y += xDiff * m_rotationSpeed; // Reversed yaw
				}
				else {
					m_eulerAngleInDegrees.y -= xDiff * m_rotationSpeed; // Yaw
				}

				if(m_yAxisReversed) {
					m_eulerAngleInDegrees.x -= yDiff * m_rotationSpeed; // Reversed pitch
				}
				else {
					m_eulerAngleInDegrees.x += yDiff * m_rotationSpeed; // Pitch
				}
				
				glm::vec3 CFront = getCFront();
				glm::vec3 CRight = getCRight();

				if(glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS) {
					m_position += CFront * m_translationSpeed;
				}
				if(glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS) {
					m_position -= CFront * m_translationSpeed;
				}
				if(glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS) {
					m_position += CRight * m_translationSpeed;
				}
				if(glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS) {
					m_position -= CRight * m_translationSpeed;
				}
			}
		break;

		case SECOND_MODE:
			{
				m_rotationSpeed = 1.0f; // Adapt rotation speed for this mode
				glm::vec3 CFront = getCFront();
				glm::vec3 CRight = getCRight();
				CFront.y = 0; // Here, since we don't want to follow CFront height, we define its y component to 0 (little trick so we don't have to project on a plane)
				CRight.y = 0;

				if(glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS) {
					m_position += CFront * m_translationSpeed;
				}
				if(glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS) {
					m_position -= CFront * m_translationSpeed;
				}
				if(glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS) {
					m_position += CRight * m_translationSpeed;
				}
				if(glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS) {
					m_position -= CRight * m_translationSpeed;
				}
				if(glfwGetKey(_window, GLFW_KEY_UP) == GLFW_PRESS) {
					if(m_yAxisReversed) {
						m_eulerAngleInDegrees.x += m_rotationSpeed;
					}
					else {
						m_eulerAngleInDegrees.x -= m_rotationSpeed;
					}
				}
				if(glfwGetKey(_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
					if(m_yAxisReversed) {
						m_eulerAngleInDegrees.x -= m_rotationSpeed;
					}
					else {
						m_eulerAngleInDegrees.x += m_rotationSpeed;
					}
				}
				if(glfwGetKey(_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
					if(m_xAxisReversed) {
						m_eulerAngleInDegrees.y -= m_rotationSpeed;
					}
					else {
						m_eulerAngleInDegrees.y += m_rotationSpeed;
					}
				}
				if(glfwGetKey(_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
					if(m_xAxisReversed) {
						m_eulerAngleInDegrees.y += m_rotationSpeed;
					}
					else {
						m_eulerAngleInDegrees.y -= m_rotationSpeed;
					}
				}
			}
		break;

		default:
		break;
	}

}

void Camera::update(float _deltaTime, GLFWwindow* _window)
{
	updateInterface(_deltaTime);
	updateFreeInput(_deltaTime, _window);
	Camera_Helper::clipAngleToValue(m_eulerAngleInDegrees.y,180); // Clip yaw to [-180;180]
	Camera_Helper::clipAngleToValue(m_eulerAngleInDegrees.x,90); // Clip pitch to [-90;90]
	m_eulerAngle = glm::radians(m_eulerAngleInDegrees);
	m_rotation = glm::quat{m_eulerAngle};
	Camera_Helper::computeFinalView(m_projectionMatrix, m_viewMatrix, m_position, m_rotation, m_fovDegree);
}

glm::vec3 Camera::getCFront() const {
	return m_rotation * VEC_FRONT;
}

glm::vec3 Camera::getCUp() const {
	return m_rotation * VEC_UP;
}

glm::vec3 Camera::getCRight() const {
	return m_rotation * VEC_RIGHT;
}