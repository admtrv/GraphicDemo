#version 330 core

layout(location = 0) in vec3 Position;
layout(location = 2) in vec3 Normal;
layout(location = 1) in vec2 TexCoord;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 LightSpaceMatrix;

out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;
out vec4 fragPosLightSpace;

void main() {
  texCoord = TexCoord;
  normal = normalize(mat3(transpose(inverse(ModelMatrix))) * Normal);
  fragPos = vec3(ModelMatrix * vec4(Position, 1.0));
  fragPosLightSpace = LightSpaceMatrix  * ModelMatrix * vec4(Position, 1.0);
  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);
}
