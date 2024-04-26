#include <AABB.hpp>
#include <climits>

AABB::AABB() {}

AABB::AABB(const glm::vec3 vmin, const glm::vec3 vmax) {
    min = vmin;
    max = vmax;
}

void AABB::processAABB(const glm::vec3 min, const glm::vec3 max) {
    this->min = min;
    this->max = max;
}

AABB AABB::combine(const AABB& a, const AABB& b) {
    glm::vec3 newMin = glm::vec3(std::min(a.min[0], b.min[0]), std::min(a.min[1], b.min[1]), std::min(a.min[2], b.min[2]));
    glm::vec3 newMax = glm::vec3(std::max(a.max[0], b.max[0]), std::max(a.max[1], b.max[1]), std::max(a.max[2], b.max[2]));
    return AABB(newMin, newMax);
}

void AABB::drawBox() {
    GLfloat vertices[] = {
    // Bas de la boîte
    min.x, min.y, min.z,  max.x, min.y, min.z,
    max.x, min.y, min.z,  max.x, max.y, min.z,
    max.x, max.y, min.z,  min.x, max.y, min.z,
    min.x, max.y, min.z,  min.x, min.y, min.z,
    // Haut de la boîte
    min.x, min.y, max.z,  max.x, min.y, max.z,
    max.x, min.y, max.z,  max.x, max.y, max.z,
    max.x, max.y, max.z,  min.x, max.y, max.z,
    min.x, max.y, max.z,  min.x, min.y, max.z,
    // Côtés de la boîte
    min.x, min.y, min.z,  min.x, min.y, max.z,
    max.x, min.y, min.z,  max.x, min.y, max.z,
    min.x, max.y, min.z,  min.x, max.y, max.z,
    max.x, max.y, min.z,  max.x, max.y, max.z
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
    glBindVertexArray(0); // Unbind VAO

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 24); // 24 sommets au total
    glBindVertexArray(0);
};

// void AABB::drawBox() {
//             glm::vec3 corners[8];
//             corners[0] = glm::vec3(min[0], min[1], min[2]);
//             corners[1] = glm::vec3(max[0], min[1], min[2]);
//             corners[2] = glm::vec3(min[0], max[1], min[2]);
//             corners[3] = glm::vec3(max[0], max[1], min[2]);
//             corners[4] = glm::vec3(min[0], min[1], max[2]);
//             corners[5] = glm::vec3(max[0], min[1], max[2]);
//             corners[6] = glm::vec3(min[0], max[1], max[2]);
//             corners[7] = glm::vec3(max[0], max[1], max[2]);

            
//             glLineWidth(1.0f);
//             glBegin(GL_LINES);

//             // Bas de la boîte
//             glVertex3f(corners[0][0], corners[0][1], corners[0][2]);
//             glVertex3f(corners[1][0], corners[1][1], corners[1][2]);

//             glVertex3f(corners[1][0], corners[1][1], corners[1][2]);
//             glVertex3f(corners[3][0], corners[3][1], corners[3][2]);

//             glVertex3f(corners[3][0], corners[3][1], corners[3][2]);
//             glVertex3f(corners[2][0], corners[2][1], corners[2][2]);

//             glVertex3f(corners[2][0], corners[2][1], corners[2][2]);
//             glVertex3f(corners[0][0], corners[0][1], corners[0][2]);

//             // Haut de la boîte
//             glVertex3f(corners[4][0], corners[4][1], corners[4][2]);
//             glVertex3f(corners[5][0], corners[5][1], corners[5][2]);

//             glVertex3f(corners[5][0], corners[5][1], corners[5][2]);
//             glVertex3f(corners[7][0], corners[7][1], corners[7][2]);

//             glVertex3f(corners[7][0], corners[7][1], corners[7][2]);
//             glVertex3f(corners[6][0], corners[6][1], corners[6][2]);

//             glVertex3f(corners[6][0], corners[6][1], corners[6][2]);
//             glVertex3f(corners[4][0], corners[4][1], corners[4][2]);

//             // Côtés de la boîte
//             glVertex3f(corners[0][0], corners[0][1], corners[0][2]);
//             glVertex3f(corners[4][0], corners[4][1], corners[4][2]);

//             glVertex3f(corners[1][0], corners[1][1], corners[1][2]);
//             glVertex3f(corners[5][0], corners[5][1], corners[5][2]);

//             glVertex3f(corners[2][0], corners[2][1], corners[2][2]);
//             glVertex3f(corners[6][0], corners[6][1], corners[6][2]);

//             glVertex3f(corners[3][0], corners[3][1], corners[3][2]);
//             glVertex3f(corners[7][0], corners[7][1], corners[7][2]);

//             glEnd();
// }