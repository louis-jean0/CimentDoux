#include <vector>
#include <RigidBody.hpp>

class PhysicsEngine {
    public:
        std::vector<RigidBody*> entities;

        void update(float delta_time);
        void add_entity(RigidBody* entity);
};