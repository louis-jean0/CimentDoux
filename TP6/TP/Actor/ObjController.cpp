#include <TP/Actor/ObjController.hpp>


void ObjController::loadObj(const char* _path, const glm::vec3& _color, GLuint _colorID)
{
    loadOBJ(_path, m_vertices, m_uvs, m_normals);

    glGenBuffers(1, &m_vertexbufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexbufferID);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &m_uvbufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_uvbufferID);
    glBufferData(GL_ARRAY_BUFFER, m_uvs.size() * sizeof(glm::vec2), &m_uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &m_normalbufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_normalbufferID);
    glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);

    m_color = _color;
    m_colorID = _colorID;

}


void ObjController::updateTrs(const glm::vec3& _position, const glm::mat4& _rotationMatrix)
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(), _position);
	glm::mat4 scalingMatrix = glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));
	m_modelMatrix = translationMatrix * _rotationMatrix * scalingMatrix;
}

void ObjController::updateViewAndDraw(const Camera& _camera, GLuint _matrixID, GLuint _modelMatrixID)
{
    updateView(_camera, _matrixID, _modelMatrixID);
    drawObj();
}

void ObjController::updateView(const Camera& _camera, GLuint _matrixID, GLuint _modelMatrixID)
{
    glm::mat4 projectionMatrix = _camera.getProjectionMatrix();
    glm::mat4 viewMatrix = _camera.getViewMatrix();
    glm::mat4 MVP = projectionMatrix * viewMatrix * m_modelMatrix;

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    glUniform3f(m_colorID, m_color.x, m_color.y, m_color.z);
    glUniformMatrix4fv(_matrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(_modelMatrixID, 1, GL_FALSE, &m_modelMatrix[0][0]);
}

void ObjController::drawObj()
{
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexbufferID);
    glVertexAttribPointer(
        0,                  // attribute
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_uvbufferID);
    glVertexAttribPointer(
        1,                                // attribute
        2,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_normalbufferID);
    glVertexAttribPointer(
        2,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
}

void ObjController::deleteBuffer()
{
    glDeleteBuffers(1, &m_vertexbufferID);
    glDeleteBuffers(1, &m_uvbufferID);
    glDeleteBuffers(1, &m_normalbufferID);
}