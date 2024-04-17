#include "Camera_Shake.hpp"
#include "Camera.hpp"
#include "Camera_Helper.hpp"

void Camera_Shake::update(float _deltaTime) {
    if(isShaking) {
        if(timeElapsed < duration) {
            float shake = amplitude * cos(2 * M_PI * frequency * timeElapsed);

            glm::vec3 currentPosition = camera.getPosition();
            glm::vec3 currentRotation = camera.getRotationDegrees();

            glm::vec3 shakePosition = currentPosition + glm::vec3(0,shake,0);
            glm::vec3 shakeRotation = currentRotation + glm::vec3(shake);

            camera.setPosition(shakePosition);
            camera.setRotationDegrees(shakeRotation);

            timeElapsed += _deltaTime;
        }
        else {
            camera.setPosition(originalPosition);
            camera.setRotationDegrees(originalRotation);
            isShaking = false;
        }
    }
}

void Camera_Shake::playShake() {
    if(!isShaking) {
        originalPosition = camera.getPosition();
        originalRotation = camera.getRotationDegrees();
        timeElapsed = 0;
        isShaking = true;
    }
}