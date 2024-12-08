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

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

float ShadowCalculation(vec4 FragPosLightSpace) {
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0) return 0.0;

    float currentDepth = projCoords.z;
    //float bias = max(0.005 * (1.0 - dot(normalize(normal), normalize(LightDirection))), 0.001);
    float bias = 0;

    float shadow = 0.0;
    vec2 textSize = textureSize(ShadowMap, 0);
    vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);
    for (int x = -2; x <= 2; ++x) {
        for (int y = -2; y <= 2; ++y) {
            float pcfDepth = texture(ShadowMap, projCoords.xy + vec2(x, y) * texelSize + (rand(projCoords.xy + vec2(x, y))/textSize)).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 25.0;
    return shadow;
}

void main() {
    vec3 norm = normalize(normal);

    // ambient light
    vec3 ambientLight = vec3(0.1);

    // main light
    float diffuse = max(dot(norm, normalize(LightDirection)), 0.0);
    float shadow = ShadowCalculation(fragPosLightSpace);
    vec3 mainLighting = (0.2 + (1.0 - shadow) * diffuse) * vec3(1.0);

    // additional light
    vec3 additionalLighting = vec3(0.0);

    // point light
    float pointRadius = 15.0;
    for (int i = 0; i < int(LightCount); ++i) {
        vec3 toFrag = fragPos - AdditionalLightPositions[i];
        float distance = length(toFrag);

        if (distance < pointRadius) {
            float attenuation = 1.0 - (distance / pointRadius);
            vec3 lightDir = normalize(-toFrag);
            float lightDiffuse = max(dot(norm, lightDir), 0.0);
            additionalLighting += lightDiffuse * AdditionalLightIntensities[i] * AdditionalLightColors[i] * attenuation;
        }
    }

    // linear light
    float beamRadius = 3.5;
    for (int i = 0; i < int(AdditionalDirectionalLightCount); ++i) {
        vec3 Lpos = AdditionalDirectionalLightPositions[i];
        vec3 Ldir = normalize(-AdditionalDirectionalLightDirections[i]);
        vec3 toFrag = Lpos - fragPos;

        float alongBeam = dot(toFrag, Ldir);
        if (alongBeam > 0.0) {
            vec3 projected = Ldir * alongBeam;
            float distFromAxis = length(toFrag - projected);

            if (distFromAxis < beamRadius) {
                float attenuation = 1.0 - (distFromAxis / beamRadius);
                float lightDiffuse = max(dot(norm, Ldir), 0.0);
                additionalLighting += lightDiffuse * AdditionalDirectionalLightIntensities[i] * AdditionalDirectionalLightColors[i] * attenuation;
            }
        }
    }

    // final light
    vec3 finalLighting = ambientLight + mainLighting + additionalLighting;

    // gamma correction
    const float gamma = 2.2;
    finalLighting = pow(finalLighting, vec3(1.0 / gamma));

    // final fragment color
    vec4 texColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y));
    FragmentColor = vec4(texColor.rgb * finalLighting, texColor.a * Transparency);
}
