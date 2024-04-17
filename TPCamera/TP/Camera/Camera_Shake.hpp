#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

class Camera;

class Camera_Shake {
    public:
        Camera &camera;
        glm::vec3 originalPosition;
        glm::vec3 originalRotation;
        float duration;
        float amplitude;
        float frequency;
        float timeElapsed;
        bool isShaking;

        Camera_Shake(Camera &camera, float duration, float amplitude, float frequency) :
        camera(camera), duration(duration), amplitude(amplitude), frequency(frequency), isShaking(false), timeElapsed(0) {}

        void update(float _deltaTime);
        void playShake();
};
        