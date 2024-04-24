#pragma once
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <common/objloader.hpp>
#include <common/shader.hpp>
#include <TP/Camera/Camera.hpp>

class ObjController
{
public: 

	void loadObj(const char* _path, const glm::vec3& _color,GLuint _colorID);
	void updateTrs(const glm::vec3& _position, const glm::mat4& _rotationMatrix);
	void drawObj();
	void updateViewAndDraw(const Camera& _camera, GLuint _matrixID, GLuint _modelMatrixID);
	void updateView(const Camera& _camera, GLuint _matrixID, GLuint _modelMatrixID);
	void deleteBuffer();

	glm::vec3 m_color;
private:
	GLuint m_colorID;
	GLuint m_vertexbufferID;
	GLuint m_uvbufferID;
	GLuint m_normalbufferID;

	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec2> m_uvs;


	glm::mat4 m_modelMatrix;
};