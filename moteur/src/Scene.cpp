#include <Scene.hpp>
#include <glm/gtx/string_cast.hpp>

void Scene::setup_scene() {
    // Shader
    auto shader = shaders->getShader();

    // Map
    auto map = Model::create("../data/models/mapsansnormale/map_sans_normales.obj", shader);
    add_meshes_from_model(map);
    
    // Directional light
    glm::vec3 ambient = glm::vec3(1.0f,1.0f,1.0f);
    glm::vec3 diffuse = glm::vec3(1.0f,1.0f,1.0f);
    glm::vec3 specular = glm::vec3(1.0f,1.0f,1.0f);
    glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    auto directionalLight = DirectionalLight::create(ambient, diffuse, specular, direction);
    //lights->add_light(directionalLight);

    //std::cout<<scene_nodes.size()<<std::endl;
        
    glm::vec3 reglage = glm::vec3(1.0f, 0.09f, 0.032f);   
    glm::vec3 ambient2 = glm::vec3(0.3f,0.3f,0.3f);
    glm::vec3 diffuse2 = glm::vec3(0.1f,0.1f,0.1f);
    glm::vec3 specular2 = glm::vec3(0.1f,0.1f,0.1f);

    //lumire 1
    glm::vec3 position = glm::vec3(-23.6f,6.5f,14.4f);
    auto pointLight = PointLight::create(ambient2, diffuse2, specular2, position, 0.10f, 0.001f, 0.152f);
    lights->add_light(pointLight);

    //spot2
    glm::vec3 position2 = glm::vec3(-12.0f,0.5f,0.8f);
    glm::vec3 directionTest = glm::vec3(1.0f,0.5f,0.0f);
    auto pointLight2 = TorchLight::create(ambient2, diffuse2, specular2, position2, 0.10f, 0.001f, 0.152f, directionTest, 45.0f, 90.0f);
    lights->add_light(pointLight2);

    //lumiere 3
    glm::vec3 position3 = glm::vec3(30.0f,16.f,15.3f);
    auto pointLight3 = PointLight::create(ambient2, diffuse2, specular2, position3, 0.10f, 0.001f, 0.152f);
    lights->add_light(pointLight3);

    //lumiere 2
    glm::vec3 position4 = glm::vec3(0.2f,9.f,28.3f);
    auto pointLight4 = PointLight::create(ambient2, diffuse2, specular2, position4, 0.10f, 0.001f, 0.152f);
    lights->add_light(pointLight4);

    //spot1 bétoniere
    glm::vec3 position5 = glm::vec3(-27.7f,0.5f,24.f);
    auto pointLight5 = PointLight::create(ambient2, diffuse2, specular2, position5, 0.10f, 0.001f, 0.152f);
    lights->add_light(pointLight5);

    //neon1
    glm::vec3 position6 = glm::vec3(-10.5f,6.5f,0.1f);
    auto pointLight6 = PointLight::create(ambient2, diffuse2, specular2, position6, 0.10f, 0.001f, 0.352f);
    lights->add_light(pointLight6);
    glm::vec3 position7 = glm::vec3(-9.5f,7.f,0.1f);
    auto pointLight7 = PointLight::create(ambient2, diffuse2, specular2, position7, 0.10f, 0.001f, 0.352f);
    lights->add_light(pointLight7);

    //neon2
    glm::vec3 position8 = glm::vec3(-10.5f,6.5f,-4.f);
    auto pointLight8 = PointLight::create(ambient2, diffuse2, specular2, position8, 0.10f, 0.001f, 0.352f);
    lights->add_light(pointLight8);
    glm::vec3 position9 = glm::vec3(-9.5f,7.f,-4.f);
    auto pointLight9 = PointLight::create(ambient2, diffuse2, specular2, position9, 0.10f, 0.001f, 0.352f);
    lights->add_light(pointLight9);

    //lumière 4 
    glm::vec3 position10 = glm::vec3(8.9f,8.f,10.5f);
    auto pointLight10 = PointLight::create(ambient2, diffuse2, specular2, position10, 0.10f, 0.001f, 0.152f);
    lights->add_light(pointLight10);

    //lumière 5 
    glm::vec3 position11 = glm::vec3(-28.f,20.f,-11.f);
    auto pointLight11 = PointLight::create(ambient2, diffuse2, specular2, position11, 0.10f, 0.001f, 0.152f);
    lights->add_light(pointLight11);

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
    //int i = 100;
    for(auto& node : nodes) {
        node->set_scale(glm::vec3(2.0f));
        scene_nodes.push_back(node);
        // if(i == 100) {
        //     std::cout<<"Caca"<<std::endl;
        //     node->mesh->material->emissive = glm::vec3(1.0f,0.0f,0.0f);
        //     node->rigid_body->friction_coefficient = 1.0f;
        // } 
        // i++;
    }
}

void Scene::add_entities_into_physics_engine(std::shared_ptr<PhysicsEngine> pe) {
    for(auto& scene_node : scene_nodes) {
        if(scene_node->mesh->material->diffuse != glm::vec3(0.074825,0.454437,0.037416)) {
            pe->add_entity(scene_node);
        }
    }
}

void Scene::draw(glm::mat4& view, glm::mat4& projection) {
    for(auto& scene_node : scene_nodes) {
        scene_node->draw(view, projection);
    }
    lights->setup_lights(shaders->getShader());
}