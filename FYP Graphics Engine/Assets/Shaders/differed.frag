#version 410

layout (location = 0) out vec3 PositionTex;
layout (location = 1) out vec3 NormalTex;
layout (location = 2) out vec4 ColorTex;
layout (location = 3) out vec3 Components;

out vec4 fragment_colour;

in vec2 TexCoords;
in vec3 Position;
in vec3 Normal;
in vec3 toCameraVector;

uniform sampler2D texture2D;
uniform sampler2D specular2D;
uniform sampler2D normal2D;
uniform sampler2D metalic2D;
uniform sampler2D rough2D;

uniform bool UseTexture;
uniform bool UseSpecular;
uniform bool UseNormal;
uniform bool UseMetalic;
uniform bool UseRough;

struct Material {
	vec3 Color;
	vec3 Spec;
};
uniform Material material;

in mat3 TBN;
in vec3 tan;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    PositionTex = Position;
    // also store the per-fragment normals into the gbuffer
	
    
    // and the diffuse per-fragment color
	if(UseTexture)
	{
		ColorTex.rgb = texture(texture2D, TexCoords).rgb * material.Color;
	}
	else{
		ColorTex.rgb = material.Color;
	}
	if(UseSpecular)
	{
		ColorTex.a = texture(specular2D, TexCoords).r;
	}
	else
	{
		ColorTex.a = 0;
	}
	if(UseNormal)
	{
		NormalTex = normalize(TBN * (texture(normal2D, TexCoords).rgb * 2 - 1));
	}
	else
	{
		NormalTex = normalize(Normal);
	}
	if(UseMetalic)
	{
		Components.x = texture(metalic2D, TexCoords).r;
	}
	else
	{
		Components.x = 0;
	}
	if(UseRough)
	{
		Components.y = texture(rough2D, TexCoords).r;
	}
	else
	{
		Components.y = 0;
	}
	Components.z = 0; //Well be used for reflectivity
} 
