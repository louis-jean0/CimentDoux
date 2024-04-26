#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <Shader.hpp>

class AABB {
    public:
        AABB();
        AABB(const glm::vec3 vmin, const glm::vec3 vmax);

        void processAABB(const glm::vec3 min, const glm::vec3 max);

        static AABB combine(const AABB& a, const AABB& b);

        void drawBox();

    private:
        glm::vec3 min, max;
};