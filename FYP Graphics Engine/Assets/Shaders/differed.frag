#version 410

layout (location = 0) out vec3 PositionTex;
layout (location = 1) out vec3 NormalTex;
layout (location = 2) out vec3 ColorTex;


out vec4 fragment_colour;

in vec2 TexCoords;
in vec3 Position;
in vec3 Normal;

uniform sampler2D texture2D;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    PositionTex = Position;
    // also store the per-fragment normals into the gbuffer
    NormalTex = normalize(Normal);
    // and the diffuse per-fragment color
    ColorTex = texture(texture2D, TexCoords).rgb;
	vec4 color = texture(texture2D, TexCoords);
	fragment_colour = color;
} 
