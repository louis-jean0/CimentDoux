#include <Scene.hpp>
#include <ShadowMap.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>

void Scene::setup_scene() {
    // Shader
    auto shader = shaders->getShader();

    // Map
    auto map = Model::create("../data/models/map/Ciment_doux_level_v2.obj", shader);
    //auto map = Model::create("../data/models/mapsansnormale/map_sans_normales.obj", shader);
    add_meshes_from_model(map);


    // Directional light
    glm::vec3 ambient = glm::vec3(0.001f,0.001f,0.001f);
    glm::vec3 diffuse = glm::vec3(0.5f,0.5f,0.5f);
    glm::vec3 specular = glm::vec3(0.1f,0.1f,0.1f);
    glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    auto directionalLight = DirectionalLight::create(ambient, diffuse, specular, direction);

    //std::cout<<scene_nodes.size()<<std::endl;
    
    lights->add_directional_light(directionalLight);

    glm::vec3 reglage = glm::vec3(1.0f, 0.09f, 0.032f);   
    glm::vec3 ambient2 = glm::vec3(0.1f,0.1f,0.1f);
    glm::vec3 diffuse2 = glm::vec3(1.0f,1.0f,1.0f);
    glm::vec3 specular2 = glm::vec3(1.0f,1.0f,1.0f);

    // neon----------------------------------------------------
    // neon1
    glm::vec3 reglage_neon = glm::vec3(0.50f, 0.001f, 0.352f); 
    glm::vec3 position6 = glm::vec3(-10.5f,6.5f,0.1f);
    auto pointLight6 = PointLight::create(ambient2, diffuse2, specular2, position6, reglage_neon.x, reglage_neon.y, reglage_neon.z);
    lights->add_point_light(pointLight6);

    //neon2
    glm::vec3 position8 = glm::vec3(-10.5f,6.5f,-4.f);
    auto pointLight8 = PointLight::create(ambient2, diffuse2, specular2, position8, reglage_neon.x, reglage_neon.y, reglage_neon.z);
    lights->add_point_light(pointLight8);

    //neon3
    glm::vec3 position14 = glm::vec3(-28.7f,49.5f,-0.13f);
    auto pointLight14 = PointLight::create(ambient2, diffuse2, specular2, position14, reglage_neon.x, reglage_neon.y, reglage_neon.z);
    lights->add_point_light(pointLight14);

    //neon4
    glm::vec3 position15 = glm::vec3(16.7f,67.f,29.f);
    auto pointLight15 = PointLight::create(ambient2, diffuse2, specular2, position15, reglage_neon.x, reglage_neon.y, reglage_neon.z);
    lights->add_point_light(pointLight15);

    //neon5
    glm::vec3 position16 = glm::vec3(-5.f,64.f,29.f);
    auto pointLight16 = PointLight::create(ambient2, diffuse2, specular2, position16,reglage_neon.x, reglage_neon.y, reglage_neon.z);
    lights->add_point_light(pointLight16);

    //neon6
    glm::vec3 pos_neon6 = glm::vec3(29.f,55.f,-10.9f);
    auto neon6 = PointLight::create(ambient2, diffuse2, specular2, pos_neon6,reglage_neon.x, reglage_neon.y, reglage_neon.z);
    lights->add_point_light(neon6);

    //neon7
    glm::vec3 pos_neon7 = glm::vec3(13.2f,43.f,-28.6f);
    auto neon7 = PointLight::create(ambient2, diffuse2, specular2, pos_neon7,reglage_neon.x, reglage_neon.y, reglage_neon.z);
    lights->add_point_light(neon7);

    //neon8
    glm::vec3 pos_neon8 = glm::vec3(28.55f,90.f,-5.f);
    auto neon8 = PointLight::create(ambient2, diffuse2, specular2, pos_neon8,reglage_neon.x, reglage_neon.y, reglage_neon.z);
    lights->add_point_light(neon8);

    // lumiere---------------------------------------------------
    glm::vec3 rouge = glm::vec3(1.,0.,0.);
    glm::vec3 vert = glm::vec3(0.,1.,0.);
    glm::vec3 jaune = glm::vec3(0.5,0.45,0.);

    glm::vec3 reglage_lumiere = glm::vec3(0.50f, 0.001f, 0.352f);  

    //lumiere 1
    glm::vec3 position = glm::vec3(-23.6f,6.5f,12.5f);
    auto pointLight = PointLight::create(ambient2, diffuse2, specular2, position, reglage_lumiere.x, reglage_lumiere.y, reglage_lumiere.z);
    lights->add_point_light(pointLight);

    //lumiere 3
    glm::vec3 position3 = glm::vec3(31.0f,23.3f,16.f);
    auto pointLight3 = PointLight::create(ambient2, diffuse2, specular2, position3, reglage_lumiere.x, reglage_lumiere.y, reglage_lumiere.z);
    lights->add_point_light(pointLight3);

    //lumiere 2
    glm::vec3 position4 = glm::vec3(0.1f,9.3f,28.3f);
    auto pointLight4 = PointLight::create(rouge, diffuse2, specular2, position4, reglage_lumiere.x, reglage_lumiere.y, reglage_lumiere.z);
    lights->add_point_light(pointLight4);

    //lumière 4 
    glm::vec3 position10 = glm::vec3(8.9f,8.f,8.5f);
    auto pointLight10 = PointLight::create(ambient2, diffuse2, specular2, position10,reglage_lumiere.x, reglage_lumiere.y, reglage_lumiere.z);
    lights->add_point_light(pointLight10);

    //lumière 5 
    glm::vec3 position11 = glm::vec3(-28.f,25.f,-13.5f);
    auto pointLight11 = PointLight::create(ambient2, diffuse2, specular2, position11, reglage_lumiere.x, reglage_lumiere.y, reglage_lumiere.z);
    lights->add_point_light(pointLight11);

    //lumiere 6
    glm::vec3 position13 = glm::vec3(-28.f,90.f,-17.f);
    auto pointLight13 = PointLight::create(vert, diffuse2, specular2, position13, reglage_lumiere.x, reglage_lumiere.y, reglage_lumiere.z);
    lights->add_point_light(pointLight13);

    //lumiere 7
    glm::vec3 position17 = glm::vec3(28.8f,79.5f,27.f);
    auto pointLight17 = PointLight::create(ambient2, diffuse2, specular2, position17, reglage_lumiere.x, reglage_lumiere.y, reglage_lumiere.z);
    lights->add_point_light(pointLight17);

    //lumiere 8
    glm::vec3 position18 = glm::vec3(-13.f,79.5f,27.86f);
    auto pointLight18 = PointLight::create(ambient2, diffuse2, specular2, position18, reglage_lumiere.x, reglage_lumiere.y, reglage_lumiere.z);
    lights->add_point_light(pointLight18);

    //lumiere 9
    glm::vec3 pos_lumiere9 = glm::vec3(-13.f,79.5f,-28.86f);
    auto lumiere9 = PointLight::create(ambient2, diffuse2, specular2, pos_lumiere9, reglage_lumiere.x, reglage_lumiere.y, reglage_lumiere.z);
    lights->add_point_light(lumiere9);

    //lumiere 10
    glm::vec3 pos_lumiere10 =  glm::vec3(29.f,79.5f,-29.f);
    auto lumiere10 = PointLight::create(ambient2, diffuse2, specular2, pos_lumiere10, reglage_lumiere.x, reglage_lumiere.y, reglage_lumiere.z);
    lights->add_point_light(lumiere10);
    
    //lumiere11
    glm::vec3 diffuse4 = glm::vec3(0.1f,0.1f,0.1f);
    glm::vec3 specular4 = glm::vec3(0.1f,0.1f,0.1f);
    glm::vec3 pos_lumiere11 = glm::vec3(1.65f,79.5f,3.3f);
    auto lumiere11 = PointLight::create(jaune, diffuse4, specular4, pos_lumiere11, reglage_lumiere.x, reglage_lumiere.y, reglage_lumiere.z);
    lights->add_point_light(lumiere11);


    //spot--------------------------------------------------------
    glm::vec3 ambient3 = glm::vec3(0.3f,0.3f,0.3f);
    glm::vec3 diffuse3 = glm::vec3(1.0f,1.0f,1.0f);
    glm::vec3 specular3 = glm::vec3(0.2f,0.2f,0.2f);
    //spot joueur
    glm::vec3 pos_joueur = player->player_node->get_translation();
    std::shared_ptr<Camera> c = player->get_camera();
    glm::vec3 dir_joueur = c->getCFront();
    auto spot_joueur = TorchLight::create(ambient3, diffuse3, specular3, pos_joueur, 1.f, 0.3f, 0.002f,dir_joueur,40.f,50.f);
    lights->add_torch_light(spot_joueur);

    glm::vec3 ambient5 = glm::vec3(0.3f,0.3f,0.3f);
    glm::vec3 diffuse5 = glm::vec3(1.0f,1.0f,1.0f);
    glm::vec3 specular5 = glm::vec3(0.2f,0.2f,0.2f);

    //spot1 bétoniere
    glm::vec3 position5 = glm::vec3(-29.f,0.5f,24.5f);
    glm::vec3 direction1 = glm::vec3(0.8,0.23,-0.55);
    auto pointLight5 = TorchLight::create(ambient5, diffuse5, specular5, position5, 1.0f, 0.3f, 0.002f,direction1,40.f,120.f);
    lights->add_torch_light(pointLight5);

    //spot2
    glm::vec3 position2 = glm::vec3(-12.3f,0.5f,0.7f);
    glm::vec3 direction2 = glm::vec3(-0.4,0.2,0.9);
    auto pointLight2 = TorchLight::create(ambient5, diffuse5, specular5, position2, 1.5f, 0.5f, 0.012f,direction2,40.f,50.f);
    lights->add_torch_light(pointLight2);

    //spot3 plateforme
    glm::vec3 position12 = glm::vec3(-4.3f,30.f,-27.5f);
    glm::vec3 direction3 = glm::vec3(-0.6,-0.6,0.55);
    auto pointLight12 = TorchLight::create(ambient5, diffuse5, specular5, position12, 1.5f, 0.2f, 0.012f,direction3,40.f,50.f);
    lights->add_torch_light(pointLight12);

    //spot4 plateforme
    glm::vec3 pos_spot4 = glm::vec3(29.f,83.f,-29.f);
    glm::vec3 dir_spot_4 = glm::vec3(-0.6,-0.8,0.55);
    auto spot4 = TorchLight::create(ambient5, diffuse5, specular5, pos_spot4, 1.5f, 0.2f, 0.012f,dir_spot_4,40.f,50.f);
    lights->add_torch_light(spot4);

    //spot5 plateforme
    glm::vec3 pos_spot5 = glm::vec3(28.8f,88.f,27.f);
    glm::vec3 dir_spot_5 = glm::vec3(-0.43,-0.6,-0.7);
    auto spot5 = TorchLight::create(ambient5, diffuse5, specular5, pos_spot5, 1.5f, 0.2f, 0.012f,dir_spot_5,40.f,50.f);
    lights->add_torch_light(spot5);

    for(const auto& torch_light : lights->torch_lights) {
        torch_light->gen_shadow_map();
    }
}

