#include <GLFW/glfw3.h>
#include <Camera.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/gtc/type_ptr.hpp>
#include <string>

void Camera::init()
{
	m_fovDegree = 45.0f;
	m_position = glm::vec3(0.f, 2.f, 0.f);
	m_translationSpeed = 5.0f;
	m_eulerAngle = glm::vec3(0.f, 0.f, 0.f);
	m_eulerAngleInDegrees = glm::vec3(0.f, 0.f, 0.f);
	m_rotation = glm::quat{};
	m_rotationSpeed = 5.0f;
	m_showImguiDemo = false;
	m_showHelp = false;
	m_transitionDuration = 1;
	m_interpolationMode = LINEAR;
}

void Camera::reset()
{
	m_fovDegree = 45.0f;
	m_position = pos_player + glm::vec3(0.f, 1.f, -10.f);
	m_translationSpeed = 5.0f;
	m_eulerAngle = glm::vec3(0.f, 0.f, 0.f);
	m_eulerAngleInDegrees = glm::vec3(0.f, 0.f, 0.f);
	m_rotation = glm::quat{};
	m_rotationSpeed = 5.0f;
	m_showImguiDemo = false;
	m_showHelp = false;
	m_transitionDuration = 1;
	m_interpolationMode = LINEAR;
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
		//ImGui::Checkbox("Show ImGui demo", &m_showImguiDemo);
		// ImGui::SameLine();
		ImGui::Checkbox("Show help", &m_showHelp);
		ImGui::Separator();
		ImGui::Text(("Camera mode : " + std::to_string(m_cameraMode)).c_str());
		if(ImGui::IsKeyPressed(ImGuiKey_Tab)) {
			m_cameraMode = (CameraMode)(m_cameraMode + 1);
			if(m_cameraMode >= MODE_COUNT) {
				m_cameraMode = FIRST_MODE;
			}
			init(); // Reset values when changing mode
		}
		ImGui::Separator();

		ImGui::Text("Camera FOV");
		ImGui::SliderFloat("FOV",&m_fovDegree,30.0f,179.9f);
		ImGui::Separator();

		ImGui::Text("Camera translation");
		ImGui::DragFloat3("Camera position (x,y,z)",glm::value_ptr(m_position),m_translationSpeed);
		ImGui::SliderFloat("Translation speed",&m_translationSpeed,5.0f,50.0f);
		ImGui::Separator();

		ImGui::Text("Camera rotation (in degrees)");
		ImGui::Checkbox("Reverse x-axis", &m_xAxisReversed);
		ImGui::Checkbox("Reverse y-axis", &m_yAxisReversed);
		ImGui::SliderFloat("Yaw",&m_eulerAngleInDegrees.y,-180.0f,180.0f);
		ImGui::SliderFloat("Pitch",&m_eulerAngleInDegrees.x,-90.0f,90.0f);
		ImGui::SliderFloat("Roll",&m_eulerAngleInDegrees.z,-90.0f,90.0f);
		ImGui::SliderFloat("Rotation speed",&m_rotationSpeed,1.0f,50.0f);
		ImGui::Separator();

		ImGui::Text("Camera transition");
		ImGui::SliderInt("Transition duration",&m_transitionDuration,1,10);
		const char* transitions[] = {"LINEAR","SMOOTHSTEP","SMOOTHSTEP2","SMOOTHSTEP3","SMOOTHERSTEP","SQUARED","INV_SQUARED","CUBED","INV_CUBED","SIN"};
		int currentTransition = (int)m_interpolationMode;
		if(ImGui::BeginCombo("Transition mode", transitions[currentTransition])) {
			for(int n = 0; n < IM_ARRAYSIZE(transitions); n++) {
				bool isSelected = (currentTransition == n);
				if(ImGui::Selectable(transitions[n], isSelected)) {
					currentTransition = n;
					m_interpolationMode = (InterpolationMode)currentTransition;
				}
				if(isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		if(ImGui::Button("Play transition")) {
			m_transitionProgress = 0.0f;
			m_transitionStartPosition = m_position;
			m_transitionStopPosition = m_position + glm::normalize(getCFront()) * 30.0f;
			m_isTransitioning = true;
		}
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

	if(m_showHelp) {
		ImGui::Begin("Camera help", &m_showHelp);
		ImGui::Text("To toggle mouse, press T");
		ImGui::Text("To change camera mode, press TAB");
		ImGui::Text("In first mode, you can move the camera by using W,A,S,D and rotate it with the mouse (mouse needs to be toggled off)");
		ImGui::Text("In second mode, you can move the camera using W,A,S,D and rotate it with directional keys");
		ImGui::Text("To run a camera transition, click on the 'Play transition' button");
		ImGui::Text("Click on the 'Reset values' button to reset all the values of the program without restarting it");
		ImGui::End();
	}

}

void Camera::updateFreeInput(float _deltaTime, GLFWwindow* _window)
{

	if(!m_isTransitioning) {

		static bool lastShowMouseState = m_showMouse;

		if(m_showMouse && !lastShowMouseState) {
			double cursorXPos, cursorYPos;
			glfwGetCursorPos(_window, &cursorXPos, &cursorYPos);
			lastCursorXPos = cursorXPos;
			lastCursorYPos = cursorYPos;
			firstPass = true;
		}

		lastShowMouseState = m_showMouse;

		if(mode_cam==0) {
			m_position=pos_player+glm::vec3(0.,1.,1.);

		}
		if(mode_cam==1) {

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
					m_eulerAngleInDegrees.y += xDiff * m_rotationSpeed * _deltaTime; // Reversed yaw
				}
				else {
					m_eulerAngleInDegrees.y -= xDiff * m_rotationSpeed * _deltaTime; // Yaw
				}

				if(m_yAxisReversed) {
					m_eulerAngleInDegrees.x -= yDiff * m_rotationSpeed * _deltaTime; // Reversed pitch
				}
				else {
					m_eulerAngleInDegrees.x += yDiff * m_rotationSpeed * _deltaTime; // Pitch
				}
				
				// glm::vec3 CFront = glm::normalize(getCFront());
				// glm::vec3 CRight = glm::normalize(getCRight());

				// if(glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS) {
				// 	m_position += CFront * m_translationSpeed * _deltaTime;
				// }
				// if(glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS) {
				// 	m_position -= CFront * m_translationSpeed * _deltaTime;
				// }
				// if(glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS) {
				// 	m_position += CRight * m_translationSpeed * _deltaTime;
				// }
				// if(glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS) {
				// 	m_position -= CRight * m_translationSpeed * _deltaTime;
				// }
				// if(glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS) {
				// 	m_position += glm::vec3(0.,1.,0.) * m_translationSpeed * _deltaTime;
				// }
				// if(glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS) {
				// 	m_position -= glm::vec3(0.,1.,0.) * m_translationSpeed * _deltaTime;
				// }
			}
		}
		if(mode_cam==2){
				glm::vec3 CFront = getCFront();
				glm::vec3 CRight = getCRight();
				CFront.y = 0; // Here, since we don't want to follow CFront height, we define its y component to 0 (little trick so we don't have to project on a plane)
				CRight.y = 0;

				if(glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS) {
					m_position += CFront * m_translationSpeed * _deltaTime;
				}
				if(glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS) {
					m_position -= CFront * m_translationSpeed * _deltaTime;
				}
				if(glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS) {
					m_position += CRight * m_translationSpeed * _deltaTime;
				}
				if(glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS) {
					m_position -= CRight * m_translationSpeed * _deltaTime;
				}
				if(glfwGetKey(_window, GLFW_KEY_UP) == GLFW_PRESS) {
					if(m_yAxisReversed) {
						m_eulerAngleInDegrees.x += m_rotationSpeed * _deltaTime;
					}
					else {
						m_eulerAngleInDegrees.x -= m_rotationSpeed * _deltaTime;
					}
				}
				if(glfwGetKey(_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
					if(m_yAxisReversed) {
						m_eulerAngleInDegrees.x -= m_rotationSpeed * _deltaTime;
					}
					else {
						m_eulerAngleInDegrees.x += m_rotationSpeed * _deltaTime;
					}
				}
				if(glfwGetKey(_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
					if(m_xAxisReversed) {
						m_eulerAngleInDegrees.y -= m_rotationSpeed * _deltaTime;
					}
					else {
						m_eulerAngleInDegrees.y += m_rotationSpeed * _deltaTime;
					}
				}
				if(glfwGetKey(_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
					if(m_xAxisReversed) {
						m_eulerAngleInDegrees.y += m_rotationSpeed * _deltaTime;
					}
					else {
						m_eulerAngleInDegrees.y -= m_rotationSpeed * _deltaTime;
					}
				}
				if(glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS) {
					m_position.y += m_translationSpeed * _deltaTime;
				}
				if(glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS) {
					m_position.y -= m_translationSpeed * _deltaTime;
				}
		}

	}

}

void Camera::transition(float delta_time) {
	if(m_transitionProgress < m_transitionDuration) { // Correct because glfwGetTime() returns time in seconds
		m_transitionProgress += delta_time;
		float v = m_transitionProgress / m_transitionDuration;
	
		switch(m_interpolationMode) {
			case LINEAR:
				m_position = glm::mix(m_transitionStartPosition,m_transitionStopPosition,v);
			break;

			case SMOOTHSTEP:
				v = v * v * (3 - 2 * v);
				m_position = glm::mix(m_transitionStartPosition,m_transitionStopPosition,v);
			break;

			case SMOOTHSTEP2:
				v = v * v * (3 - 2 * v);
				v = v * v * (3 - 2 * v);
				m_position = glm::mix(m_transitionStartPosition,m_transitionStopPosition,v);
			break;

			case SMOOTHSTEP3:
				v = v * v * (3 - 2 * v);
				v = v * v * (3 - 2 * v);
				v = v * v * (3 - 2 * v);
				m_position = glm::mix(m_transitionStartPosition,m_transitionStopPosition,v);
			break;

			case SMOOTHERSTEP:
				v = v * v * v * (v * (v * 6 - 15) + 10);
				m_position = glm::mix(m_transitionStartPosition,m_transitionStopPosition,v);
			break;

			case SQUARED:
				v = v * v;
				m_position = glm::mix(m_transitionStartPosition,m_transitionStopPosition,v);
			break;

			case INV_SQUARED:
				v = 1 - (1 - v) * (1 - v);
				m_position = glm::mix(m_transitionStartPosition,m_transitionStopPosition,v);
			break;

			case CUBED:
				v = v * v * v;
				m_position = glm::mix(m_transitionStartPosition,m_transitionStopPosition,v);
			break;

			case INV_CUBED:
				v = 1 - (1 - v) * (1 - v) * (1 - v);
				m_position = glm::mix(m_transitionStartPosition,m_transitionStopPosition,v);
			break;

			case SIN:
				v = sin(v * M_PI / 2);
				m_position = glm::mix(m_transitionStartPosition,m_transitionStopPosition,v);
			break;

			default:
			break;
		}
	}
	else {
		m_isTransitioning = false;
	}
}

void Camera::update(float _deltaTime, GLFWwindow* _window)
{
	updateFreeInput(_deltaTime, _window);
	if(m_isTransitioning) {
		transition(_deltaTime);
	}
	Camera_Helper::clipAngleToBounds(m_eulerAngleInDegrees.y,180); // Clip yaw to [-180;180] (but still be able to do a 360 turn)
	Camera_Helper::clampAngleToValue(m_eulerAngleInDegrees.x,90); // Clamp pitch to [-90;90]
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
