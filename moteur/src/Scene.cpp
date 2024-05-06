#include <Scene.hpp>
#include <glm/gtx/string_cast.hpp>

void Scene::setup_scene() {
    // Shader
    auto shader = shaders->getShader();

    // Map
    auto map = Model::create("../data/models/map2/map2.obj", shader);
    add_meshes_from_model(map);
    
    // Directional light
    glm::vec3 ambient = glm::vec3(0.7f,0.7f,0.7f);
    glm::vec3 diffuse = glm::vec3(0.5f,0.5f,0.5f);
    glm::vec3 specular = glm::vec3(0.2f,0.2f,0.2f);
    glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    auto directionalLight = DirectionalLight::create(ambient, diffuse, specular, direction);
    lights->add_light(directionalLight);

    //std::cout<<scene_nodes.size()<<std::endl;
    glm::vec3 position = glm::vec3(-12.0f,3.2f,8.0f);
    glm::vec3 ambient2 = glm::vec3(0.3f,0.3f,0.3f);
    glm::vec3 diffuse2 = glm::vec3(0.1f,0.1f,0.1f);
    glm::vec3 specular2 = glm::vec3(0.1f,0.1f,0.1f);
    auto pointLight = PointLight::create(ambient2, diffuse2, specular2, position, 1.0f, 0.09f, 0.032f);
    //lights->add_light(pointLight);

    // Point lights
    for(auto& scene_node : scene_nodes) {
        auto& mesh = scene_node->mesh;
        if(mesh->material->emissive != glm::vec3(0.0f,0.0f,0.0f)) {
            //std::cout<<glm::to_string(scene_node->get_position())<<std::endl;
        }
    }
}

void Scene::add_node(std::shared_ptr<SceneNode> node) {
    scene_nodes.push_back(node);
}

void Scene::add_model(std::shared_ptr<Model> model) {
    auto node = SceneNode::create(model);
    scene_nodes.push_back(node);
}

void Scene::add_meshes_from_model(std::shared_ptr<Model> model) {
    auto nodes = SceneNode::create_node_meshes_from_model(model);
    for(auto& node : nodes) {
        node->set_scale(glm::vec3(10.0f));
        scene_nodes.push_back(node);
    }
}

void Scene::add_entities_into_physics_engine(std::shared_ptr<PhysicsEngine> pe) {
    for(auto& scene_node : scene_nodes) {
        pe->add_entity(scene_node);
    }
}

void Scene::draw(glm::mat4& view, glm::mat4& projection) {
    for(auto& scene_node : scene_nodes) {
        scene_node->draw(view, projection);
    }
    lights->setup_lights(shaders->getShader());
}