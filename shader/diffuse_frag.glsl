#version 330 core

uniform sampler2D Texture;
uniform sampler2D ShadowMap;

uniform vec3 LightDirection;
uniform float Transparency;
uniform vec2 TextureOffset;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
in vec4 fragPosLightSpace;

out vec4 FragmentColor;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

float ShadowCalculation(vec4 FragPosLightSpace)
{
    vec3 projCoords = FragPosLightSpace.xyz/ FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float currentDepth = projCoords.z;

    float bias = max(0.005 * (1.0 - dot(normalize(normal), normalize(LightDirection))), 0.001);

    float shadow = 0.0;
    vec2 textSize = textureSize(ShadowMap, 0);
    vec2 texelSize = 1.0 / textSize;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(ShadowMap, projCoords.xy + vec2(x, y)*texelSize + (rand(projCoords.xy + vec2(x, y))/textSize) ).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    //if(projCoords.z > 1.0)
    //shadow = 0.0;

    return shadow;
}
void main() {
    vec3 norm = normalize(normal);
    float diffuse = max(dot(norm, normalize(LightDirection)), 0.0);
    float ambient = 0.6;
    float shadow = ShadowCalculation(fragPosLightSpace);
    float lighting = min(ambient + (1.0 - shadow) * diffuse, 1.0);

    vec4 texColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset);

    FragmentColor = texColor * lighting;

    FragmentColor.a *= Transparency;
}