void Scene::update_light_player(){
    if(lights->torch_lights[0]->power){
        lights->torch_lights[0]->on();
        glm::vec3 pos_joueur = player->player_node->get_translation();
        std::shared_ptr<Camera> c = player->get_camera();
        glm::vec3 pos_camera = c->getPosition();
        glm::vec3 dir_joueur = glm::normalize(c->getCFront());
        lights->torch_lights[0]->set_position(pos_joueur);
        lights->torch_lights[0]->direction=dir_joueur;
    }else{
        lights->torch_lights[0]->off();
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
        node->set_scale(glm::vec3(2.0f));
        if(node->mesh->material->name == "Echelle") { // Ladders
            node->rigid_body->is_ladder = true;
        }
        if(node->mesh->material->name == "Trampoline") { // Trampolines
            node->rigid_body->is_trampoline = true;
            node->rigid_body->restitution_coefficient = 1.0f;
        }
        if(node->mesh->material->name == "Glace") { // Ice
            node->rigid_body->friction_coefficient = -0.1f;
        }
        scene_nodes.push_back(node);
    }
}

void Scene::add_entities_into_physics_engine(std::shared_ptr<PhysicsEngine> pe) {
    for(auto& scene_node : scene_nodes) {
        if(scene_node->mesh->material->name != "Plante" && scene_node->mesh->material->name != "Clou" && scene_node->mesh->material->name != "UnderTrampoline") { // To prevent plants collisions and under trampoline AABB bug
            pe->add_entity(scene_node);
        }
    }
}

void Scene::updateAABB() {
    for(const auto& scene_node : scene_nodes) {
        scene_node->updateAABB();
    }
}

void Scene::draw(glm::mat4& view, glm::mat4& projection, unsigned int width, unsigned int height) {
    update_light_player();
    auto shadow_shader = shaders->getShadowShader();
    shadow_shader->useShader();
    for (auto& torch_light : lights->torch_lights) {
        torch_light->setup_shadow_map(shadow_shader);
        torch_light->shadow_map->bind();
        for(auto& scene_node : scene_nodes) { 
            scene_node->computeShadow(shadow_shader);
        }
        torch_light->shadow_map->unbind();
    }
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    lights->setup_lights(shaders->getShader());
    for(auto& scene_node : scene_nodes) {
        scene_node->draw(view, projection);
    }
}

void Scene::on_off_torch_light_player(){
    lights->torch_lights[0]->power=!lights->torch_lights[0]->power;
}

void Scene::mode_torch_light_player(){
    lights->torch_lights[0]->mode=!lights->torch_lights[0]->mode;
}