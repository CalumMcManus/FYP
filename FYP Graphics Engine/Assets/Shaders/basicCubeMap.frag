#version 430
in vec3 texcoords;
uniform samplerCube cube_texture;
layout (location = 0) out vec3 PositionTex;
layout (location = 1) out vec3 NormalTex;
layout (location = 2) out vec3 ColorTex;
layout (location = 3) out vec3 UnlitColorTex;

void main() {
	//PositionTex = texture (cube_texture, texcoords).xyz;
	//NormalTex = texture (cube_texture, texcoords).xyz;
	//ColorTex = texture (cube_texture, texcoords).xyz;
	UnlitColorTex = texture (cube_texture, texcoords).xyz;

}
