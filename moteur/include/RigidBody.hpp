#include <glm/glm.hpp>
#include <SceneNode.hpp>

class RigidBody {
    public:
        SceneNode* node;
        glm::vec3 velocity;
        float mass;

        RigidBody(SceneNode* node) : node(node), velocity(0.0f), mass(1.0f) {}

        void updatePhysics(float delta_time);
        bool checkCollision(RigidBody* other, float& collisionDepth, glm::vec3& collisionNormal);
        void solveCollision(RigidBody* other, float& collisionDepth, glm::vec3& collisionNormal);
};