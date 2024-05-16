#include <memory>
#include <vector>
#include <SceneNode.hpp>
#include <RigidBody.hpp>
#include <Player.hpp>

class PhysicsEngine {
    public:
        int id_block=-1;
        bool ladder_collision=false;
        std::vector<std::shared_ptr<RigidBody>> entities;
        void update(float delta_time);
        void add_player(std::shared_ptr<Player> player);
        void add_entity(std::shared_ptr<SceneNode> node);
};