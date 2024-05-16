#pragma once

#include <glm/glm.hpp>

class Transform {
    public:
        bool transform_updated = false;
        Transform();
        Transform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);

        // Public methods
        glm::vec3 get_translation() const {return this->translation;}
        glm::vec3 get_rotation() const {return this->rotation;}
        glm::vec3 get_scale() const {return this->scale;}
        void set_translation(glm::vec3 translation);
        void set_rotation(glm::vec3 rotation);
        void set_scale(glm::vec3 scale);
        void adjust_translation(glm::vec3 delta);
        void adjust_scale(glm::vec3 delta);
        void adjust_rotation(glm::vec3 delta);
        glm::mat4 get_matrix();
        
    private:
        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scale;
};