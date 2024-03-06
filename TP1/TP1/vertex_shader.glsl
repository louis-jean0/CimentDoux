#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;
layout(location = 1) in vec2 vertexUV;

//TODO create uniform transformations matrices Model View Projection
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform sampler2D heightmap;
uniform float heightmapScale;

out vec2 UV;
out float height;

void main() {
        height = texture(heightmap,vertexUV).r * heightmapScale;
        vec3 pos = vertices_position_modelspace;
        pos.z = height;
        UV = vertexUV;
        gl_Position = projection_matrix * view_matrix * model_matrix * vec4(pos,1);
}