#pragma once

#include <Plane.hpp>
#include <SceneNode.hpp>
#include <Shader.hpp>

class Scene{
public:

	Scene();
	void creation_plan(const char* image_path,unsigned int grid_x, unsigned int grid_z, unsigned int size, unsigned int height_scale,Shader &s);
	void creationMap(Shader &s);
	void draw_plan();
	void draw_model();
	void draw();

private: 
	std::vector<Plane*> list_plane;
	std::vector<SceneNode> list_scene;
	std::vector<SceneNode*> list_model;



};