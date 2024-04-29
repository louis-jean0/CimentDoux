#include <Scene.hpp>

Scene::Scene(){};

void Scene::creation_plan(const char* image_path,unsigned int grid_x, unsigned int grid_z, unsigned int size, unsigned int height_scale,Shader &shader){
	Plane* plane = new Plane(grid_x,grid_z,size,height_scale);
	Texture plane_texture(image_path);
    plane->add_texture(plane_texture);
    plane->bind_shader(shader);
    list_plane.push_back(plane);
    SceneNode plane_node(plane);
    list_scene.push_back(plane_node);
}

// void Scene::creationMap(Shader &shader){
	
//     Model obst1("../data/models/cube/Cube.gltf");
//     obst1.bind_shader_to_meshes(shader);
//     SceneNode* obst1_node = new SceneNode(&obst1);
//     obst1_node->transform.scale = glm::vec3(1.0f);
//     obst1_node->transform.translation = glm::vec3(15., 1.0f, 15.);
//     list_model.push_back(obst1_node);

//     Model obst2("../data/models/cube/Cube.gltf");
//     obst2.bind_shader_to_meshes(shader);
//     SceneNode* obst2_node = new SceneNode(&obst2);
//     obst2_node->transform.scale = glm::vec3(1.0f);
//     obst2_node->transform.translation = glm::vec3(15., 2.0f, 17);
//     list_model.push_back(obst2_node);

//     Model obst3("../data/models/cube/Cube.gltf");
//     obst3.bind_shader_to_meshes(shader);
//     SceneNode* obst3_node = new SceneNode(&obst3);
//     obst3_node->transform.sescale = glm::vec3(1.0f);
//     obst3_node->transform.translation = glm::vec3(15., 3.0f, 19);
//     list_model.push_back(obst3_node);

//     Model obst4("../data/models/cube/Cube.gltf");
//     obst4.bind_shader_to_meshes(shader);
//     SceneNode* obst4_node = new SceneNode(&obst4);
//     obst4_node->transform.scale = glm::vec3(2.0f,1.f,3.f);
//     obst4_node->transform.translation = glm::vec3(15., 4.0f, 23);
//     list_model.push_back(obst4_node);

//     Model obst5("../data/models/cube/Cube.gltf");
//     obst5.bind_shader_to_meshes(shader);
//     SceneNode* obst5_node = new SceneNode(&obst5);
//     obst5_node->transform.scale = glm::vec3(3.0f,1.f,5.f);
//     obst5_node->transform.translation = glm::vec3(15., 5.0f, 33);
//     list_model.push_back(obst5_node);

//     Model obst6("../data/models/cube/Cube.gltf");
//     obst6.bind_shader_to_meshes(shader);
//     SceneNode* obst6_node = new SceneNode(&obst6);
//     obst6_node->transform.scale = glm::vec3(2.0f,1.f,3.f);
//     obst6_node->transform.translation = glm::vec3(15., 4.0f, 83.5);
//     list_model.push_back(obst6_node);
// }

void Scene::draw_plan(glm::mat4 &view, glm::mat4 &projection){
	int t=list_scene.size();
	for(int i=0;i<t;++i){
		list_scene[i].draw(view, projection);
	}
}

void Scene::draw_model(glm::mat4 &view, glm::mat4 &projection){
	int t=list_model.size();
	for(int i=0;i<t;++i){
		list_model[i]->draw(view, projection);
	}
}

void Scene::draw(glm::mat4 &view, glm::mat4 &projection){
	draw_plan(view, projection);
	draw_model(view, projection);
}