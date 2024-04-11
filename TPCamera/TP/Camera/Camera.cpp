#include <TP/Camera/Camera.hpp>
#include <TP/Camera/Camera_Helper.hpp>

// Include GLM
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glm/gtc/type_ptr.hpp>

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
		ImGui::Text("Camera translation");
		ImGui::DragFloat3("Camera position (x,y,z)",glm::value_ptr(m_position),m_translationSpeed);
		ImGui::SliderFloat("Translation speed",&m_translationSpeed,0.01f,5.0f);
		ImGui::Separator();
		ImGui::Text("Camera rotation (in degrees)");
		ImGui::SliderFloat("Yaw",&m_eulerAngleInDegrees.y,-180.0f,180.0f);
		ImGui::SliderFloat("Pitch",&m_eulerAngleInDegrees.x,-180.0f,180.0f);
		ImGui::SliderFloat("Roll",&m_eulerAngleInDegrees.z,-180.0f,180.0f);
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

}


void Camera::update(float _deltaTime, GLFWwindow* _window)
{
	updateInterface(_deltaTime);
	updateFreeInput(_deltaTime, _window);
	m_eulerAngle = glm::radians(m_eulerAngleInDegrees);
	m_rotation = glm::quat{m_eulerAngle};
	Camera_Helper::computeFinalView(m_projectionMatrix, m_viewMatrix, m_position, m_rotation, m_fovDegree);
}