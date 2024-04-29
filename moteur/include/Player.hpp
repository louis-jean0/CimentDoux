#include <SceneNode.hpp>
#include <Camera.hpp>

class Player {
    public:
        SceneNode* player_node;
        Camera camera;

        Player(SceneNode* player_node) : player_node(player_node), camera() {
            camera.init();
        }

    private:
        void update(float delta_time);

};