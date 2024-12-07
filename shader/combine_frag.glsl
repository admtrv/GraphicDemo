#version 330 core
#extension GL_ARB_separate_shader_objects : enable

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D sceneTexture;
uniform sampler2D bloomTexture;
uniform float bloomIntensity;

void main() {
    vec3 sceneColor = texture(sceneTexture, TexCoords).rgb;
    vec3 bloomColor = texture(bloomTexture, TexCoords).rgb;
    vec3 finalColor = sceneColor + bloomColor * bloomIntensity;
    FragColor = vec4(finalColor, 1.0);
}
