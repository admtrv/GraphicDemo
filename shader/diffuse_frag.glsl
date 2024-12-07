#version 330 core

uniform sampler2D Texture;
uniform sampler2D ShadowMap;

uniform vec3 LightDirection;

uniform vec3 AdditionalLightPositions[8];
uniform vec3 AdditionalLightColors[8];
uniform float AdditionalLightIntensities[8];
uniform bool AdditionalLightTypes[8];
uniform float LightCount;

uniform vec3 AdditionalDirectionalLightPositions[8];
uniform vec3 AdditionalDirectionalLightDirections[8];
uniform vec3 AdditionalDirectionalLightColors[8];
uniform float AdditionalDirectionalLightIntensities[8];
uniform float AdditionalDirectionalLightCount;

uniform float Transparency;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
in vec4 fragPosLightSpace;

out vec4 FragmentColor;

float ShadowCalculation(vec4 FragPosLightSpace) {
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0) return 0.0;

    float currentDepth = projCoords.z;
    float bias = max(0.005 * (1.0 - dot(normalize(normal), normalize(LightDirection))), 0.001);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    return shadow;
}

void main() {
    vec3 norm = normalize(normal);

    float diffuse = max(dot(norm, normalize(LightDirection)), 0.0);
    float shadow = ShadowCalculation(fragPosLightSpace);
    vec3 mainLighting = (0.2 + (1.0 - shadow) * diffuse) * vec3(1.0);

    vec3 additionalLighting = vec3(0.0);
    for (int i = 0; i < LightCount; ++i) {
        vec3 lightDir;
        lightDir = normalize(AdditionalLightPositions[0] - fragPos);
        float lightDiffuse = max(dot(norm, lightDir), 0.0);
        additionalLighting += lightDiffuse * AdditionalLightIntensities[0] * AdditionalLightColors[0];
    }
    for (int i = 0; i < AdditionalDirectionalLightCount; ++i) {
        vec3 lightDir;
        lightDir = normalize(-AdditionalDirectionalLightDirections[0]);
        float lightDiffuse = max(dot(norm, lightDir), 0.0);
        additionalLighting += lightDiffuse * AdditionalDirectionalLightIntensities[0] * AdditionalDirectionalLightColors[0];
    }
    vec3 finalLighting = mainLighting + additionalLighting;

    const float gamma = 2.2;
    finalLighting = pow(finalLighting, vec3(1.0 / gamma));

    vec4 texColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y));
    FragmentColor = vec4(texColor.rgb * finalLighting, texColor.a * Transparency);
}
