#version 410

layout (location = 0) out vec3 PositionTex;
layout (location = 1) out vec3 NormalTex;
layout (location = 2) out vec4 ColorTex;
layout (location = 3) out vec3 Components;


out vec4 fragment_colour;

in vec2 TexCoords;
in vec3 Position;
in vec3 Normal;

uniform vec3 color;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    PositionTex = Position;
    // also store the per-fragment normals into the gbuffer
    NormalTex = normalize(Normal);
    // and the diffuse per-fragment color
    ColorTex = vec4(color, -1.0);
	Components = vec3(0, 0, 0);
    fragment_colour = vec4(color, 1.0);
} 
