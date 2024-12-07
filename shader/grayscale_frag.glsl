#version 330 core
#extension GL_ARB_separate_shader_objects : enable

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main() {
    vec3 col = texture(screenTexture, TexCoords).rgb;
    float gray = dot(col, vec3(0.299, 0.587, 0.114));
    FragColor = vec4(vec3(gray), 1.0);
}