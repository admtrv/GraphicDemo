#version 330 core

uniform sampler2D Texture;
uniform vec3 LightDirection;
uniform float Transparency;
uniform vec2 TextureOffset;

in vec2 texCoord;
in vec3 normal;

out vec4 FragmentColor;

void main() {
    vec3 norm = normalize(normal);
    float diffuse = max(dot(norm, normalize(LightDirection)), 0.0);
    float ambient = 0.6;
    float lighting = min(ambient + diffuse, 1.0);

    vec4 texColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset);

    FragmentColor = texColor * lighting;

    FragmentColor.a *= Transparency;
}
