#version 330 core
#extension GL_ARB_separate_shader_objects : enable

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D sceneTexture;
uniform float threshold;

void main() {
    vec3 color = texture(sceneTexture, TexCoords).rgb;
    float brightness = dot(color, vec3(0.2126,0.7152,0.0722));
    if(brightness > threshold)
        FragColor = vec4(color, 1.0);
    else
        FragColor = vec4(0.0);
}
