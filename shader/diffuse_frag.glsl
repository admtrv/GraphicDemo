#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

// Direction of light
uniform vec3 LightDirection;

// (optional) Transparency
uniform float Transparency;

// (optional) Texture offset
uniform vec2 TextureOffset;

// The vertex shader will feed this input
in vec2 texCoord;

// Wordspace normal passed from vertex shader
in vec3 normal;

// The final color
out vec4 FragmentColor;

void main() {
  // Normalize the normal vector
  vec3 norm = normalize(normal);

  // Compute diffuse lighting
  float diffuse = max(dot(norm, normalize(LightDirection)), 0.0);

  // Ambient lighting factor
  float ambient = 0.6;

  // Combine ambient and diffuse lighting
  float lighting = ambient + diffuse;

  // Ensure the lighting value does not exceed 1.0
  lighting = min(lighting, 1.0);

  // Lookup the color in Texture on coordinates given by texCoord
  // NOTE: Texture coordinate is inverted vertically for compatibility with OBJ
  vec4 texColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset);

  // Apply lighting to the texture color
  FragmentColor = texColor * lighting;
  FragmentColor.a = Transparency;
}
