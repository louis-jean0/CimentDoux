#version 330 core

in vec2 UV;
in float height;
// Ouput data
out vec3 color;

uniform sampler2D grassSampler;
uniform sampler2D rockSampler;
uniform sampler2D snowrockSampler;
uniform float heightmapScale;

void main() {
        float grassHeight = 0.2 * heightmapScale;
        float rockHeight = 0.6 * heightmapScale;
        float snowrockHeight = 0.7 * heightmapScale;
        vec3 grass = texture(grassSampler,UV).rgb;
        vec3 rock = texture(rockSampler,UV).rgb;
        vec3 snowrock = texture(snowrockSampler,UV).rgb;

        if(height <= grassHeight) {
                color = grass;
        }
        else if(height <= rockHeight) {
                float t = (height - grassHeight) / (rockHeight - grassHeight);
                color = mix(grass,rock,t);
        }
        else if(height <= snowrockHeight) {
                float t = (height - rockHeight) / (snowrockHeight - rockHeight);
                color = mix(rock,snowrock,t);
        }
        else {
                color = snowrock;
        }
}