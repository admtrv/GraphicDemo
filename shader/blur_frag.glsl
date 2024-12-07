#version 330 core
#extension GL_ARB_separate_shader_objects : enable

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float offset = 1.0/512.0;

void main() {
    vec3 result = texture(screenTexture, TexCoords).rgb * 0.382;

    result += texture(screenTexture, TexCoords + vec2(offset, 0.0)).rgb * 0.25;
    result += texture(screenTexture, TexCoords - vec2(offset, 0.0)).rgb * 0.25;
    result += texture(screenTexture, TexCoords + vec2(0.0, offset)).rgb * 0.07;
    result += texture(screenTexture, TexCoords - vec2(0.0, offset)).rgb * 0.07;

    FragColor = vec4(result, 1.0);
}
